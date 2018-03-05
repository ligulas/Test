#include "qreadini_config.h"

QReadIni_config_info::QReadIni_config_info()
{
    m_global_ip_info.clear();
    m_global_obj_id_MapInfo.clear();

    QDir dirName(LightFiberTemp_Client_CONFIG_DIR);
    if(!dirName.exists())
    {
        dirName.mkpath(LightFiberTemp_Client_CONFIG_DIR);
    }

    QFile new_config_info_file(LightFiberTemp_Client_CONFIG_FILE);
    if(!new_config_info_file.exists())
    {
        qCritical() << QString("%1 not exist ,Now Create!").arg(LightFiberTemp_Client_CONFIG_FILE);

        //初始化ini 文件
        init_global_ip_port_info();
    }



}


QReadIni_config_info::~QReadIni_config_info()
{

}

void QReadIni_config_info::init_global_ip_port_info()
{
    QSettings ServerInfoSettings(LightFiberTemp_Client_CONFIG_FILE, QSettings::IniFormat, 0);
    ServerInfoSettings.setIniCodec("GB2312");

    if(!QFile::exists(LightFiberTemp_Client_CONFIG_FILE))
    {
        //不存在则初始化一个值
        set_new_ip_port_serverInfo("127.0.0.1",50005,100,ServerInfoSettings);

        ServerInfoSettings.beginGroup("DataBase_info");

        ServerInfoSettings.setValue("db_ip", "127.0.0.1");
        ServerInfoSettings.setValue("db_port", 3306);
        ServerInfoSettings.setValue("db_name", "sddl");
        ServerInfoSettings.setValue("db_user", "hoshing");
        ServerInfoSettings.setValue("db_pass", "hoshingpwd");

        ServerInfoSettings.endGroup();
    }
    else
    {
        //如果已经存在，则读取
        int size = ServerInfoSettings.beginReadArray("ServerInfo");
        for (int i = 0; i < size; ++i) {
            ServerInfoSettings.setArrayIndex(i);
            m_global_ip_info.insert(ServerInfoSettings.value("ip").toString(),
                                    ServerInfoSettings.value("port").toInt());

            m_global_obj_id_MapInfo.insert(ServerInfoSettings.value("ip").toString(),
                                           ServerInfoSettings.value("obj_id").toInt());
        }
        ServerInfoSettings.endArray();

        qDebug() << Q_FUNC_INFO << QString(" Totally get ServerInfo count:%1").arg(m_global_ip_info.count()) ;

        //如果已经存在，则读取
        ServerInfoSettings.beginGroup("DataBase_info");

        g_dataBase_ip = ServerInfoSettings.value("db_ip",QString("127.0.0.1")).toString();
        g_dataBase_port = ServerInfoSettings.value("db_port",3306).toInt();
        g_global_logon_db_name = ServerInfoSettings.value("db_name",QString("sddl")).toString();
        g_global_logon_db_user = ServerInfoSettings.value("db_user",QString("hoshing")).toString();
        g_global_logon_db_pass = ServerInfoSettings.value("db_pass",QString("hoshingpwd")).toString();

        ServerInfoSettings.endGroup();

        qDebug() << Q_FUNC_INFO << QString(" init database_info success: \n"
                                           " db_ip=%1\n "
                                           " db_port=%2\n "
                                           " db_name=%3\n "
                                           " db_user=%4\n "
                                           " db_pass=%5 ")
                    .arg(g_dataBase_ip).arg(g_dataBase_port).arg(g_global_logon_db_name).arg(g_global_logon_db_user).arg(g_global_logon_db_pass) ;
    }
}

void QReadIni_config_info::set_new_ip_port_serverInfo(const QString &ip_qstr,const int port,const int obj_id,QSettings &setting_file_toAdd)
{
    m_global_ip_info.insert(ip_qstr,port);
    m_global_obj_id_MapInfo.insert(ip_qstr,obj_id);

    int size = setting_file_toAdd.beginReadArray("ServerInfo");
    setting_file_toAdd.endArray();

    setting_file_toAdd.beginWriteArray("ServerInfo");

    setting_file_toAdd.setArrayIndex(size);
    setting_file_toAdd.setValue("ip", ip_qstr);  //向ini文件的第一个节写入内容,ip节下的第一个参数
    setting_file_toAdd.setValue("port", port);   //向ini文件的第一个节写入内容,port节下的第二个参数
    setting_file_toAdd.setValue("obj_id", obj_id); //向ini文件的第一个节写入内容,obj_id节下的第三个参数

    setting_file_toAdd.endArray();
}


void QReadIni_config_info::set_Ip_mapInfo(QMap<QString,int> insertMap_info)
{
    m_global_ip_info = insertMap_info;
}

QMap<QString,int> QReadIni_config_info::get_Ip_mapInfo()
{
    return m_global_ip_info;
}


QMap<QString,int> QReadIni_config_info::get_obj_id_MapInfo()
{
    return m_global_obj_id_MapInfo;
}
