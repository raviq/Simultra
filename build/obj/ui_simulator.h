/********************************************************************************
** Form generated from reading UI file 'simulator.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMULATOR_H
#define UI_SIMULATOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "gui/simulationgui.h"

QT_BEGIN_NAMESPACE

class Ui_TrafficSimulator
{
public:
    QAction *openActionSimulationFile;
    QAction *actionAbout;
    QAction *actionTestt;
    QWidget *widget;
    QPushButton *boutonLectureArret;
    QGroupBox *groupBox;
    QLabel *label_2;
    QSlider *reglageVitesse;
    QLabel *label_3;
    QSlider *reglageVitesse_2;
    QLCDNumber *lcdTimer;
    QTextBrowser *agentBrowser;
    QTabWidget *tabParamWidget;
    QWidget *tab_3;
    QTextBrowser *paramBrowser;
    QWidget *tab;
    QTextBrowser *lanesBrowser;
    QWidget *tab_2;
    QTextBrowser *logsBrowser;
    QWidget *tab_5;
    QTextBrowser *detectBrowser;
    QWidget *tab_4;
    QTextBrowser *debugBrowser;
    QPushButton *newButton;
    QToolButton *toolButton;
    QTabWidget *tabWidget;
    QWidget *tab_6;
    QWidget *OSMWidget;
    QWidget *tab_7;
    SimulationGUI *simulationView;
    QPushButton *aboutPushButton;
    QPushButton *weatherPushButton;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_3;
    QGroupBox *groupBox_3;
    QCheckBox *checkBoxRenderVelocity;
    QCheckBox *checkBoxRenderMotorForce;
    QCheckBox *checkBoxRenderFrottements;
    QGroupBox *groupBox_2;
    QCheckBox *checkBoxIntersecNaming;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;
    QLabel *mouseCoords;
    QPushButton *walkersPushButton;
    QPushButton *goPushButton;
    QPushButton *pushButton;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QSpinBox *spinBox;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_8;
    QPushButton *boutonZoomAvant;
    QPushButton *boutonZoomArriere;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_10;
    QHBoxLayout *horizontalLayout_10;
    QPushButton *decreasesizeCarButton;
    QPushButton *increasesizeCarButton;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_9;
    QLabel *label_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBoxLat;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBoxLng;
    QMenuBar *menuBar;
    QMenu *fileMenu;

    void setupUi(QMainWindow *TrafficSimulator)
    {
        if (TrafficSimulator->objectName().isEmpty())
            TrafficSimulator->setObjectName(QStringLiteral("TrafficSimulator"));
        TrafficSimulator->resize(1460, 869);
        TrafficSimulator->setStyleSheet(QStringLiteral(""));
        openActionSimulationFile = new QAction(TrafficSimulator);
        openActionSimulationFile->setObjectName(QStringLiteral("openActionSimulationFile"));
        actionAbout = new QAction(TrafficSimulator);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setCheckable(false);
        actionTestt = new QAction(TrafficSimulator);
        actionTestt->setObjectName(QStringLiteral("actionTestt"));
        widget = new QWidget(TrafficSimulator);
        widget->setObjectName(QStringLiteral("widget"));
        boutonLectureArret = new QPushButton(widget);
        boutonLectureArret->setObjectName(QStringLiteral("boutonLectureArret"));
        boutonLectureArret->setGeometry(QRect(30, 10, 31, 31));
        boutonLectureArret->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/play.png"), QSize(), QIcon::Normal, QIcon::Off);
        boutonLectureArret->setIcon(icon);
        boutonLectureArret->setAutoRepeat(false);
        boutonLectureArret->setAutoExclusive(false);
        boutonLectureArret->setAutoDefault(false);
        boutonLectureArret->setFlat(false);
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(1240, 600, 191, 81));
        groupBox->setStyleSheet(QStringLiteral(""));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 30, 51, 16));
        reglageVitesse = new QSlider(groupBox);
        reglageVitesse->setObjectName(QStringLiteral("reglageVitesse"));
        reglageVitesse->setGeometry(QRect(70, 30, 81, 21));
        reglageVitesse->setMinimum(1);
        reglageVitesse->setMaximum(5);
        reglageVitesse->setPageStep(1);
        reglageVitesse->setValue(3);
        reglageVitesse->setSliderPosition(3);
        reglageVitesse->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 50, 51, 16));
        reglageVitesse_2 = new QSlider(groupBox);
        reglageVitesse_2->setObjectName(QStringLiteral("reglageVitesse_2"));
        reglageVitesse_2->setGeometry(QRect(70, 50, 81, 21));
        reglageVitesse_2->setMinimum(1);
        reglageVitesse_2->setMaximum(5);
        reglageVitesse_2->setPageStep(1);
        reglageVitesse_2->setValue(3);
        reglageVitesse_2->setSliderPosition(3);
        reglageVitesse_2->setOrientation(Qt::Horizontal);
        lcdTimer = new QLCDNumber(widget);
        lcdTimer->setObjectName(QStringLiteral("lcdTimer"));
        lcdTimer->setGeometry(QRect(130, 10, 101, 31));
        lcdTimer->setFrameShape(QFrame::Box);
        lcdTimer->setLineWidth(1);
        lcdTimer->setMidLineWidth(0);
        lcdTimer->setSmallDecimalPoint(false);
        lcdTimer->setDigitCount(9);
        agentBrowser = new QTextBrowser(widget);
        agentBrowser->setObjectName(QStringLiteral("agentBrowser"));
        agentBrowser->setGeometry(QRect(1230, 700, 201, 111));
        agentBrowser->setStyleSheet(QStringLiteral(""));
        agentBrowser->setFrameShape(QFrame::Panel);
        agentBrowser->setFrameShadow(QFrame::Sunken);
        agentBrowser->setLineWidth(0);
        tabParamWidget = new QTabWidget(widget);
        tabParamWidget->setObjectName(QStringLiteral("tabParamWidget"));
        tabParamWidget->setGeometry(QRect(10, 690, 1211, 151));
        tabParamWidget->setStyleSheet(QStringLiteral("   background-color: transparent ;"));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        paramBrowser = new QTextBrowser(tab_3);
        paramBrowser->setObjectName(QStringLiteral("paramBrowser"));
        paramBrowser->setGeometry(QRect(0, 0, 1201, 141));
        paramBrowser->setStyleSheet(QStringLiteral(""));
        paramBrowser->setFrameShape(QFrame::NoFrame);
        paramBrowser->setLineWidth(0);
        tabParamWidget->addTab(tab_3, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        lanesBrowser = new QTextBrowser(tab);
        lanesBrowser->setObjectName(QStringLiteral("lanesBrowser"));
        lanesBrowser->setGeometry(QRect(0, 0, 1201, 141));
        lanesBrowser->setStyleSheet(QStringLiteral(""));
        lanesBrowser->setFrameShape(QFrame::Panel);
        lanesBrowser->setLineWidth(0);
        tabParamWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        logsBrowser = new QTextBrowser(tab_2);
        logsBrowser->setObjectName(QStringLiteral("logsBrowser"));
        logsBrowser->setGeometry(QRect(0, 0, 1201, 141));
        logsBrowser->setAutoFillBackground(false);
        logsBrowser->setStyleSheet(QStringLiteral(""));
        logsBrowser->setFrameShape(QFrame::Panel);
        logsBrowser->setFrameShadow(QFrame::Sunken);
        logsBrowser->setLineWidth(0);
        logsBrowser->setHtml(QLatin1String("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.SF NS Text'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'.Helvetica Neue DeskInterface';\"><br /></p></body></html>"));
        tabParamWidget->addTab(tab_2, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        detectBrowser = new QTextBrowser(tab_5);
        detectBrowser->setObjectName(QStringLiteral("detectBrowser"));
        detectBrowser->setGeometry(QRect(0, 0, 1201, 151));
        detectBrowser->setStyleSheet(QStringLiteral(""));
        detectBrowser->setFrameShape(QFrame::Panel);
        detectBrowser->setFrameShadow(QFrame::Plain);
        detectBrowser->setLineWidth(0);
        tabParamWidget->addTab(tab_5, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        debugBrowser = new QTextBrowser(tab_4);
        debugBrowser->setObjectName(QStringLiteral("debugBrowser"));
        debugBrowser->setGeometry(QRect(0, 0, 1201, 141));
        debugBrowser->setStyleSheet(QStringLiteral(""));
        debugBrowser->setFrameShape(QFrame::Panel);
        debugBrowser->setLineWidth(0);
        tabParamWidget->addTab(tab_4, QString());
        newButton = new QPushButton(widget);
        newButton->setObjectName(QStringLiteral("newButton"));
        newButton->setGeometry(QRect(60, 10, 31, 31));
        newButton->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/new.png"), QSize(), QIcon::Normal, QIcon::Off);
        newButton->setIcon(icon1);
        newButton->setAutoRepeat(false);
        newButton->setAutoExclusive(false);
        newButton->setAutoDefault(false);
        newButton->setFlat(false);
        toolButton = new QToolButton(widget);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setGeometry(QRect(1330, 10, 31, 31));
        toolButton->setStyleSheet(QStringLiteral(""));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/drag-icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon2);
        toolButton->setIconSize(QSize(20, 20));
        tabWidget = new QTabWidget(widget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(10, 30, 1221, 661));
        tabWidget->setStyleSheet(QStringLiteral("   background-color: transparent;"));
        tabWidget->setTabShape(QTabWidget::Triangular);
        tab_6 = new QWidget();
        tab_6->setObjectName(QStringLiteral("tab_6"));
        OSMWidget = new QWidget(tab_6);
        OSMWidget->setObjectName(QStringLiteral("OSMWidget"));
        OSMWidget->setGeometry(QRect(0, 0, 1231, 631));
        OSMWidget->setStyleSheet(QStringLiteral("background: gray;"));
        tabWidget->addTab(tab_6, QString());
        tab_7 = new QWidget();
        tab_7->setObjectName(QStringLiteral("tab_7"));
        simulationView = new SimulationGUI(tab_7);
        simulationView->setObjectName(QStringLiteral("simulationView"));
        simulationView->setGeometry(QRect(10, 0, 1211, 631));
        simulationView->setStyleSheet(QStringLiteral("background-color: #E5E5E5;"));
        QBrush brush(QColor(0, 0, 0, 66));
        brush.setStyle(Qt::SolidPattern);
        simulationView->setProperty("backgroundBrush", QVariant(brush));
        QBrush brush1(QColor(0, 0, 0, 255));
        brush1.setStyle(Qt::NoBrush);
        simulationView->setProperty("foregroundBrush", QVariant(brush1));
        simulationView->setProperty("sceneRect", QVariant(QRectF(0, 0, 0, 300000)));
        tabWidget->addTab(tab_7, QString());
        aboutPushButton = new QPushButton(widget);
        aboutPushButton->setObjectName(QStringLiteral("aboutPushButton"));
        aboutPushButton->setGeometry(QRect(1410, 10, 31, 31));
        aboutPushButton->setStyleSheet(QStringLiteral(""));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/QuestionMark.png"), QSize(), QIcon::Normal, QIcon::Off);
        aboutPushButton->setIcon(icon3);
        aboutPushButton->setIconSize(QSize(24, 24));
        weatherPushButton = new QPushButton(widget);
        weatherPushButton->setObjectName(QStringLiteral("weatherPushButton"));
        weatherPushButton->setGeometry(QRect(1370, 10, 31, 31));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/weather4.png"), QSize(), QIcon::Normal, QIcon::Off);
        weatherPushButton->setIcon(icon4);
        weatherPushButton->setIconSize(QSize(24, 24));
        dockWidget = new QDockWidget(widget);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setGeometry(QRect(1230, 150, 211, 231));
        dockWidget->setAutoFillBackground(false);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        dockWidget->setWidget(dockWidgetContents_3);
        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(1240, 500, 191, 101));
        checkBoxRenderVelocity = new QCheckBox(groupBox_3);
        checkBoxRenderVelocity->setObjectName(QStringLiteral("checkBoxRenderVelocity"));
        checkBoxRenderVelocity->setGeometry(QRect(10, 30, 121, 20));
        checkBoxRenderVelocity->setStyleSheet(QStringLiteral(""));
        checkBoxRenderVelocity->setChecked(false);
        checkBoxRenderMotorForce = new QCheckBox(groupBox_3);
        checkBoxRenderMotorForce->setObjectName(QStringLiteral("checkBoxRenderMotorForce"));
        checkBoxRenderMotorForce->setGeometry(QRect(10, 50, 141, 20));
        checkBoxRenderMotorForce->setStyleSheet(QStringLiteral(""));
        checkBoxRenderMotorForce->setChecked(false);
        checkBoxRenderFrottements = new QCheckBox(groupBox_3);
        checkBoxRenderFrottements->setObjectName(QStringLiteral("checkBoxRenderFrottements"));
        checkBoxRenderFrottements->setGeometry(QRect(10, 70, 151, 20));
        checkBoxRenderFrottements->setStyleSheet(QStringLiteral(""));
        checkBoxRenderFrottements->setChecked(false);
        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(1240, 380, 191, 121));
        groupBox_2->setAutoFillBackground(false);
        groupBox_2->setStyleSheet(QStringLiteral(""));
        checkBoxIntersecNaming = new QCheckBox(groupBox_2);
        checkBoxIntersecNaming->setObjectName(QStringLiteral("checkBoxIntersecNaming"));
        checkBoxIntersecNaming->setGeometry(QRect(10, 90, 111, 20));
        checkBoxIntersecNaming->setChecked(false);
        checkBox = new QCheckBox(groupBox_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setEnabled(true);
        checkBox->setGeometry(QRect(10, 30, 81, 20));
        checkBox_2 = new QCheckBox(groupBox_2);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(110, 30, 89, 20));
        checkBox_2->setAutoFillBackground(false);
        checkBox_2->setStyleSheet(QStringLiteral(""));
        checkBox_4 = new QCheckBox(groupBox_2);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setGeometry(QRect(10, 50, 61, 20));
        checkBox_5 = new QCheckBox(groupBox_2);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setGeometry(QRect(10, 70, 51, 20));
        checkBox_3 = new QCheckBox(groupBox_2);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setGeometry(QRect(110, 50, 71, 20));
        checkBox_6 = new QCheckBox(groupBox_2);
        checkBox_6->setObjectName(QStringLiteral("checkBox_6"));
        checkBox_6->setGeometry(QRect(110, 70, 71, 20));
        checkBox_7 = new QCheckBox(groupBox_2);
        checkBox_7->setObjectName(QStringLiteral("checkBox_7"));
        checkBox_7->setGeometry(QRect(110, 90, 89, 20));
        mouseCoords = new QLabel(widget);
        mouseCoords->setObjectName(QStringLiteral("mouseCoords"));
        mouseCoords->setGeometry(QRect(1230, 820, 201, 21));
        walkersPushButton = new QPushButton(widget);
        walkersPushButton->setObjectName(QStringLiteral("walkersPushButton"));
        walkersPushButton->setGeometry(QRect(1290, 10, 31, 31));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/walker.png"), QSize(), QIcon::Normal, QIcon::Off);
        walkersPushButton->setIcon(icon5);
        walkersPushButton->setIconSize(QSize(16, 16));
        goPushButton = new QPushButton(widget);
        goPushButton->setObjectName(QStringLiteral("goPushButton"));
        goPushButton->setGeometry(QRect(1250, 10, 31, 31));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/images/jump.png"), QSize(), QIcon::Normal, QIcon::Off);
        goPushButton->setIcon(icon6);
        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(90, 10, 31, 31));
        pushButton->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/images/rec.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon7);
        horizontalLayoutWidget = new QWidget(widget);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(820, 10, 430, 38));
        horizontalLayout_6 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        label_6 = new QLabel(horizontalLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_7->addWidget(label_6);

        spinBox = new QSpinBox(horizontalLayoutWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setStyleSheet(QStringLiteral("border: none;"));

        horizontalLayout_7->addWidget(spinBox);


        horizontalLayout->addLayout(horizontalLayout_7);

        label_7 = new QLabel(horizontalLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout->addWidget(label_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        boutonZoomAvant = new QPushButton(horizontalLayoutWidget);
        boutonZoomAvant->setObjectName(QStringLiteral("boutonZoomAvant"));
        boutonZoomAvant->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/images/zoomin_.ico"), QSize(), QIcon::Normal, QIcon::Off);
        boutonZoomAvant->setIcon(icon8);
        boutonZoomAvant->setIconSize(QSize(20, 20));

        horizontalLayout_8->addWidget(boutonZoomAvant);

        boutonZoomArriere = new QPushButton(horizontalLayoutWidget);
        boutonZoomArriere->setObjectName(QStringLiteral("boutonZoomArriere"));
        boutonZoomArriere->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/images/zoomout_.ico"), QSize(), QIcon::Normal, QIcon::Off);
        boutonZoomArriere->setIcon(icon9);
        boutonZoomArriere->setIconSize(QSize(20, 20));

        horizontalLayout_8->addWidget(boutonZoomArriere);


        horizontalLayout->addLayout(horizontalLayout_8);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_10 = new QLabel(horizontalLayoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_4->addWidget(label_10);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        decreasesizeCarButton = new QPushButton(horizontalLayoutWidget);
        decreasesizeCarButton->setObjectName(QStringLiteral("decreasesizeCarButton"));
        decreasesizeCarButton->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/images/vehicles_img/increasesize/minus.png"), QSize(), QIcon::Normal, QIcon::Off);
        decreasesizeCarButton->setIcon(icon10);
        decreasesizeCarButton->setIconSize(QSize(30, 30));

        horizontalLayout_10->addWidget(decreasesizeCarButton);

        increasesizeCarButton = new QPushButton(horizontalLayoutWidget);
        increasesizeCarButton->setObjectName(QStringLiteral("increasesizeCarButton"));
        increasesizeCarButton->setStyleSheet(QStringLiteral("border: none;"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/images/vehicles_img/increasesize/plus.png"), QSize(), QIcon::Normal, QIcon::Off);
        increasesizeCarButton->setIcon(icon11);
        increasesizeCarButton->setIconSize(QSize(30, 30));

        horizontalLayout_10->addWidget(increasesizeCarButton);


        horizontalLayout_4->addLayout(horizontalLayout_10);


        horizontalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_6->addLayout(horizontalLayout);

        verticalLayoutWidget = new QWidget(widget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(1240, 60, 203, 81));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_9 = new QLabel(verticalLayoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_5->addWidget(label_9);

        label_5 = new QLabel(verticalLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setStyleSheet(QStringLiteral("border:none;"));

        horizontalLayout_5->addWidget(label_5);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        doubleSpinBoxLat = new QDoubleSpinBox(verticalLayoutWidget);
        doubleSpinBoxLat->setObjectName(QStringLiteral("doubleSpinBoxLat"));
        doubleSpinBoxLat->setStyleSheet(QStringLiteral("border: none;"));
        doubleSpinBoxLat->setDecimals(10);
        doubleSpinBoxLat->setMinimum(1);
        doubleSpinBoxLat->setMaximum(200);
        doubleSpinBoxLat->setSingleStep(0.001);
        doubleSpinBoxLat->setValue(136.901);

        horizontalLayout_2->addWidget(doubleSpinBoxLat);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setStyleSheet(QStringLiteral("border:none;"));

        horizontalLayout_3->addWidget(label_4);

        doubleSpinBoxLng = new QDoubleSpinBox(verticalLayoutWidget);
        doubleSpinBoxLng->setObjectName(QStringLiteral("doubleSpinBoxLng"));
        doubleSpinBoxLng->setStyleSheet(QStringLiteral("border: none;"));
        doubleSpinBoxLng->setDecimals(10);
        doubleSpinBoxLng->setMinimum(1);
        doubleSpinBoxLng->setMaximum(200);
        doubleSpinBoxLng->setSingleStep(0.001);
        doubleSpinBoxLng->setValue(35.1696);

        horizontalLayout_3->addWidget(doubleSpinBoxLng);


        verticalLayout->addLayout(horizontalLayout_3);

        TrafficSimulator->setCentralWidget(widget);
        groupBox->raise();
        lcdTimer->raise();
        agentBrowser->raise();
        tabParamWidget->raise();
        newButton->raise();
        toolButton->raise();
        aboutPushButton->raise();
        tabWidget->raise();
        weatherPushButton->raise();
        dockWidget->raise();
        groupBox_3->raise();
        groupBox_2->raise();
        mouseCoords->raise();
        walkersPushButton->raise();
        goPushButton->raise();
        pushButton->raise();
        horizontalLayoutWidget->raise();
        verticalLayoutWidget->raise();
        boutonLectureArret->raise();
        menuBar = new QMenuBar(TrafficSimulator);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1460, 22));
        fileMenu = new QMenu(menuBar);
        fileMenu->setObjectName(QStringLiteral("fileMenu"));
        TrafficSimulator->setMenuBar(menuBar);

        menuBar->addAction(fileMenu->menuAction());
        fileMenu->addAction(openActionSimulationFile);

        retranslateUi(TrafficSimulator);
        QObject::connect(boutonLectureArret, SIGNAL(pressed()), TrafficSimulator, SLOT(OnReadStopSimulation()));
        QObject::connect(reglageVitesse, SIGNAL(valueChanged(int)), TrafficSimulator, SLOT(OnSettingSpeedSim(int)));
        QObject::connect(checkBoxRenderVelocity, SIGNAL(toggled(bool)), TrafficSimulator, SLOT(OnSpeedView(bool)));
        QObject::connect(openActionSimulationFile, SIGNAL(activated()), TrafficSimulator, SLOT(OnOpenSimulation()));
        QObject::connect(checkBoxRenderMotorForce, SIGNAL(toggled(bool)), TrafficSimulator, SLOT(OnRenderMotorForce(bool)));
        QObject::connect(checkBoxRenderFrottements, SIGNAL(toggled(bool)), TrafficSimulator, SLOT(OnRenderFrottements(bool)));
        QObject::connect(boutonZoomArriere, SIGNAL(pressed()), TrafficSimulator, SLOT(OnZoomArriere()));
        QObject::connect(boutonZoomAvant, SIGNAL(pressed()), TrafficSimulator, SLOT(OnZoomAvant()));

        boutonLectureArret->setDefault(false);
        tabParamWidget->setCurrentIndex(2);
        newButton->setDefault(false);
        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TrafficSimulator);
    } // setupUi

    void retranslateUi(QMainWindow *TrafficSimulator)
    {
        TrafficSimulator->setWindowTitle(QApplication::translate("TrafficSimulator", "Simultra", nullptr));
        openActionSimulationFile->setText(QApplication::translate("TrafficSimulator", "Open Plan Files", nullptr));
        actionAbout->setText(QApplication::translate("TrafficSimulator", "About", nullptr));
        actionTestt->setText(QApplication::translate("TrafficSimulator", "Testt", nullptr));
        boutonLectureArret->setText(QString());
        groupBox->setTitle(QApplication::translate("TrafficSimulator", "Macroscopic Flow Parameters", nullptr));
        label_2->setText(QApplication::translate("TrafficSimulator", "Velocity", nullptr));
        label_3->setText(QApplication::translate("TrafficSimulator", "Dst", nullptr));
        tabParamWidget->setTabText(tabParamWidget->indexOf(tab_3), QApplication::translate("TrafficSimulator", "Parameters", nullptr));
        tabParamWidget->setTabText(tabParamWidget->indexOf(tab), QApplication::translate("TrafficSimulator", "Lanes", nullptr));
        tabParamWidget->setTabText(tabParamWidget->indexOf(tab_2), QApplication::translate("TrafficSimulator", "Vehicles", nullptr));
        tabParamWidget->setTabText(tabParamWidget->indexOf(tab_5), QApplication::translate("TrafficSimulator", "Detectors", nullptr));
        tabParamWidget->setTabText(tabParamWidget->indexOf(tab_4), QApplication::translate("TrafficSimulator", "Debug", nullptr));
        newButton->setText(QString());
        toolButton->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_6), QApplication::translate("TrafficSimulator", "     Simulation     ", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_7), QApplication::translate("TrafficSimulator", "     Physics Engine     ", nullptr));
        aboutPushButton->setText(QString());
        weatherPushButton->setText(QString());
        dockWidget->setWindowTitle(QApplication::translate("TrafficSimulator", "Layers", nullptr));
        groupBox_3->setTitle(QApplication::translate("TrafficSimulator", "Forces View", nullptr));
        checkBoxRenderVelocity->setText(QApplication::translate("TrafficSimulator", "Velocity Vector", nullptr));
        checkBoxRenderMotorForce->setText(QApplication::translate("TrafficSimulator", "Motive Force Norm", nullptr));
        checkBoxRenderFrottements->setText(QApplication::translate("TrafficSimulator", "Friction Force Norm", nullptr));
        groupBox_2->setTitle(QApplication::translate("TrafficSimulator", "Components View", nullptr));
        checkBoxIntersecNaming->setText(QApplication::translate("TrafficSimulator", "Intersect", nullptr));
        checkBox->setText(QApplication::translate("TrafficSimulator", "Protocol", nullptr));
        checkBox_2->setText(QApplication::translate("TrafficSimulator", "Agent", nullptr));
        checkBox_4->setText(QApplication::translate("TrafficSimulator", "Synch", nullptr));
        checkBox_5->setText(QApplication::translate("TrafficSimulator", "Map", nullptr));
        checkBox_3->setText(QApplication::translate("TrafficSimulator", "Sensor", nullptr));
        checkBox_6->setText(QApplication::translate("TrafficSimulator", "Routes", nullptr));
        checkBox_7->setText(QApplication::translate("TrafficSimulator", "Walkway", nullptr));
        mouseCoords->setText(QString());
        walkersPushButton->setText(QString());
        goPushButton->setText(QString());
        pushButton->setText(QString());
        label_6->setText(QApplication::translate("TrafficSimulator", "Online Map", nullptr));
        label_7->setText(QApplication::translate("TrafficSimulator", "Offline Map", nullptr));
        boutonZoomAvant->setText(QString());
        boutonZoomArriere->setText(QString());
        label_10->setText(QApplication::translate("TrafficSimulator", "Vehicles", nullptr));
        decreasesizeCarButton->setText(QString());
        increasesizeCarButton->setText(QString());
        label_9->setText(QApplication::translate("TrafficSimulator", "Geoloc:", nullptr));
        label_5->setText(QApplication::translate("TrafficSimulator", "Nagoya_sim", nullptr));
        label->setText(QApplication::translate("TrafficSimulator", "Latitude", nullptr));
        label_4->setText(QApplication::translate("TrafficSimulator", "Longitude", nullptr));
        fileMenu->setTitle(QApplication::translate("TrafficSimulator", "File", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TrafficSimulator: public Ui_TrafficSimulator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMULATOR_H
