//
// Traffic simulator UI actions
//

#include <QTimer>
#include <QFileDialog>
#include <iostream>
#include <QWidget>
#include <QMessageBox>
#include <QDesktopWidget>
#include <iostream>
#include "ui_aboutdialog.h"
#include "math.h"

// Simultra
#include "traffic.h"
#include "ui_simulator.h"
#include "plan.h"
#include "log.h"
#include "utils.h"
#include "graph.h"
#include "plan.h"
#include "world.h"
#include "walker.h"
#include "node.h"
#include "edge.h"
#include "gvzoom.h"
#include "simulation.h"
#include "config.h"
#include "conversions.h"
#include "weather.h"
#include "vehimob.h"
#include "debug.h"
#include "driver.h"

// MapGraphics
#include "MapGraphics/MapGraphicsView.h"
#include "MapGraphics/MapGraphicsScene.h"
#include "MapGraphics/tileSources/GridTileSource.h"
#include "MapGraphics/tileSources/OSMTileSource.h"
#include "MapGraphics/tileSources/CompositeTileSource.h"
#include "MapGraphics/guts/CompositeTileSourceConfigurationWidget.h"
#include "MapGraphics/CircleObject.h"
#include "MapGraphics/PolygonObject.h"
#include "MapGraphics/LineObject.h"

using namespace std;

QPointF drawCircle(QGraphicsScene *scene, QPointF point, qreal radius, QColor colour)
{
  QPointF center( point.x() + radius/2, point.y() + radius/2 );
  scene->addEllipse(point.x(), point.y(), radius, radius, QPen(colour), QBrush(Qt::transparent));
  scene->addEllipse(center.x(), center.y(), 0.5, 0.5, QPen(colour), QBrush(colour));
  return center;
}

