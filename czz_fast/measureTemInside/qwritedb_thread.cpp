#include "qwritedb_thread.h"

QWriteDB_thread::QWriteDB_thread()
{
    this->moveToThread(&m_workThread);
}


QWriteDB_thread::~QWriteDB_thread()
{
    if(m_workThread.isRunning())
    {
        m_workThread.quit();

        m_workThread.wait();
    }
}

void QWriteDB_thread::init_data_source(const QMap<QString,QDataStream_ParseThread *> &para_data_source)
{
    m_parsed_data_source = para_data_source;
}

void QWriteDB_thread::startWork()
{
    m_workThread.start();
}


void QWriteDB_thread::deal_store_lx_DB_event(QString device_addr_ip, int data_list_count)
{
    QMap<QString,int> ip_addr_objId_map_info = g_get_global_ip_port_info.get_obj_id_MapInfo();

    if(!ip_addr_objId_map_info.contains(device_addr_ip))
    {
        qCritical() << Q_FUNC_INFO << QString("[impossible error!]: ip_addr_objId_map_info didnot find %1 obj_id mapInfo").arg(device_addr_ip);
        return;
    }

    //缓存内小于 CACHE_BATCH_DEAL_RECORD_NUM 条数据不处理,等数据多了,批量处理
    if(data_list_count <= CACHE_BATCH_DEAL_RECORD_NUM)
    {
        return;
    }

    if(m_parsed_data_source.value(device_addr_ip) == NULL)
    {
        qCritical() << Q_FUNC_INFO << QString("[unusual problem:] m_parsed_data_source.value(device_addr_ip) == NULL");
        return;
    }

    QDataStream_ParseThread * data_getor = m_parsed_data_source.value(device_addr_ip);

    QList<ZX_DATA_SRC> toBe_deal_lx_data_list;
    QMap<QDateTime,FIBRE_TEMP_STRUC> cache_data_list = data_getor->get_parsed_data_list();
    QMap<QDateTime,FIBRE_TEMP_STRUC>::iterator iterator_ = cache_data_list.begin();
    for(; iterator_!=cache_data_list.end() ;iterator_++)
    {

        //类型 和 参数ID 定死 obj_id 来自配置文件
        //A相
        ZX_DATA_SRC a_phase_struct_data;
        a_phase_struct_data.stu_obj_id = QString::number(ip_addr_objId_map_info.value(device_addr_ip));
        a_phase_struct_data.stu_data_define = "0";
        a_phase_struct_data.stu_obj_type = "162";
        a_phase_struct_data.stu_item_id = "2";

        a_phase_struct_data.stu_rcvtime = iterator_.key();
        a_phase_struct_data.stu_f_phase_temper = iterator_.value().phase_A_temp;

        //B相
        ZX_DATA_SRC b_phase_struct_data;
        b_phase_struct_data.stu_obj_id = QString::number(ip_addr_objId_map_info.value(device_addr_ip));
        b_phase_struct_data.stu_data_define = "0";
        b_phase_struct_data.stu_obj_type = "162";
        b_phase_struct_data.stu_item_id = "3";

        b_phase_struct_data.stu_rcvtime = iterator_.key();
        b_phase_struct_data.stu_f_phase_temper = iterator_.value().phase_B_temp;

        //C相
        ZX_DATA_SRC c_phase_struct_data;
        c_phase_struct_data.stu_obj_id = QString::number(ip_addr_objId_map_info.value(device_addr_ip));
        c_phase_struct_data.stu_data_define = "0";
        c_phase_struct_data.stu_obj_type = "162";
        c_phase_struct_data.stu_item_id = "4";

        c_phase_struct_data.stu_rcvtime = iterator_.key();
        c_phase_struct_data.stu_f_phase_temper = iterator_.value().phase_C_temp;

        toBe_deal_lx_data_list.push_back(a_phase_struct_data);
        toBe_deal_lx_data_list.push_back(b_phase_struct_data);
        toBe_deal_lx_data_list.push_back(c_phase_struct_data);
    }

    do_store_lx_DB_detail_act(toBe_deal_lx_data_list);
}

void QWriteDB_thread::deal_store_zx_DB_event(QString device_addr_ip,
                                             float a_phase_f,float b_phase_f,float c_phase_f,
                                             QDateTime new_data_rcvTime)
{

    QMap<QString,int> ip_addr_objId_map_info = g_get_global_ip_port_info.get_obj_id_MapInfo();

    if(!ip_addr_objId_map_info.contains(device_addr_ip))
    {
        qCritical() << Q_FUNC_INFO << QString("[impossible error!]: ip_addr_objId_map_info didnot find %1 obj_id mapInfo").arg(device_addr_ip);
        return;
    }

    //类型 和 参数ID 定死
    //A相
    ZX_DATA_SRC a_phase_struct_data;
    a_phase_struct_data.stu_obj_id = QString::number(ip_addr_objId_map_info.value(device_addr_ip));
    a_phase_struct_data.stu_data_define = "0";
    a_phase_struct_data.stu_obj_type = "162";
    a_phase_struct_data.stu_item_id = "2";

    a_phase_struct_data.stu_rcvtime = new_data_rcvTime;
    a_phase_struct_data.stu_f_phase_temper = a_phase_f;

    //B相
    ZX_DATA_SRC b_phase_struct_data;
    b_phase_struct_data.stu_obj_id = QString::number(ip_addr_objId_map_info.value(device_addr_ip));
    b_phase_struct_data.stu_data_define = "0";
    b_phase_struct_data.stu_obj_type = "162";
    b_phase_struct_data.stu_item_id = "3";

    b_phase_struct_data.stu_rcvtime = new_data_rcvTime;
    b_phase_struct_data.stu_f_phase_temper = b_phase_f;

    //C相
    ZX_DATA_SRC c_phase_struct_data;
    c_phase_struct_data.stu_obj_id = QString::number(ip_addr_objId_map_info.value(device_addr_ip));
    c_phase_struct_data.stu_data_define = "0";
    c_phase_struct_data.stu_obj_type = "162";
    c_phase_struct_data.stu_item_id = "4";

    c_phase_struct_data.stu_rcvtime = new_data_rcvTime;
    c_phase_struct_data.stu_f_phase_temper = c_phase_f;

    //同时存储 ABC 三相数据
    QList<ZX_DATA_SRC> toBe_store_dataList;
    toBe_store_dataList.push_back(a_phase_struct_data);
    toBe_store_dataList.push_back(b_phase_struct_data);
    toBe_store_dataList.push_back(c_phase_struct_data);


    do_store_zx_DB_detail_act(toBe_store_dataList);
}


