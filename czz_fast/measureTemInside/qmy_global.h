#ifndef QMY_GLOBAL_H
#define QMY_GLOBAL_H

#include <QDebug>
#include <QDateTime>

#include "qreadini_config.h"

#define LightFiberTemp_Client_CONFIG_DIR "./config"
#define LightFiberTemp_Client_CONFIG_FILE LightFiberTemp_Client_CONFIG_DIR"/LightFibre_Ip_info.ini"
#define LightFiberTemp_Client_LOG_DIR "./log"
#define LightFiberTemp_Client_LOG_FILE_PATH LightFiberTemp_Client_LOG_DIR"/LightFibre_temp.txt"


typedef struct _ZX_DATA_SRC
{
    QString stu_obj_id;
    QString stu_obj_type;
    QString stu_item_id;   //参数ID
    QString stu_data_define;     //是遥测还是遥信 等 数值0

    float stu_f_phase_temper;    //分相温度
    QDateTime stu_rcvtime;

}ZX_DATA_SRC;

extern QString g_dataBase_ip;
extern unsigned short g_dataBase_port;
extern QString g_global_logon_db_name;
extern QString g_global_logon_db_user;
extern QString g_global_logon_db_pass;


class QReadIni_config_info;
extern QReadIni_config_info g_get_global_ip_port_info;

class QMy_Global
{
public:
    QMy_Global();
};

#endif // QMY_GLOBAL_H