void TrafficSimulator::On_about_Action()
{
    ui->aboutPushButton->setDown(false);

    QDialog dialog(this);
    dialog.move(350, 200);

    Ui::AboutDialog About;
    About.setupUi(&dialog);
    dialog.setWindowFlags(dialog.windowFlags() & ~Qt::WindowContextHelpButtonHint);
    dialog.setWindowFlags(dialog.windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    dialog.setWindowFlags(dialog.windowFlags() | Qt::FramelessWindowHint);

    About.Version->setText(About.Version->text().arg(_SIMULTRA_VERSION_));
    About.QTVersion->setText(About.QTVersion->text().arg(qVersion()).arg(QT_VERSION_STR));
    About.osmVersion->setText(About.osmVersion->text().arg(OSM_VERSION_STR));
    About.xVersion->setText(About.xVersion->text().arg(X_VERSION_STR));

    QFile ct(_CHANGELOG_);
    ct.open(QIODevice::ReadOnly);
    QTextStream cl(&ct);
    About.txtChangelog->setPlainText(cl.readAll());
    QPixmap px(_LOGO_);
    About.pxIcon->setPixmap(px);
    //About.lblUrl->setOpenExternalLinks(true);

    dialog.exec();
}

void TrafficSimulator::setWalkers(int numberOfWalkers, QPointF* walk, int swalk, QColor colour)
{
    qreal size = 0.08f;

    for (int j = 0; j < numberOfWalkers; j++)
    {
           Walker *walker = new Walker(colour, size);
           walker->set_walk(walk, swalk);
           walker->setName("walker " + QString::number(j));
           walker->setView(ui->simulationView);
           walker->setPos(0, 0);
           ui->simulationView->scene()->addItem(walker);
    }
}

// MapGraphics-based

void TrafficSimulator::addOsmMapGraphics(QWidget* parent)
{
    // Setup the MapGraphics scene and view
    _OsmScene = new MapGraphicsScene(parent);
    _OsmView = new MapGraphicsView(_OsmScene, parent);

    // Setup some tile sources
    QSharedPointer<OSMTileSource> osmTiles(new OSMTileSource(OSMTileSource::OSMTiles), &QObject::deleteLater);
    QSharedPointer<GridTileSource> gridTiles(new GridTileSource(), &QObject::deleteLater);
    QSharedPointer<CompositeTileSource> composite(new CompositeTileSource(), &QObject::deleteLater);
    // MapQuestAerialTiles was removed.
    composite->addSourceBottom(osmTiles);
    composite->addSourceTop(gridTiles);
    _OsmView->setTileSource(composite);

    // Create a widget in the dock that lets us configure tile source layers
    CompositeTileSourceConfigurationWidget * tileConfigWidget = new CompositeTileSourceConfigurationWidget(composite.toWeakRef(), ui->dockWidget);
    ui->dockWidget->setWidget(tileConfigWidget);
    delete ui->dockWidgetContents_3;
    ui->fileMenu->addAction(ui->dockWidget->toggleViewAction());
    ui->dockWidget->toggleViewAction()->setText("&Layers");

    // To hide the title bar completely must replace the default widget with a generic one
    QWidget * titleWidget = new QWidget(this);
    ui->dockWidget->setTitleBarWidget(titleWidget);

    // Init zoom
    _OsmView->setZoomLevel(_INITIAL_ZOOM_); // MapGraphicsView::ZoomMode::MouseZoom/CenterZoom
    ui->spinBox->setValue(_INITIAL_ZOOM_);
    _OsmView->setFixedSize(parent->size());

    // Center the view in this point of the OSM map:
    QPointF center(_CENTER_IN_X_, _CENTER_IN_Y_);
    _OsmView->centerOn(center);

    //_weatherManager = new JWeatherManager(&_OsmScene, this);
}

void TrafficSimulator::disactivateWeather()
{
    // Weather
    if (_weatherManager)
    {
        _weatherManager->clear();
        delete _weatherManager;
        _weatherManager = 0;
        if (_numVehicles)
            for (int i = 0 ; i < _numVehicles ; i++)
                    _vehicles[i]->setOpacity(1.0);
    }
    else
    {
        _weatherManager = new JWeatherManager(&_OsmScene, this);
        for (int i = 0 ; i < _numVehicles ; i++)
                _vehicles[i]->setOpacity(0.0);
    }
}

// TODO use the real lat/lng from OSM.
void TrafficSimulator::addComponents()
{
    if (_Simulation && _Simulation->_Plan)
    {
      foreach(Plan::Road* road, _Simulation->_Plan->getRoads())
      {
          //qreal width = road->width();
          debug("first (%f, %f)\n",  road->first().x() ,  road->first().y() );
          LineObject* line = new LineObject(
                      Position(road->first().x(), road->first().y()),
                      Position(road->second().x(), road->second().y()),
                      1.0, 0);
          _OsmScene->addObject(line);
      }
    }
}

TrafficSimulator::TrafficSimulator(QWidget *parent) : QMainWindow(parent), ui(new Ui::TrafficSimulator)
{
      _center = 0;

      ui->setupUi(this);
      ui->toolButton->setCheckable(true); // drag
      ui->toolButton->setChecked(false);
      ui->weatherPushButton->setCheckable(true);
      ui->weatherPushButton->setChecked(false);

      QFile file(_QSS_);
      file.open(QFile::ReadOnly);
      QString styleSheet = QLatin1String(file.readAll());
      setStyleSheet(styleSheet);

      // Adding OSM map
      addOsmMapGraphics(ui->OSMWidget);

      _Simulation = Simulation::instance();
      _beingRead = false;

      if (_Simulation)
      {
            _SimRealTime = true;
            _Clock = new QTimer(this);
            connect(_Clock, SIGNAL(timeout()), this, SLOT(OnClock()));
            _Clock->setInterval(_CLOCK_INTERVAL_);
            if (_SimRealTime)
              _Clock->start();

            // Select types, map and scenario.
            _Simulation->openXMLFileTypes(_SIMULATION_TYPES_);
            _Simulation->openXMLFileSignalization(_SIMULATION_MAP_);
            _Simulation->openXMLFileSimulation(_SIMULATION_SCENARIO_);

            // renders the old grid-based map. (roads)
            //-- ui->simulationView->redrawPlan(); // TODO

            // Load the (OSM) world here, pass paths to vehicles:
            // _Simulation has a list of vehicles (_Vehicles).
            // {{
            _world = new World();
            _world->setName("id" + QString::number(1));
            _world->setView(ui->simulationView);
            _world->render( ui->simulationView->scene() );
            _Simulation->setWorld(_world);

            // TODOstat not required?

            // Setting vehicles

            _numVehicles = _Simulation->_Vehicles.length();

            debug("Leading %d vehicles.", _numVehicles);

            for (int i = 0 ; i < _numVehicles; i++)
            {
                    QImage vehImage = QImage(_Simulation->_Vehicles[i]->type()->_NameImage);
                    _vehicles[i] = new VehicleImageObject(vehImage, 0);
                    _vehicles[i]->setRotation(40);
                    _vehicles[i]->set_Type(1);
                    _vehicles[i]->setFlag(VehicleImageObject::ObjectIsFocusable, true);
                    _vehicles[i]->setFlag(VehicleImageObject::ObjectIsMovable, true);
                    _vehicles[i]->setFlag(VehicleImageObject::ObjectIsSelectable, true);
                    _OsmScene->addObject(_vehicles[i]);
            }

            // TODO remove calculate paths calculation in _world.
            //QVector<QPainterPath> paths = _world->getPaths();

            for (int i = 0 ; i < _numVehicles ; i++)
            {
                // Building the QPainterPath from coods.
                QPolygonF polygon_;
                QList<Driver::Coordinates> vpath = _Simulation->_Vehicles[i]->driver()->get_Path();
                for(int j = vpath.length() - 1; j >= 0; j--)
                    polygon_.append(QPoint(vpath[j].intersection->location().x(),
                                           vpath[j].intersection->location().y()));
                QPainterPath path;
                path.addPolygon(polygon_);
                _Simulation->_Vehicles[i]->set_path(path);

                # if _TRACE_
                debug("Intersections:");
                foreach(Driver::Coordinates coord, vpath)
                    debug("  <Intersection name=\"\" x=\"%f\" y=\"%f\"/>", coord.intersection->location().x(), coord.intersection->location().y());
                #endif

                _Simulation->_Vehicles[i]->set_scene(ui->simulationView->scene());
                _Simulation->_Vehicles[i]->set_OSMvehicle(_vehicles[i], true);
            }

            _Simulation->_ImagesManager.addImages();

            ui->simulationView->refreshImages(); // render the vehicles
            ui->simulationView->centrer();
            ui->simulationView->renderVelocity(false);
            ui->simulationView->renderFrictions(false);
            ui->simulationView->renderMotorForce(false);

            _Simulation->setUI(ui);

            ui->lcdTimer->setDigitCount(9);
            ui->lcdTimer->display(QTime(0, 0, 0, 0).toString(QString("hh:mm:ss")));
            ui->checkBox->setEnabled(false);   // Protocols
            ui->checkBox_2->setChecked(true);
            ui->checkBox_3->setEnabled(false); // Sensor
            ui->checkBox_4->setChecked(true);
            ui->checkBox_5->setChecked(true);
            ui->checkBox_6->setChecked(true);
            ui->checkBox_7->setChecked(true); // Walkway
            ui->spinBox->setRange(_MIN_ZOOMING_, _MAX_ZOOMING_);

            Logs::init();

            // Load intersection names here and render them when intersection is clicked

            foreach(Plan::Intersection* intersec , _Simulation->_Plan->getIntersections())
            {
                debug("Intersection '%s' : %s", (char *) intersec->name().toStdString().c_str(), (char *) QString(p_s(intersec->location())).toStdString().c_str());

                auto parent = ui->simulationView->scene()->addRect(QRectF(0, 0, 100, 100), QPen(Qt::transparent), QBrush(Qt::transparent));
                _Markers[intersec->name()] = new QGraphicsTextItem(parent);
                _Markers[intersec->name()]->setPlainText(intersec->name().compare("") ? intersec->name() : QString("Origin ") + p_s(intersec->location()) );
                _Markers[intersec->name()]->setPos(intersec->location());

                QFont font_("San Serif", 2.0, QFont::Monospace, true);
                _Markers[intersec->name()]->setFont(font_);
                _Markers[intersec->name()]->setDefaultTextColor(Qt::white);
                _Markers[intersec->name()]->setZValue(Utils::LANE_LAYER);
                _Markers[intersec->name()]->setOpacity(1.0);
                _Markers[intersec->name()]->setRotation(85);
                ui->simulationView->scene()->addItem(_Markers[intersec->name()]);
                _Markers[intersec->name()]->hide();

                // Adds small magent dots
                // ui->simulationView->scene()->addEllipse(intersec->location().x(), intersec->location().y(), 1, 1, QPen(Qt::magenta), QBrush(Qt::magenta));
            }


             // Mouse zooming
             Graphics_view_zoom* z = new Graphics_view_zoom(ui->simulationView);
             z->set_modifiers(Qt::NoModifier);

             connect(ui->increasesizeCarButton, SIGNAL(pressed()), this, SLOT(increasesizeVehicles()));
             connect(ui->decreasesizeCarButton, SIGNAL(pressed()), this, SLOT(decreasesizeVehicles()));
             connect(ui->toolButton, SIGNAL(toggled(bool)), this, SLOT(OnSelectDragMode()));
             connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(OnOsmZooming(int)));
             connect(ui->goPushButton, SIGNAL(pressed()), this, SLOT(go()));
             connect(ui->aboutPushButton, SIGNAL(pressed()), this, SLOT(On_about_Action()));
             connect(ui->weatherPushButton, SIGNAL(pressed()), this, SLOT(disactivateWeather()));

             ui->simulationView->rotate(-85); // to fit the OSM rendering

            if (!_SimRealTime)
              _Simulation->start();
       }

       _alpha = 1.0;
}


