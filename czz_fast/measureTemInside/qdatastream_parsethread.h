#ifndef QDATASTREAM_PARSETHREAD_H
#define QDATASTREAM_PARSETHREAD_H

#include <QDateTime>
#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QMutex>

#define DATA_CONTAINER_MAXSIZE 4096
#define RECEIVE_PACKAGE_LENGTH 11    //一条应答的命令长度为11
typedef unsigned char BYTE;

typedef struct _FIBRE_TEMP_STRUC
{
    float phase_A_temp;
    float phase_B_temp;
    float phase_C_temp;
    QDateTime recv_time;

}FIBRE_TEMP_STRUC;

class QSocketThread;

class QDataStream_ParseThread:public QThread
{
    Q_OBJECT
public:
    QDataStream_ParseThread(const QString& server_addr= "127.0.0.1",const int portNum = 50005,QObject *parent = 0);
    ~QDataStream_ParseThread();

    static void get_constant_modbus_cmd(QString &command_getData);
    void dynamic_construct_modbus_cmd(const char byte_address,QString &command_getData);

    QMap<QDateTime,FIBRE_TEMP_STRUC> get_parsed_data_list();

    static unsigned char* strToHexByte(std::string strMsg,unsigned char *pResultBuf,unsigned long nResultBufLength);
    static std::string HexToStr(unsigned char * cGPSDataBuf,int length);
    static int strHexToInt(const char* strSource);

    static bool crcCheck(unsigned char *cbuf,int iLen);
protected:
    void run();

signals:
    void sig_container_size_notifier(QString,int);
    void sig_Parse_data_info(QString,float,float,float,QDateTime);

private slots:
    void on_deal_dataStream_fromCommunicator();

private:

    static std::string crc_calc(unsigned char * pushMsg,unsigned usDataLen);
    static void crc_kernel_check(unsigned char crc_data,unsigned int &crc_result); // crc_data is the number of check

    void parse_dataStream(const QByteArray& float_info,const QDateTime &dateTime_recv);
private:
    QTimer *m_parseCmd_timer;

    QMutex m_readyData_container_mutex;
    QMap<QDateTime,FIBRE_TEMP_STRUC> m_data_container_map;

    QSocketThread *m_communicate_dealer;

    QString m_ip_flag;
};

#endif // QDATASTREAM_PARSETHREAD_H
