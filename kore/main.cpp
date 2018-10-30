//
// main application entry
//

#include <QApplication>
#include <QResource>
#include <QGraphicsView>
#include <iostream>
#include <qwidget.h>
#include <QTime>
#include <QMainWindow>

#include "gui/rarrow.h"
#include "graph.h"
#include "traffic.h"
#include "config.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TrafficSimulator simulator;
    simulator.show();
    return app.exec();
}