void TrafficSimulator::increasesizeVehicles()
{
    for (int i = 0 ; i < _numVehicles ; i++)
        if (_vehicles[i])
            if ( _vehicles[i]->get_Type() == 1 )
                    _vehicles[i]->set_Scale((_vehicles[i]->get_Scale() == 0.1) ? 0.01 : _vehicles[i]->get_Scale() + 0.01);
}


void TrafficSimulator::decreasesizeVehicles()
{
    for (int i = 0 ; i < _numVehicles ; i++)
        if (_vehicles[i])
            if ( _vehicles[i]->get_Type() == 1 )
                    _vehicles[i]->set_Scale( (_vehicles[i]->get_Scale() == 0.0) ? 0.01 : _vehicles[i]->get_Scale() - 0.01);
}

QImage createSubImage(QImage* image, const QRect & rect)
{
    size_t offset = rect.x() * image->depth() / 8 + rect.y() * image->bytesPerLine();
    return QImage(image->bits() + offset, rect.width(), rect.height(), image->bytesPerLine(), image->format());
}

// Goes to a particular location. TEST ONLY
void TrafficSimulator::go()
{
    cout << "Go!" << endl;

    QPointF ShortPath_(136.90082390000001, 35.170364399999997);
    if (_zoomLevel < 19)
    {
        OnOsmZooming(_zoomLevel+2);
        _OsmView->centerOn(ShortPath_);
        _OsmView->update();
    }
    cout << "doubleSpinBoxLat = " << fixed << ui->doubleSpinBoxLat->value() << endl;
    cout << "doubleSpinBoxLat = " << fixed << ui->doubleSpinBoxLng->value() << endl;
}

