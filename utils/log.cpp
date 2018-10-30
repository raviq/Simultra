#include <QtDebug>
#include <QTextStream>
#include <QFile>
#include <QTime>
#include <iostream>
#include "log.h"
#include "utils.h"
#include "config.h"

using namespace std;

void Logs::init()
{
   qInstallMessageHandler(myMessageOutput);

   qDebug()     << "t,vehicle,speed,v.x,v.y,angle,dest.x,dest.y,wheelbase,x,y" << endl;
   qWarning()   << "t,vehicle,lane,road,length,start.x,start.y,end.x,end.y,speed,x,y" << endl;
   qCritical()  << "detector,vehicle,x1,y1,x2,y2,t" << endl;

}

void Logs::close()
{
    // TODO
}

// New output classes
// enum LogMsgType { //QtMsgType
//    FlowLogMsg,
//    LanesLogMsg,
//    VehiclesLogMsg,
//    DetectLogMsg
//};

void Logs::myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString path = qApp->applicationDirPath();

    UNUSED(context);

     // Write the message to any stream!
     switch (type)
     {
          case QtDebugMsg:
          {
                 const QString fileName = path + _VEHICLES_CSV_;
                 QString txt = msg;
                 txt.prepend(" ");

                 QFile *logFile = new QFile(fileName);
                 if (logFile->open(QIODevice::WriteOnly | QIODevice::Append))
                 {
                     QTextStream out(logFile);
                     out << txt;
                 }
                 delete logFile; //TODO
                 break;
          }

        case QtWarningMsg:
        {
            const QString fileName_ = path + _FLOW_CSV_;
            QString txt_ = msg;
            txt_.prepend(" ");

            QFile *logFile_ = new QFile(fileName_);
            if (logFile_->open(QIODevice::WriteOnly | QIODevice::Append))
            {
                QTextStream out_(logFile_);
                out_ << txt_;
            }
            delete logFile_; //TODO
            break;
         }

         case QtCriticalMsg:
         {
             const QString fileName_2 = path + _DETECTIONS_CSV_;
             QString txt_2 = msg;
             txt_2.prepend(" ");

             QFile *logFile_2 = new QFile(fileName_2);
             if (logFile_2->open(QIODevice::WriteOnly | QIODevice::Append))
             {
                 QTextStream out_2(logFile_2);
                 out_2 << txt_2;
             }
             delete logFile_2; //TODO
             break;
          }

         case QtFatalMsg:
         {
            perror("QtFatalMsg");
             break;
         }

         case QtInfoMsg:
         {
             perror("QtInfoMsg");
             break;
         }

     }
 }
