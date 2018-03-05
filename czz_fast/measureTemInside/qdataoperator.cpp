#include <QVariant>
#include <QFile>
#include <QSettings>
#include <QDebug>
#include <QSqlError>
#include <QString>
#include <QMessageBox>
#include "qdataoperator.h"


enum{SQLSERVER=1, ORACLE=2, MYSQL=3, ACCESS=4};

QDataOperator::QDataOperator()
{
    m_strHostName = "127.0.0.1";
    m_strDbName="sddl";
    m_strUserName="hoshing";
    m_strPassword="hoshingpwd";
}

QDataOperator::~QDataOperator()
{
    bool bIsOpen = db.isOpen();
    if(bIsOpen)
    {
        db.close();
    }
}

bool QDataOperator::OpenDataBase(QString connect_name)
{
    int nType = MYSQL;

    switch (nType)
    {
    case ORACLE:
        return OpenDataBase_Sql(g_global_logon_db_name, g_dataBase_ip, g_dataBase_port, g_global_logon_db_user, g_global_logon_db_pass, connect_name);
    case SQLSERVER:
        return false;  //暂不支持
    case MYSQL:
        return OpenDataBase_MYSQL(g_global_logon_db_name, g_dataBase_ip, g_dataBase_port, g_global_logon_db_user, g_global_logon_db_pass, connect_name);
    }

    return false;
}

// the code is used for mysql connect
bool QDataOperator::OpenDataBase_MYSQL(QString strDbName, QString strIP, int nPort, QString strUserName, QString strpassword, QString name)
{
    m_strHostName = strIP;
    m_strDbName=strDbName;
    m_strUserName=strUserName;
    m_strPassword=strpassword;

    db = QSqlDatabase::addDatabase("QMYSQL",name);
    QStringList lst = QSqlDatabase::drivers();
    db.setHostName(m_strHostName);
    db.setPort(nPort);
    db.setUserName(m_strUserName);
    db.setPassword(m_strPassword);
    db.setDatabaseName(m_strDbName);
    bool bIsOpen = db.open();

    if(!bIsOpen)
        qCritical() << Q_FUNC_INFO << QString("[open db error:] %1").arg(db.lastError().text());


    return bIsOpen;
}

bool QDataOperator::OpenDataBase_Sql(QString strDbName, QString strIP, int nPort, QString strUserName, QString strpassword, QString name)
{
    m_strHostName = strIP;
    m_strDbName=strDbName;
    m_strUserName=strUserName;
    m_strPassword=strpassword;

    db = QSqlDatabase::addDatabase("QODBC",name);
    //QStringList lst = QSqlDatabase::drivers();
    db.setHostName(m_strHostName);
    db.setPort(nPort);
    db.setUserName(m_strUserName);
    db.setPassword(m_strPassword);
    db.setDatabaseName(m_strDbName);

    bool bIsOpen = db.open();
    if(!bIsOpen)
    {
        QMessageBox::critical(0, QObject::tr("Database Error"),db.lastError().text());
    }

    return bIsOpen;
}

bool QDataOperator::IsOpen()
{
    bool bIsOpen = db.isOpen();
    return bIsOpen;
}


int QDataOperator::Select_Count_at_Table_Condition(const QString &tableName,const QString &whereCondition)
{
    int rowCount = -1;
    QSqlQuery sqlQuery(db);

    QString strSql = QString("select count(1) from %1 t where %2 ").arg(tableName).arg(whereCondition);

    if(!sqlQuery.exec(strSql))
    {
        qCritical() << Q_FUNC_INFO << QString("[select count error!]: %1").arg(sqlQuery.lastError().text());
        return rowCount;
    }

    while(sqlQuery.next())
    {
        rowCount = sqlQuery.value(0).toInt();
    }

    if(rowCount == -1)
        qCritical() << Q_FUNC_INFO << QString("[impossible error!] check!");

    return rowCount;

}

//批量执行插入语句
bool QDataOperator::Insert_Into_Lx_Data_Src_Batch(const QList<ZX_DATA_SRC> &lx_data_src_list)
{
    QSqlQuery sqlQuery(db);

    QString strSql;
    strSql = "INSERT INTO Lx_Data_src( OBJ_ID, OBJ_TYPE," \
            " ITEM_ID, DATADEF, RCVTIME, DATA )" \
             " VALUES(?, ?," \
             " ?, ?, ?, ?)";

    if(!sqlQuery.prepare(strSql))
    {
        qCritical() << Q_FUNC_INFO << QString("[error!]: %1").arg(sqlQuery.lastError().text()) ;
        return false;
    }

    QVariantList obj_id_list;        //decimal
    QVariantList obj_type_list;      //decimal
    QVariantList item_id_list;       //decimal
    QVariantList data_def_list;      //decimal
    QVariantList revtime_list;       //datetime
    QVariantList float_data_list;    //decimal float

    for(int iCnt = 0 ; iCnt<lx_data_src_list.count() ; iCnt++)
    {
        obj_id_list.push_back(QVariant(lx_data_src_list.at(iCnt).stu_obj_id.toInt()));
        obj_type_list.push_back(QVariant(lx_data_src_list.at(iCnt).stu_obj_type.toInt()));
        item_id_list.push_back(QVariant(lx_data_src_list.at(iCnt).stu_item_id.toInt()));
        data_def_list.push_back(QVariant(lx_data_src_list.at(iCnt).stu_data_define.toInt()));
        revtime_list.push_back(QVariant(lx_data_src_list.at(iCnt).stu_rcvtime));
        float_data_list.push_back(QVariant(lx_data_src_list.at(iCnt).stu_f_phase_temper));
    }

    sqlQuery.addBindValue(obj_id_list);
    sqlQuery.addBindValue(obj_type_list);
    sqlQuery.addBindValue(item_id_list);
    sqlQuery.addBindValue(data_def_list);
    sqlQuery.addBindValue(revtime_list);
    sqlQuery.addBindValue(float_data_list);

    //sqlQuery.bindValue(":CDATA", QVariant(zx_data_src.CDATA));
    //sqlQuery.bindValue(":ANATYPE", QVariant(zx_data_src.ANATYPE));
    //sqlQuery.bindValue(":ANATXT", QVariant(zx_data_src.ANATXT));
    //sqlQuery.bindValue(":BATCH", QVariant(zx_data_src.BATCH));

    if(!sqlQuery.execBatch())
    {
        qCritical() << Q_FUNC_INFO << QString("[execBatch error!]: %1").arg(sqlQuery.lastError().text());
        return false;
    }

    return true;
}



