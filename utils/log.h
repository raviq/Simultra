#ifndef LOG_H
#define LOG_H

#include <QApplication>

namespace Logs
{
    void init();
    void close();
    void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg);
}

#endif // LOG_H
