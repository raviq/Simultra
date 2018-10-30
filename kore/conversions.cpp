#include <QPointF>
#include "config.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
// converts (x, y) coordinates into lattitude and longitude (lat, lon).
// If the displacements are not too big (< few kilometers) and we are not right at the poles,
// we can use the quick and dirty estimate that 111.111 meters (111.111 km) in the y direction
// is roughly 1 degree (of latitude) and 111.111 * cos(latitude) meters in the x direction
// is 1 degree (of longitude).
///////////////////////////////////////////////////////////////////////////////////////////////////

// (x, y) -> (lat, lon)
QPointF rev_convert(double x, double y,
                    double minLat, double minLon, double maxLat, double maxLon,
                    double height, double width)
{
    return QPointF((((maxLat - minLat) * x ) / (height - 1) ) + minLat,
                   (((maxLon - minLon) * y ) / (width - 1) ) + minLon);
}


// (lat, lon) -> (x, y)
QPointF convert(double lat,    double lon,
                double minLat, double maxLat, double minLon, double maxLon,
                double height, double width)
{
    return QPointF(((lat - minLat) / (maxLat - minLat)) * (height - 1),
                   ((lon - minLon) / (maxLon - minLon)) * (width - 1) );
}
