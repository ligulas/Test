#ifndef QSOCKETTHREAD_H
#define QSOCKETTHREAD_H

#include <QMutex>
#include <QTimer>
#include <QTcpSocket>
#include <QThread>
#include <QObject>
#include <QDataStream>
#include <QHostAddress>
#include <QDateTime>

#include "qdatastream_parsethread.h"

typedef struct _RAW_DATA_AT_TIMESTAMP
{
    QByteArray raw_data;
    QDateTime match_time;

}RAW_DATA_AT_TIMESTAMP;

class QTcpSocket;

class QSocketThread: public QThread
{
    Q_OBJECT
public:
    QSocketThread(QObject *parent = 0);
    ~QSocketThread();

    bool set_Server_ip_port(const QString &ip_info,const int iPort = 50005);

    void push_new_dataStream_back_toList(const QByteArray raw_bytes);
    QList<RAW_DATA_AT_TIMESTAMP> get_all_dataList_fromList();

protected:
    void run() Q_DECL_OVERRIDE;

    void timerEvent(QTimerEvent *e);

private slots:
    void onConnected();
    void onReadyRead();
    void onDisconnected();

    void onSend_GetData_cmd_event();
    void onReConnect_event();

private:

    int m_port_num;
    QHostAddress m_server_address;

    QTcpSocket *m_socket;
    qintptr m_descriptor;

    QTimer *m_SendGetData_cmd_timer;
    QTimer *m_reConnect_timer;

    QMutex m_receiveList_mutex;
    QList<RAW_DATA_AT_TIMESTAMP> m_receive_dataStream_list;

    QByteArray m_byte_array_cache;
};


#endif // QSOCKETTHREAD_H