void TrafficSimulator::OnOsmZooming(int value)
{
    ui->spinBox->setValue(value);
    _OsmView->setZoomLevel(value);
    _zoomLevel = value;
}

void TrafficSimulator::OnSelectDragMode()
{
    ui->simulationView->setDragMode(ui->toolButton->isChecked() ?
                                        QGraphicsView::ScrollHandDrag :
                                        QGraphicsView::RubberBandDrag );
    ui->simulationView->setInteractive(ui->toolButton->isChecked());
}

TrafficSimulator::~TrafficSimulator()
{
    delete ui;

#if _DEL_SIM_INSTANCE_
      if (_Simulation)
      {
        _Simulation->setFinSimulation(true);
        _Simulation->_TpsSimulation.setHMS(0, 0 ,0);
        _Simulation->_tx.setHMS(0, 0 ,0);
        if (_Simulation->wait())
          _Simulation->deleteInstance();
      }
#endif

}

void TrafficSimulator::OnClock()
{
  if (!_Simulation)
    _Simulation = Simulation::instance();

  if (!_beingRead)
  {
    ui->boutonLectureArret->setIcon(QIcon(_PLAY_BUTTON_));
    return;
  }

  ui->boutonLectureArret->setIcon(QIcon(_STOP_BUTTON_));

  if (_SimRealTime)
     _Simulation->move();

  _Simulation->_ImagesManager.nextImage();
  ui->simulationView->refreshImages();
}

