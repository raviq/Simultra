############################################################################
# Simultra's qmake file.
# Command: qmake simultra.pro
# Rafik Hadfi <rafik.hadfi@gmail.com>
############################################################################

QT += core xml testlib widgets gui

TARGET = simultra
TEMPLATE = app

INCLUDEPATH += utils gui gui/images kore

# Readosm
LIBS += -L/usr/local/lib -lreadosm
INCLUDEPATH += /usr/local/include/

# MapGraphics
LIBS += -L$$PWD/MapGraphics/ -lMapGraphics.1.0.0
INCLUDEPATH += $$PWD/MapGraphics/

# Build
OBJECTS_DIR = $$PWD/build/obj # Intermediate object files directory
MOC_DIR = $$PWD/build/obj     # Intermediate moc files directory
UI_DIR  = $$PWD/build/obj     # UI files
RCC_DIR = $$PWD/build/obj     # Resources
DESTDIR = $$PWD/build         # Target file directory

SOURCES += kore/main.cpp \
           utils/point.cpp \
           utils/utils.cpp \
           utils/segment.cpp \
           kore/plan.cpp \
           kore/simulation.cpp \
           gui/images/image.cpp \
           gui/images/imagesignalisation.cpp \
           kore/driver.cpp \
           kore/vehicle.cpp \
           gui/images/imagevehicle.cpp \
           utils/log.cpp \
           kore/graph.cpp \
           gui/rarrow.cpp \
           kore/detector.cpp \
           kore/world.cpp \
           kore/parser.cpp \
           gui/simulationgui.cpp \
           gui/roadgui.cpp \
           kore/walker.cpp \
           gui/intersectiongui.cpp \
           kore/node.cpp \
           kore/edge.cpp \
           gui/pedestriangui.cpp \
           gui/vehimob.cpp \
           gui/aboutdialog.cpp \
           kore/payload.c \
           kore/agent.c \
           kore/weather.cpp \
           gui/gvzoom.cpp \
           gui/weathimage.cpp \
           gui/images/imagesmanager.cpp \
           kore/signalisation.cpp \
           gui/signalisationgui.cpp \
           utils/json.c \
           utils/line.cpp \
           utils/vector.cpp \
           gui/vehiclegui.cpp \
           gui/traffic.cpp \
           kore/conversions.cpp

HEADERS  += \
           utils/debug.h \
           utils/point.h \
           utils/utils.h \
           utils/segment.h \
           kore/plan.h \
           kore/simulation.h \
           gui/images/image.h \
           gui/images/imagesignalisation.h \
           kore/driver.h \
           kore/vehicle.h \
           gui/images/imagevehicle.h \
           utils/log.h \
           kore/graph.h \
           gui/rarrow.h \
           kore/detector.h \
           kore/world.h \
           kore/parser.h \
           kore/osmdata.h \
           gui/simulationgui.h \
           gui/roadgui.h \
           kore/walker.h \
           gui/intersectiongui.h \
           kore/node.h \
           kore/edge.h \
           gui/pedestriangui.h \
           gui/vehimob.h \
           gui/weathimage.h \
           gui/aboutdialog.h \
           kore/conversions.h \
           kore/atypes.h \
           kore/payload.h \
           kore/agent.h \
           kore/weather.h \
           gui/gvzoom.h \
           gui/images/imagesmanager.h \
           kore/signalisation.h \
           gui/signalisationgui.h \
           utils/json.h \
           utils/line.h \
           utils/vector.h \
           gui/vehiclegui.h \
           gui/traffic.h \
           kore/config.h
           
FORMS    += gui/aboutdialog.ui gui/simulator.ui

RESOURCES += resources/resources.qrc

OTHER_FILES += default.qss resources/drag-icon.png

DISTFILES += \
    resources/RadarImages/Japan/ABC/radar_20140806_000000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_001000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_002000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_003000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_004000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_005000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_010000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_011000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_012000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_013000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_014000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_015000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_020000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_021000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_022000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_023000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_024000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_025000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_030000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_031000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_032000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_033000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_034000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_035000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_040000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_041000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_042000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_043000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_044000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_045000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_050000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_051000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_052000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_053000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_054000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_055000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_060000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_061000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_062000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_063000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_064000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_065000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_070000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_071000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_072000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_073000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_074000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_075000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_080000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_081000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_082000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_083000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_084000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_085000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_090000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_091000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_092000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_093000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_094000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_095000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_100000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_101000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_102000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_103000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_104000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_105000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_110000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_111000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_112000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_113000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_114000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_115000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_120000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_121000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_122000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_123000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_124000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_125000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_130000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_131000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_132000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_133000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_134000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_135000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_140000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_141000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_142000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_143000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_144000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_145000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_150000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_151000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_152000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_153000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_154000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_155000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_160000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_161000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_162000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_163000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_164000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_165000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_170000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_171000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_172000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_173000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_174000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_175000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_180000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_181000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_182000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_183000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_184000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_185000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_190000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_191000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_192000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_193000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_194000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_195000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_200000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_201000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_202000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_203000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_204000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_205000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_210000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_211000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_212000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_213000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_214000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_215000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_220000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_221000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_222000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_223000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_224000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_225000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_230000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_231000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_232000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_233000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_234000.gif \
    resources/RadarImages/Japan/ABC/radar_20140806_235000.gif \
    resources/RadarImages/Japan/ABC/sample_radar_20140806_140000.gif \
    README.md \
    resources/CHANGELOG




































