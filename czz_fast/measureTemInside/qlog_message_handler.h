#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <QDebug>
#include <QFile>

#include "qmy_global.h"

//注意:外部在调用这个类的时候注意要配合信号把 信息messmage 通知出去

namespace message_handler {
class  QLog_MessageHandler: public QFile
{
private:
    QLog_MessageHandler();
    ~QLog_MessageHandler();

public:
    static void setLogFile(QString file = "CeWen.log");
    static void FormatMessage_myown(QtMsgType type,const QMessageLogContext & log_context,const QString &msg);

private:
    static QString s_log;
};
}


#endif // MESSAGEHANDLER_H
