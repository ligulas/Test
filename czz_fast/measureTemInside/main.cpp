#include "mainwindow.h"

#include <QThread>
#include <QApplication>

#include "qmy_global.h"
#include "qlog_message_handler.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //定义日志文件位置
    message_handler::QLog_MessageHandler::setLogFile(LightFiberTemp_Client_LOG_FILE_PATH);
    qInstallMessageHandler(message_handler::QLog_MessageHandler::FormatMessage_myown);

    qDebug() << QString("[From main()]: ") <<QThread::currentThreadId();

    //读全局的IP Port 信息
    g_get_global_ip_port_info.init_global_ip_port_info();

    MainWindow w;
    w.show();

    return a.exec();

    qDebug() << QString("[From main()]: ") << QString("ready to exit main()");
}