bool  QDataOperator::Insert_Into_Lx_Data_Src(const ZX_DATA_SRC &zx_data_src)
{
    QSqlQuery sqlQuery(db);

    QString strSql;
    strSql = "INSERT INTO Lx_Data_src( OBJ_ID, OBJ_TYPE," \
            " ITEM_ID, DATADEF, RCVTIME, DATA, CDATA, ANATYPE, ANATXT, BATCH )" \
             " VALUES(?,?," \
             " ?,?, ?,?, ?,?, ?,?)";


    if(!sqlQuery.prepare(strSql))
    {
        qCritical() << Q_FUNC_INFO << QString("[error!]: %1").arg(sqlQuery.lastError().text()) ;
        return false;
    }

    sqlQuery.bindValue(0, QVariant(zx_data_src.stu_obj_id.toInt()));
    sqlQuery.bindValue(1, QVariant(zx_data_src.stu_obj_type.toInt()));
    sqlQuery.bindValue(2, QVariant(zx_data_src.stu_item_id.toInt()));
    sqlQuery.bindValue(3, QVariant(zx_data_src.stu_data_define.toInt()));
    sqlQuery.bindValue(4, QVariant(zx_data_src.stu_rcvtime));
    sqlQuery.bindValue(5, QVariant(zx_data_src.stu_f_phase_temper));
    sqlQuery.bindValue(6, QVariant(QVariant::String));
    sqlQuery.bindValue(7, QVariant(QVariant::Int));
    sqlQuery.bindValue(8, QVariant(QVariant::String));
    sqlQuery.bindValue(9, QVariant(QVariant::String));

    bool bRet = sqlQuery.exec();
    if(!bRet)
    {
        qCritical() << Q_FUNC_INFO << QString("[insert error!]: %1").arg(sqlQuery.lastError().text());
        return false;
    }

    return true;
}


bool  QDataOperator::Insert_Into_Zx_Data_Src(const ZX_DATA_SRC &zx_data_src)
{
    QSqlQuery sqlQuery(db);

    QString strSql;
    strSql = "INSERT INTO zx_data_src( OBJ_ID, OBJ_TYPE," \
             " ITEM_ID, DATADEF, RCVTIME, DATA)" \
             " VALUES(?, ?," \
             " ?, ?, ?, ?)";

    if(!sqlQuery.prepare(strSql))
    {
        qCritical() << Q_FUNC_INFO << QString("[error!]: %1").arg(sqlQuery.lastError().text()) ;
        return false;
    }

    sqlQuery.bindValue(0, QVariant(zx_data_src.stu_obj_id.toInt()));
    sqlQuery.bindValue(1, QVariant(zx_data_src.stu_obj_type.toInt()));
    sqlQuery.bindValue(2, QVariant(zx_data_src.stu_item_id.toInt()));
    sqlQuery.bindValue(3, QVariant(zx_data_src.stu_data_define.toInt()));
    sqlQuery.bindValue(4, QVariant(zx_data_src.stu_rcvtime));
    sqlQuery.bindValue(5, QVariant(zx_data_src.stu_f_phase_temper));

    bool bRet = sqlQuery.exec();
    if(!bRet)
    {
        qCritical() << Q_FUNC_INFO << QString("[insert error!]: %1").arg(sqlQuery.lastError().text());
        return false;
    }

    return true;

}

bool  QDataOperator::Update_Zx_Data_Src(const ZX_DATA_SRC &zx_data_src)
{
    QSqlQuery sqlQuery(db);

    QString strSql;
    strSql = QString("update zx_Data_src set " \
            " RCVTIME=:RCVTIME, DATA=:DATA " \
            " where OBJ_ID = %1 and OBJ_TYPE = %2 and ITEM_ID = %3 and DATADEF = %4 ")
            .arg(zx_data_src.stu_obj_id.toInt()).arg(zx_data_src.stu_obj_type.toInt())
            .arg(zx_data_src.stu_item_id.toInt()).arg(zx_data_src.stu_data_define.toInt());

    if(!sqlQuery.prepare(strSql))
    {
        qCritical() << Q_FUNC_INFO << QString("[error!]: %1").arg(sqlQuery.lastError().text()) ;
        return false;
    }

    sqlQuery.bindValue(":RCVTIME", QVariant(zx_data_src.stu_rcvtime));
    sqlQuery.bindValue(":DATA", QVariant(zx_data_src.stu_f_phase_temper));

    bool bRet = sqlQuery.exec();
    if(!bRet)
    {
        qCritical() << Q_FUNC_INFO << QString("[update error!]: %1").arg(sqlQuery.lastError().text());
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////

