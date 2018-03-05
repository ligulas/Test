#include "qsocketthread.h"

QSocketThread::QSocketThread(QObject *parent)
    :  QThread(parent)
{
    m_socket = NULL;
    m_byte_array_cache.clear();     //接收数据缓冲区清空

    m_SendGetData_cmd_timer = NULL;
    m_reConnect_timer = NULL;

    m_receive_dataStream_list.clear();
    m_port_num = 50005;     //设置默认端口号

    if(!m_server_address.setAddress("127.0.0.1"))
    {
        qCritical() << QString("[QSocketThread::QSocketThread()]: init m_server_address to 127.0.0.1 failed,check");
    }

}


QSocketThread::~QSocketThread()
{
    if(m_SendGetData_cmd_timer != NULL)
    {
        if(m_SendGetData_cmd_timer->isActive())
            m_SendGetData_cmd_timer->stop();

        delete m_SendGetData_cmd_timer;
    }

    if(m_reConnect_timer != NULL)
    {
        if(m_reConnect_timer->isActive())
            m_reConnect_timer->stop();

        delete m_reConnect_timer;
    }

    if(m_socket != NULL)
    {
        if(m_socket->state() == QAbstractSocket::ConnectedState)
            m_socket->abort();

        m_socket->deleteLater();
    }


    // Exit event loop
    quit();
    wait();
}

bool QSocketThread::set_Server_ip_port(const QString &ip_info,const int iPort)
{
    m_port_num = iPort;

    if(!m_server_address.setAddress(ip_info))
    {
        qCritical() << QString("the ip address was not successfully parsed,check");
       return false;
    }

    return true;
}

void QSocketThread::push_new_dataStream_back_toList(const QByteArray raw_bytes)
{
    RAW_DATA_AT_TIMESTAMP toBeCopyed_struct;
    toBeCopyed_struct.raw_data = raw_bytes;
    toBeCopyed_struct.match_time = QDateTime::currentDateTime();     //打上时标

    int hours_= toBeCopyed_struct.match_time.time().hour();
    int minutes = toBeCopyed_struct.match_time.time().minute();
    int second_ = toBeCopyed_struct.match_time.time().second();

    m_receiveList_mutex.lock();
    m_receive_dataStream_list.push_back(toBeCopyed_struct);
    m_receiveList_mutex.unlock();
}

QList<RAW_DATA_AT_TIMESTAMP> QSocketThread::get_all_dataList_fromList()
{
    m_receiveList_mutex.lock();
    QList<RAW_DATA_AT_TIMESTAMP> result_list = m_receive_dataStream_list;
    m_receive_dataStream_list.clear();
    m_receiveList_mutex.unlock();

    return result_list;
}


void QSocketThread::run()
{
    qDebug() << Q_FUNC_INFO <<QString(",ready to into thread event loop,") << QThread::currentThreadId();
    m_socket = new QTcpSocket;
    m_reConnect_timer = new QTimer;
    m_SendGetData_cmd_timer = new QTimer;
    //5秒 尝试重连 如果连不上的话
    m_reConnect_timer->setInterval(5000);
    //3秒间隔 发送 取数据命令 如果连接在线
    m_SendGetData_cmd_timer->setInterval(3000);


    connect(m_SendGetData_cmd_timer,SIGNAL(timeout()),this,SLOT(onSend_GetData_cmd_event()),Qt::DirectConnection);
    connect(m_reConnect_timer,SIGNAL(timeout()),this,SLOT(onReConnect_event()),Qt::DirectConnection);
    //建立客户端的异步处理函数 连接
    connect(m_socket, SIGNAL(connected()),    this, SLOT(onConnected()), Qt::DirectConnection);
    connect(m_socket, SIGNAL(readyRead()),    this, SLOT(onReadyRead()), Qt::DirectConnection);
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnected()), Qt::DirectConnection);

    m_socket->connectToHost(m_server_address,m_port_num);
    m_reConnect_timer->start();
    m_SendGetData_cmd_timer->start();

    exec();
}

void QSocketThread::timerEvent(QTimerEvent *e)
{

}

