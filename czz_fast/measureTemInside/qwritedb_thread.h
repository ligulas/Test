#ifndef QWRITEDB_THREAD_H
#define QWRITEDB_THREAD_H

#include <QThread>
#include <QObject>
#include <QUuid>

#include "qdataoperator.h"
#include "qmy_global.h"
#include "qdatastream_parsethread.h"

#define CACHE_BATCH_DEAL_RECORD_NUM 10

class QWriteDB_thread:public QObject
{
    Q_OBJECT
public:
    QThread m_workThread;

public:
    QWriteDB_thread();
    ~QWriteDB_thread();

    void init_data_source(const QMap<QString,QDataStream_ParseThread *> &para_data_source);
    void startWork();

public slots:
    void deal_store_lx_DB_event(QString device_addr_ip, int data_list_count);    //data_list_count 代表解析数据线程目前内部拥有的数据量
    void deal_store_zx_DB_event(QString device_addr_ip,
                                float a_phase_f,float b_phase_f,float c_phase_f,
                                QDateTime new_data_rcvTime);


private:
    void do_store_lx_DB_detail_act(const QList<ZX_DATA_SRC> &lx_data_src_list);
    void do_store_zx_DB_detail_act(const QList<ZX_DATA_SRC> &data_sourceList);

private:
    QMap<QString,QDataStream_ParseThread *> m_parsed_data_source;

};

#endif // QWRITEDB_THREAD_H