void QWriteDB_thread::do_store_lx_DB_detail_act(const QList<ZX_DATA_SRC> &lx_data_src_list)
{
    QUuid quid;
    QString guid_string = quid.createUuid().toString();
    QString random_connName = "store_lx" + guid_string;
    {
        QDataOperator mysqlDataBase;
        if(!mysqlDataBase.OpenDataBase(random_connName))
        {
            qCritical() << Q_FUNC_INFO << QString("open dataBase failed,check");
            goto CLEAR_UP;
        }

        if(!QSqlDatabase::database(random_connName).driver()->hasFeature(QSqlDriver::Transactions))
        {
            qCritical() << Q_FUNC_INFO << QString("the database in use didn't supports SQL transactions.check");
            goto CLEAR_UP;
        }

        //开始事务
        if(!QSqlDatabase::database(random_connName).transaction())
        {
            qCritical() << Q_FUNC_INFO << QString("the database begin transaction failed,check");
            goto CLEAR_UP;
        }

        mysqlDataBase.Insert_Into_Lx_Data_Src_Batch(lx_data_src_list);


        //提交
        if(!QSqlDatabase::database(random_connName).commit())
        {
           qWarning() << Q_FUNC_INFO << QSqlDatabase::database(random_connName).lastError();

           //回滚
           if(!QSqlDatabase::database(random_connName).rollback())
               qWarning() << Q_FUNC_INFO << QSqlDatabase::database(random_connName).lastError();
        }

    }

CLEAR_UP:
    QSqlDatabase::removeDatabase(random_connName);
}

void QWriteDB_thread::do_store_zx_DB_detail_act(const QList<ZX_DATA_SRC> &data_sourceList)
{
    QUuid quid;
    QString guid_string = quid.createUuid().toString();
    QString random_connName = "store_zx" + guid_string;
    {
        QDataOperator mysqlDataBase;
        if(!mysqlDataBase.OpenDataBase(random_connName))
        {
            qCritical() << Q_FUNC_INFO << QString("open dataBase failed,check");
            goto CLEAR_UP;
        }

        if(!QSqlDatabase::database(random_connName).driver()->hasFeature(QSqlDriver::Transactions))
        {
            qCritical() << Q_FUNC_INFO << QString("the database in use didn't supports SQL transactions.check");
            goto CLEAR_UP;
        }

        //开始事务
        if(!QSqlDatabase::database(random_connName).transaction())
        {
            qCritical() << Q_FUNC_INFO << QString("the database begin transaction failed,check");
            goto CLEAR_UP;
        }

        for(int iCnt= 0 ; iCnt < data_sourceList.count() ; iCnt++)
        {
            ZX_DATA_SRC temp_record = data_sourceList.at(iCnt);

            int recordNum = mysqlDataBase.Select_Count_at_Table_Condition("zx_data_src",
                                              QString("obj_id = %1 and obj_type = %2 and datadef = %3 and item_id = %4")
                                                 .arg(temp_record.stu_obj_id)
                                                 .arg(temp_record.stu_obj_type)
                                                 .arg(temp_record.stu_data_define)
                                                 .arg(temp_record.stu_item_id));

            //之前有对应记录
            if(recordNum > 0)
            {
                mysqlDataBase.Update_Zx_Data_Src(temp_record);
            }
            //之前无对应记录
            else if(recordNum == 0)
            {
                mysqlDataBase.Insert_Into_Zx_Data_Src(temp_record);
            }
            //执行错误
            else if(recordNum == -1)
                qCritical() << Q_FUNC_INFO << QString("Select_Count_at_Table_Condition exec error,check");
            else
                qCritical() << Q_FUNC_INFO <<QString("impossible");
        }




        //提交
        if(!QSqlDatabase::database(random_connName).commit())
        {
           qWarning() << Q_FUNC_INFO << QSqlDatabase::database(random_connName).lastError();

           //回滚
           if(!QSqlDatabase::database(random_connName).rollback())
               qWarning() << Q_FUNC_INFO << QSqlDatabase::database(random_connName).lastError();
        }

    }

CLEAR_UP:
    QSqlDatabase::removeDatabase(random_connName);
}
