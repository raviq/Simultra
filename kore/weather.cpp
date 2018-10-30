//
// Weather manager
//

#include <QtDebug>
#include <QRegExp>
#include <QTimer>
#include <QTime>
#include <QDir>
#include <iostream>

#include "MapGraphics/guts/Conversions.h"
#include "MapGraphics/CircleObject.h"
#include "weathimage.h"
#include "weather.h"
#include "config.h"
#include "debug.h"

using namespace std;

JWeatherManager::JWeatherManager(MapGraphicsScene **scene, QObject *parent): QObject(parent), _scene(*scene), _counter(0)
{
    // Setting radar images
    QString root(_WEATHER_DATA_);
    QString Clusters[] = {_WEATHER_CLUSTER_DIR_};

    _nclusters = sizeof(Clusters)/sizeof(QString); // Number of weather clusters
    _paths.resize(_nclusters);

    for (int i = 0 ; i < _nclusters ; i++)
    {
        debug(" Cluster #%d.", i);

        QString path(root + Clusters[i]);
        QDir dir(path);
        dir.setNameFilters(QStringList() << "*.gif");
        dir.setSorting( QDir::Name);

        // gif images.
        QFileInfoList list = dir.entryInfoList();
        _paths[i].reserve(list.size());
        for (int j = 0; j < list.size(); ++j)
        {
            QFileInfo fileinfo = list.at(j);
            _paths[i].push_back(path + fileinfo.fileName());
        }
    }

    // Set epicenters, randomly set, around Japan
    _epicenters.resize(_nclusters);

    QPointF Epicenters[] =
    {

    //   QPointF(140.7500, 40.8167), // Aomori
         QPointF(135.1831, 34.6908)  // Hyogo
    //   QPointF(140.7500, 40.8167), // Aomori
    //   QPointF(136.9000, 35.1833), // Nagoya
    //   QPointF(136.7667, 35.4167),
    //   QPointF(136.90332, 35.175),
    //   QPointF(137.2667, 34.6667),
    //   QPointF(140.8667, 38.2667),
    //   QPointF(142.0000, 43.0000), // Hokkaido
    //   QPointF(136.6500, 36.5667),
    //   QPointF(133.0500, 35.4667),
    //   QPointF(136.1000, 34.6333),
    //   QPointF(135.5022, 34.6939),
    //   QPointF(134.5500, 34.0667),
    //   QPointF(130.5500, 31.6000),
    //   QPointF(130.7333, 32.7833)

    };

    _scaling = _W_SCALING_;
    _updateIntervalMS = _W_UPDATE_INTERVAL_MS_;
    _opacity = _W_OPACITY_;

    for (int k = 0 ; k < _nclusters ; k++)
    {
        // set epicenter
        _epicenters[k] = Epicenters[k];
    }

    // set signals
    QTimer * timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(doWeatherUpdate()));
    timer->start(_updateIntervalMS);
    QTimer::singleShot(0, this, SLOT(doWeatherUpdate()));
}

JWeatherManager::~JWeatherManager()
{
}

// Public slot
void JWeatherManager::doWeatherUpdate()
{
    debug( "Updating Weather Object.");

    // Update counter

    _counter = (_counter == _paths[0].size() - 1) ? 0 : _counter+1 ;
    _weatherObjects.resize(_nclusters);

    for (int k = 0 ; k < _nclusters ; k++)
    {
        // Remove previous weather object
        if (_weatherObjects[k])
        {
            _scene->removeObject(_weatherObjects[k]);
            delete _weatherObjects[k];
        }

        // init objects
        _weatherObjects[k] = new WeatherImageObject(QImage(), QRectF(0, 0, _scaling, _scaling));

        QImage weatherImage(_paths[k].at(_counter));

        // specific to otenki data, need to transform gif
        QTransform transform;
        transform.rotate(180);
        qreal m11 = transform.m11();    // Horizontal scaling
        qreal m12 = transform.m12();    // Vertical shearing
        qreal m13 = transform.m13();    // Horizontal Projection
        qreal m21 = transform.m21();    // Horizontal shearing
        qreal m22 = transform.m22();    // vertical scaling
        qreal m23 = transform.m23();    // Vertical Projection
        qreal m31 = transform.m31();    // Horizontal Position (DX)
        qreal m32 = transform.m32();    // Vertical Position (DY)
        qreal m33 = transform.m33();    // Addtional Projection Factor
        // Horizontal flip
        m11 = -m11;
        transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);

        weatherImage = weatherImage.transformed(transform);

        _weatherObjects[k]->setWeather(weatherImage,
                                       QRectF(0, 0, _scaling, _scaling),
                                       _epicenters[k]);
        _weatherObjects[k]->setOpacity(_opacity);
        _scene->addObject(_weatherObjects[k]);
    }
}

void JWeatherManager::clear()
{
    for (int k = 0 ; k < _nclusters ; k++)
    {
        // remove previous weather object
        if (_weatherObjects[k])
        {
            _scene->removeObject(_weatherObjects[k]);
            delete _weatherObjects[k];
        }
    }
}
// End.
