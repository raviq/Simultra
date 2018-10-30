#ifndef WEATHER_H
#define WEATHER_H

#include <QObject>
#include <QPointer>
#include <QPointF>
#include <vector>
#include "MapGraphics/MapGraphicsScene.h"
#include "weathimage.h"

using namespace std;

class JWeatherManager : public QObject
{
    Q_OBJECT
public:
    explicit JWeatherManager(MapGraphicsScene **scene, QObject *parent = 0);
    ~JWeatherManager();

    void clear();

signals:

public slots:
    void doWeatherUpdate();

private:
    QPointer<MapGraphicsScene> _scene;
    vector<QPointF> _epicenters;
    vector< vector<QString> > _paths;
    int _nclusters;
    unsigned long _counter;
    vector <WeatherImageObject*> _weatherObjects;
    int _scaling, _updateIntervalMS;
    qreal _opacity;
};

#endif // WEATHER_H
