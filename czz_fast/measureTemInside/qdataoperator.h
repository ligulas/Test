#ifndef QDATAOPERATOR_H
#define QDATAOPERATOR_H

#include <QSqlError>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMap>
#include <QVector>
#include <QList>

#include "qmy_global.h"

class QDataOperator
{
public:
    QDataOperator();
    ~QDataOperator();

public:
    bool OpenDataBase(QString connect_name="default");

public:
    bool OpenDataBase_MYSQL(QString strDbName, QString strIP, int nPort,
                                         QString strUserName, QString strpassword, QString name="default");
    bool OpenDataBase_Sql(QString strDbName, QString strIP, int nPort,
                                         QString strUserName, QString strpassword, QString name="default");

public:
    bool IsOpen();

public:

    int Select_Count_at_Table_Condition(const QString &tableName,const QString &whereCondition);

    //操作函数集合
    bool Insert_Into_Lx_Data_Src_Batch(const QList<ZX_DATA_SRC> &lx_data_src_list);    //批量
    bool Insert_Into_Lx_Data_Src(const ZX_DATA_SRC &zx_data_src);
    bool Insert_Into_Zx_Data_Src(const ZX_DATA_SRC &zx_data_src);
    bool Update_Zx_Data_Src(const ZX_DATA_SRC &zx_data_src);


public:
    QSqlDatabase db;
    QString m_strHostName;
    QString m_strDbName;
    QString m_strUserName;
    QString m_strPassword;
    int m_port;
};

#endif // QDATAOPERATOR_H