void QSocketThread::onConnected()
{
    qDebug() << QString("Connection to Server[%1] established").arg(m_server_address.toString());
    if(m_socket == NULL)
    {
        qCritical() << QString("[QSocketThread::onConnected()]: m_socket == NULL impossible !!");
        return;
    }

    //记录原始的套接字描述符
    m_descriptor = m_socket->socketDescriptor();
    if(m_descriptor == -1)
    {
        qCritical() << QString("[QSocketThread::onConnected()]: m_descriptor == -1 impossible,check");
    }

}

void QSocketThread::onReadyRead()
{
    //缓冲区中没有数据，直接无视
    qint64 size_bytes_receive = -1;
    if((size_bytes_receive = m_socket->bytesAvailable()) <=0)
    {
        return;
    }

    //QByteArray tmp_byteArray = m_socket->readAll();
    //qDebug() << QString("Received From Server[%1]:").arg(m_server_address.toString())<< QString("  [receive Raw Data]: ") << tmp_byteArray;

    //临时用于存储缓冲区中读出来的数据    //下面这里还可以再抽象
    m_byte_array_cache.append(m_socket->readAll());

    while(m_byte_array_cache.count() >= RECEIVE_PACKAGE_LENGTH)
    {
        QByteArray deal_data_cache = m_byte_array_cache.left(RECEIVE_PACKAGE_LENGTH);
        m_byte_array_cache.remove(0,RECEIVE_PACKAGE_LENGTH);

        unsigned char raw_data_uc[RECEIVE_PACKAGE_LENGTH] = {0};
        memcpy(raw_data_uc,deal_data_cache.data(),RECEIVE_PACKAGE_LENGTH);

        if(QDataStream_ParseThread::crcCheck(raw_data_uc,RECEIVE_PACKAGE_LENGTH))
        {
            push_new_dataStream_back_toList(deal_data_cache);
        }
        else
        {
            qCritical() << Q_FUNC_INFO <<QString("  From Server[%1] {Receive Data CRC Check error,clear the cache} ")
                           .arg(m_server_address.toString());
            m_byte_array_cache.clear();
        }

    }

    //客户端设计成一直在线，不主动断开
    //m_socket->disconnectFromHost();

}

void QSocketThread::onDisconnected()
{
    qDebug() << QString("DisConnected From Server[%1] ").arg(m_server_address.toString());
    m_socket->close();

    //服务端断开了链接，尝试重新连接
    m_socket->connectToHost(m_server_address,m_port_num);

    //20秒内 等待 看是否能重连
    if(!m_socket->waitForConnected(20000))
    {
        qCritical() << QString("Can't connect to host[%1],check ").arg(m_server_address.toString());
    }

}

void QSocketThread::onSend_GetData_cmd_event()
{
    if (m_socket->state() != QAbstractSocket::ConnectedState )
    {
        qDebug() << Q_FUNC_INFO << QString(" Server[%1] m_socket->state() != QAbstractSocket::ConnectedState").arg(m_server_address.toString());
        return;
    }

    //debug test
    unsigned char raw_bytes_to_send[8] = {0};
    QString send_cmd_qstr;
    QDataStream_ParseThread::get_constant_modbus_cmd(send_cmd_qstr);
    QDataStream_ParseThread::strToHexByte(send_cmd_qstr.toStdString(),raw_bytes_to_send,8);
    QByteArray block((const char *)raw_bytes_to_send,8);

    m_socket->write(block);
}


void QSocketThread::onReConnect_event()
{
    //qDebug() << Q_FUNC_INFO << QThread::currentThreadId();
    if(m_socket == NULL)
    {
        qCritical() << Q_FUNC_INFO << QString("m_socket == NULL ,impossible ,check");
        return;
    }

    if (m_socket->state() != QAbstractSocket::ConnectedState )
    {
        //qDebug() << Q_FUNC_INFO << QString("ready to reConnect to Server[%1]").arg(m_server_address.toString());
        m_socket->connectToHost(m_server_address,m_port_num);
    }
}
