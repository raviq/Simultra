#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <QPointF>

QPointF rev_convert_( double x, double y);


QPointF rev_convert(double x, double y,
                    double minLat, double minLon, double maxLat, double maxLon,
                    double height, double width);

QPointF convert(double lat, double lon,
                double minLat, double maxLat, double minLon, double maxLon,
                double height, double width);


#endif // CONVERSIONS_H
