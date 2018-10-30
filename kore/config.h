//
// Configuration file
//

#ifndef _CONFIG_
#define _CONFIG_

// General
#define     _TRACE_              false
#define     _LOGO_               ":/images/ico.png"
#define     _PLAY_BUTTON_        ":/images/play.png"
#define     _STOP_BUTTON_        ":/images/stop.png"
#define     _CHANGELOG_          ":/CHANGELOG"
#define     _SIMULTRA_VERSION_   "1.1"
#define     OSM_VERSION_STR      "0.6"
#define     X_VERSION_STR        "1"

// Simulation configuration TODO JSONify

// _OSM_MAP_ saved from https://www.openstreetmap.org/export#map=18/35.17126/136.90116&layers=C
#define     _OSM_MAP_             "resources/config/map.osm"
#define     _SIMULATION_TYPES_    ":/config/types.xml"
#define     _SIMULATION_MAP_      ":/config/simx_map.xml"
#define     _SIMULATION_SCENARIO_ ":/config/simx_simulation.xml"
#define     _QSS_                 ":/default.qss"
// Center in: http://www.openstreetmap.org/way/200245313#map=18/35.16899/136.90054
#define     _CENTER_IN_X_          136.90082390000001
#define     _CENTER_IN_Y_          35.17036439999999
#define     _INITIAL_ZOOM_         17

// Physics
#define     UNIT_PER_METRE      1.0000f
#define     METRE_PER_UNIT      1.0/UNIT_PER_METRE
#define     LIGHT_DISTANCE      3.0
#define     MPS_A_KMH           3.6f
#define     KMH_A_MPS           1/MPS_A_KMH
#define     _COLLISION_DISTANCE_ 10.0f

// TODO Calculate
#define _MINLAT_  35.1657
#define _MINLON_  136.893
#define _MAXLAT_  35.1734
#define _MAXLON_  136.909

// Traffic

#define     _DEL_SIM_INSTANCE_   false
#define     _OSMMAPGRAPHICS_     false

#define     _MIN_ZOOMING_        1
#define     _MAX_ZOOMING_        25
#define     _ZOOM_FACTOR_BASE_   1.0015
#define     _ZOOM_DELTA_THRESH_  5

// Simulation
#define     _MOVE_STEP_          1000
#define     _MAX_SIMULATION_DURATION_   10
#define     _MAX_DISTANCE_TO_COLLISION_ 150.0
#define     _CLOCK_INTERVAL_     50

// Environment
#define     HEIGHT                  900
#define     WIDTH                   900
#define     LANE_WIDTH              2.0

#define     BUILDING_LINE_THICKNESS 0.1
#define     BUILDING_COLOR          Qt::darkGray
#define     BUILDING_LINE_COLOR     Qt::blue
#define     DRAW_BUILDINGS          false

#define     PARKING_COLOR           Qt::darkCyan
#define     PARKING_LINE_COLOR      Qt::darkMagenta
#define     PARKING_LINE_THICKNESS  0.1

#define     HIGHWAY_COLOR           Qt::yellow
#define     HIGHWAY_LINE_COLOR      Qt::blue
#define     HIGHWAY_LINE_THICKNESS  0.3

#define     ROUTE_NAMES             true
#define     ROUTE_NAME_COLOR        Qt::darkRed
#define     ROUTE_NAME_SIZE         1
#define     PARTICLE_TRACKS         false
#define     RENDER_BUILDINGS_NODES  false
#define     RENDER_NODES            false  // includes nodes around buildings, but needs to be refined !
#define     HIGHWAY_POINTS          false
#define     DRAW_LINES              true

// Vehicles
#define     _MAX_VEHICLES_          40
#define     _WHEELBASE_             3.5f
#define     _INITIAL_SCALE_         0.1    // Controls the size of the vehicle(s)
#define     _GREEN_LIGHT_ICON_      ":/images/green_light.png"
#define     _RED_LIGHT_ICON_        ":/images/red_light.png"
#define     _ORANGE_LIGHT_ICON_     ":/images/orange_light.png"

// Walker's epsilon
#define     MIN_EPSILON             0.01000
#define     MAX_EPSILON             0.01002
#define     RENDER_WALKERS          false

// Weather data
#define     _WEATHER_DATA_          ":/RadarImages/Japan/"
#define     _WEATHER_CLUSTER_DIR_   "ABC/"
#define     _W_SCALING_             3750000
#define     _W_UPDATE_INTERVAL_MS_  500
#define     _W_OPACITY_             0.7

// Driver
#define     _SHORTEST_DURATION_     90000.0

// Detectors
#define     _DETECTORS_             false
#define     _DETECOR_ICON_          ":/images/detector.png"

// Logged data
#define     _VEHICLES_CSV_          "/vehicles.csv"
#define     _FLOW_CSV_              "/flow.csv"
#define     _DETECTIONS_CSV_        "/detections.csv"

// Distributed version
#define     _DISTRIBUTED_           false
#if _DISTRIBUTED_
#define     MANAGER_IP              "127.0.0.1"
#define     MANAGER_PORT             8033
#endif


#endif