void TrafficSimulator::OnOpenSimulation()
{

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Simulation File"), "", tr("Simulation Files (*.xml)"));
  if (!fileName.isEmpty())
  {
    _Simulation = Simulation::instance();
    _Simulation->erasePlan();
    _Simulation->_Vehicles.clear();

    _Simulation->_TpsSimulation.setHMS(0,0,0);
    _Simulation->_tx.setHMS(0,0,0);

    ui->simulationView->deleteImages(true);
    _Simulation->_ImagesManager.deleteImages();
    QString fileNameSimulation = _Simulation->openXMLFileSignalization(fileName);
    QStringList splitFileName = fileName.split("/");
    splitFileName.removeLast();
    _Simulation->openXMLFileSimulation(splitFileName.join("/") + "/" + fileNameSimulation);
    ui->simulationView->redrawPlan();
    _Simulation->_ImagesManager.addImages();
    ui->simulationView->refreshImages();
    ui->simulationView->renderVelocity(false);
    ui->simulationView->renderFrictions(false);
    ui->simulationView->renderMotorForce(false);
    ui->simulationView->centrer();

    if (!_SimRealTime)
      _Simulation->start();
  }
}

void TrafficSimulator::OnReadStopSimulation()
{
  if (!_Simulation)
      _Simulation = Simulation::instance();

  if (_SimRealTime)
    _Simulation->_ImagesManager.lastImage();
  _beingRead = !_beingRead;
}

void TrafficSimulator::OnZoomAvant()
{
  ui->simulationView->zoom(1);
}

void TrafficSimulator::OnZoomArriere()
{
  ui->simulationView->zoom(0);
}

void TrafficSimulator::OnSettingSpeedSim(int speed)
{
  if (!_Simulation)
    _Simulation = Simulation::instance();

  _Clock->stop();
  switch (speed)
  {
      case 5: _Clock->setInterval(10); break;
      case 4: _Clock->setInterval(20); break;
      case 3: _Clock->setInterval(50); break;
      case 2: _Clock->setInterval(100); break;
      case 1: _Clock->setInterval(1000); break;
  }
  _Clock->start();
  // update console with new speed
  ui->logsBrowser->setText( ui->logsBrowser->document()->toPlainText() + "\n" +
                            QTime::currentTime().toString() +
                            " \t set global speed level = " + QString::number(speed) +
                            " \t interval = " + QString::number(_Clock->interval())
                        );
}

void TrafficSimulator::on_checkBoxIntersecNaming_toggled(bool checked)
{
    for ( QMap<QString, QGraphicsTextItem*>::iterator i = _Markers.begin(); i != _Markers.end(); ++i)
        _Markers[i.key()]->setVisible(checked);
}

void TrafficSimulator::on_checkBoxRenderVelocity_toggled(bool checked)
{
    ui->simulationView->renderVelocity(checked);
    ui->simulationView->hide();
    ui->simulationView->show();
}

void TrafficSimulator::on_checkBoxRenderMotorForce_toggled(bool checked)
{
    ui->simulationView->renderMotorForce(checked);
    ui->simulationView->hide();
    ui->simulationView->show();
}

void TrafficSimulator::on_checkBoxRenderFrottements_toggled(bool checked)
{
    ui->simulationView->renderFrictions(checked);
    ui->simulationView->hide();
    ui->simulationView->show();
}

void TrafficSimulator::onNewButtonpressed()
{    
    debug("New simulation");

    // ending current simulation // TODO
//    if (_Simulation)
//    {
//      _Simulation->setFinSimulation(true);
//      _Simulation->_TpsSimulation.setHMS(0, 0 ,0);
//      _Simulation->_tx.setHMS(0, 0 ,0);
//      //if (_Simulation->wait())
//        //_Simulation->deleteInstance();
//    }

}
