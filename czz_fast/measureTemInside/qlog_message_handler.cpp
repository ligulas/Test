#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QMutex>

#include "qlog_message_handler.h"

using namespace message_handler;


QString QLog_MessageHandler::s_log = LightFiberTemp_Client_LOG_FILE_PATH;


QLog_MessageHandler::QLog_MessageHandler()
{

}

QLog_MessageHandler::~QLog_MessageHandler()
{

}

void QLog_MessageHandler::setLogFile(QString file)
{
    s_log = file;

    QDir logFileDir(LightFiberTemp_Client_LOG_DIR);

    bool bIsExist = logFileDir.exists();
    if(!bIsExist)
    {
        logFileDir.mkpath(LightFiberTemp_Client_LOG_DIR);
    }
}


void QLog_MessageHandler::FormatMessage_myown(QtMsgType type,const QMessageLogContext & log_context,const QString & msg)
{
    static QMutex mutex;
    mutex.lock();

    QFile logFile(QLog_MessageHandler::s_log);
    bool bIsExist = logFile.exists();
    if(bIsExist)
    {
        logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    else
    {
        logFile.open(QIODevice::WriteOnly | QIODevice::Append);

        //初始化日志
        QString log_init("");
        log_init += QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]") + QString(" ");
        log_init += QString("[Info] ");
        log_init += QString("$$ %1 Log File Create!!").arg(logFile.fileName());

#ifdef Q_OS_WIN
        log_init += "\r\n";
#else
        log_init += "\n";
#endif

        QTextStream text_stream_init(&logFile);
        text_stream_init << log_init;
        logFile.flush();
    }

    if(logFile.size()>10*1024*1024)
    {
        logFile.resize(0);
    }

    QString log("");
    log += QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]") + QString(" ");

    switch(type)
    {
    case QtDebugMsg:
        log += QString("[Debug]");
        break;

    case QtWarningMsg:
        log += QString("[Warn]");
        break;

    case QtCriticalMsg:
        log += QString("[Critical]");
        break;

    case QtFatalMsg:
        log += QString("[Fatal]");
        abort();
        break;

    default:
        log += QString("[Info]");
        break;
    };

    log += QString(" $$ %1").arg(msg);
#ifdef Q_OS_WIN
    log += "\r\n";
#else
    log += "\n";
#endif

    QTextStream text_stream(&logFile);
    text_stream << log;
    logFile.flush();
    logFile.close();

    mutex.unlock();
}
