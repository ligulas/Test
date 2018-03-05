#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_all_beConnected_server();

    init_tableContent();

}


void MainWindow::init_all_beConnected_server()
{
    QMap<QString,int > all_address_info = g_get_global_ip_port_info.get_Ip_mapInfo();

    QMap<QString,int >::iterator iter_ = all_address_info.begin();

    m_pStore_db_operator = new QWriteDB_thread();

    for(;iter_ != all_address_info.end(); iter_++)
    {
        QString Ip_address = iter_.key();
        int port_info = iter_.value();

        //debug test
        QDataStream_ParseThread *one_server_thread= new QDataStream_ParseThread(Ip_address,port_info,this);

        connect(one_server_thread,SIGNAL(sig_container_size_notifier(QString,int)),
                this,SLOT(onRefresh_label_show_cacheSize(QString,int)));
        connect(one_server_thread,SIGNAL(sig_Parse_data_info(QString,float,float,float,QDateTime)),
                this,SLOT(onRefresh_table_item(QString,float,float,float,QDateTime)));

        connect(one_server_thread,SIGNAL(sig_container_size_notifier(QString,int)),
                m_pStore_db_operator,SLOT(deal_store_lx_DB_event(QString,int)));
        connect(one_server_thread,SIGNAL(sig_Parse_data_info(QString,float,float,float,QDateTime)),
                m_pStore_db_operator,SLOT(deal_store_zx_DB_event(QString,float,float,float,QDateTime)));

        one_server_thread->start();
        m_manger_all_server_data_dealer.insert(Ip_address,one_server_thread);
    }

    m_pStore_db_operator->init_data_source(m_manger_all_server_data_dealer);
    m_pStore_db_operator->startWork();
}

void MainWindow::init_table_header_Content()
{
    //表头外观
    QStringList headerInfo;
    headerInfo << QString("ID") << QString("服务端IP") << QString("A相温度")
               << QString("B相温度") << QString("C相温度") << QString("时标");

    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(headerInfo);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
}

void MainWindow::init_tableContent()
{
    m_ip_rowIndex_map.clear();
    init_table_header_Content();

    int rowCount = ui->tableWidget->rowCount();

    m_ip_cacheSize_str_map.clear();
    QString label_show_parseThread_container_size;
    QMap<QString,int > all_address_info = g_get_global_ip_port_info.get_Ip_mapInfo();
    QMap<QString,int >::iterator iter_ = all_address_info.begin();
    for(;iter_ != all_address_info.end(); iter_++)
    {
        ui->tableWidget->setRowCount(rowCount + 1);

        ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(QString("%1").arg(rowCount + 1)));
        ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(iter_.key()));
        ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem("-"));
        ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem("-"));
        ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem("-"));
        ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem("-"));

        m_ip_rowIndex_map.insert(iter_.key(),rowCount);
        rowCount = ui->tableWidget->rowCount();

        m_ip_cacheSize_str_map.insert(iter_.key(),QString("Server[%1] parsedCacheSize-->[0],\n").arg(iter_.key()));
        label_show_parseThread_container_size += QString("Server[%1] parsedCacheSize-->[0],\n").arg(iter_.key());
    }

    //初始化一下 label_show_cacheSize;
    ui->label_Show_cacheSize->setText(label_show_parseThread_container_size);
}


void MainWindow::onRefresh_label_show_cacheSize(QString ip_flag,int cacheSize)
{
    if(m_ip_cacheSize_str_map.count() <= 0)
        return;

    QString beFind_cacheStr = m_ip_cacheSize_str_map.value(ip_flag);
    if(beFind_cacheStr.isEmpty() || beFind_cacheStr.isNull())
        return;

    beFind_cacheStr = QString("Server[%1] parsedCacheSize-->[%2],\n").arg(ip_flag).arg(cacheSize);
    m_ip_cacheSize_str_map.insert(ip_flag,beFind_cacheStr);

    QString label_show_parseThread_container_size;
    QMap<QString,QString >::iterator iter_qstr = m_ip_cacheSize_str_map.begin();
    for(;iter_qstr != m_ip_cacheSize_str_map.end(); iter_qstr++)
    {
        label_show_parseThread_container_size += iter_qstr.value();
    }

    ui->label_Show_cacheSize->setText(label_show_parseThread_container_size);
}


void MainWindow::onRefresh_table_item(QString ip_flag,
                                      float f_phaseA_temp, float f_phaseB_temp, float f_phaseC_temp,
                                      QDateTime recvTime)
{
    int row_index = m_ip_rowIndex_map.value(ip_flag);

    QTableWidgetItem *widgetItem_A_temp = ui->tableWidget->item(row_index,2);
    if(widgetItem_A_temp == NULL)
    {
        qCritical() << Q_FUNC_INFO << QString(" impossble QTableWidgetItem situation,RowIndex:%1,columnIndex:2")
                                            .arg(row_index);
        return;
    }
    widgetItem_A_temp->setText(QString::number(f_phaseA_temp));


    QTableWidgetItem *widgetItem_B_temp = ui->tableWidget->item(row_index,3);
    if(widgetItem_B_temp == NULL)
    {
        qCritical() << Q_FUNC_INFO << QString(" impossble QTableWidgetItem situation,RowIndex:%1,columnIndex:3")
                                            .arg(row_index);
        return;
    }
    widgetItem_B_temp->setText(QString::number(f_phaseB_temp));

    QTableWidgetItem *widgetItem_C_temp = ui->tableWidget->item(row_index,4);
    if(widgetItem_C_temp == NULL)
    {
        qCritical() << Q_FUNC_INFO << QString(" impossble QTableWidgetItem situation,RowIndex:%1,columnIndex:4")
                                            .arg(row_index);
        return;
    }
    widgetItem_C_temp->setText(QString::number(f_phaseC_temp));

    QTableWidgetItem *widgetItem_time = ui->tableWidget->item(row_index,5);
    if(widgetItem_time == NULL)
    {
        qCritical() << Q_FUNC_INFO << QString(" impossble QTableWidgetItem situation,RowIndex:%1,columnIndex:5")
                                            .arg(row_index);
        return;
    }
    widgetItem_time->setText(recvTime.toString("yyyy-MM-dd hh:mm:ss"));
}


MainWindow::~MainWindow()
{
    delete ui;

    if(m_pStore_db_operator != NULL)
    {
        delete m_pStore_db_operator;
        m_pStore_db_operator = NULL;
    }
}
