#ifndef QREADINI_CONFIG_H
#define QREADINI_CONFIG_H

#include <QDir>
#include <QSettings>
#include <QFile>

#include "qmy_global.h"

class QReadIni_config_info
{
public:
    QReadIni_config_info();
    ~QReadIni_config_info();

public:
    void init_global_ip_port_info();

    void set_Ip_mapInfo(QMap<QString,int> insertMap_info);
    QMap<QString,int> get_Ip_mapInfo();

    QMap<QString,int> get_obj_id_MapInfo();
private:
    void set_new_ip_port_serverInfo(const QString &ip_qstr, const int port, const int obj_id, QSettings &setting_file_toAdd);

private:
    QMap<QString,int> m_global_ip_info;
    QMap<QString,int> m_global_obj_id_MapInfo;
};

#endif // QREADINI_CONFIG_H
