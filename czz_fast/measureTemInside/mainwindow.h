#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qmy_global.h"
#include "qdatastream_parsethread.h"
#include "qwritedb_thread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init_all_beConnected_server();
    void init_table_header_Content();
    void init_tableContent();

private slots:
    void onRefresh_table_item(QString ip_flag,
                              float f_phaseA_temp, float f_phaseB_temp, float f_phaseC_temp,
                              QDateTime recvTime);
    void onRefresh_label_show_cacheSize(QString ip_flag,int cacheSize);

private:
    Ui::MainWindow *ui;

    QMap<QString,QString> m_ip_cacheSize_str_map;
    QMap<QString,int> m_ip_rowIndex_map;
    QMap<QString,QDataStream_ParseThread *> m_manger_all_server_data_dealer;     //某个设备Ip 对应的数据解析线程

    QWriteDB_thread *m_pStore_db_operator;
};

#endif // MAINWINDOW_H
