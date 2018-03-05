#include "qdatastream_parsethread.h"
#include "qsocketthread.h"

QDataStream_ParseThread::QDataStream_ParseThread(const QString& server_addr, const int portNum, QObject *parent)
{

    m_communicate_dealer = new QSocketThread();
    m_communicate_dealer->set_Server_ip_port(server_addr,portNum);    //初始化IP 与 端口
    m_ip_flag = server_addr;

    m_data_container_map.clear();
}

QDataStream_ParseThread::~QDataStream_ParseThread()
{
    if(m_parseCmd_timer != NULL)
    {
        if(m_parseCmd_timer->isActive())
        {
            m_parseCmd_timer->stop();
        }

        delete m_parseCmd_timer;
        m_parseCmd_timer = NULL;
    }

    if(m_communicate_dealer != NULL)
    {
        delete m_communicate_dealer;
        m_communicate_dealer = NULL;
    }

    quit();
    wait();
}

void QDataStream_ParseThread::get_constant_modbus_cmd(QString &command_getData)
{
    BYTE t_buf[8] = {0};

    //modbus 协议
    t_buf[0] = 0x01;  //地址码 写死
    t_buf[1] = 0x03;  //功能码
    t_buf[2] = 0x01;  //起始地址高位
    t_buf[3] = 0x00;  //起始地址低位

    //估计这两位是用来向下方要数据个数
    t_buf[4] = 0x00;  //寄存器地址高位
    t_buf[5] = 0x03;  //寄存器地址低位

    //校验位 2位
    t_buf[6] = 0x04;
    t_buf[7] = 0x37;

    //检查一下
    if(!crcCheck(t_buf,8))
    {
        qCritical() << Q_FUNC_INFO << QString(" Contruct cmd CRC Check error,what's wrong? ");
        command_getData = "error";
    }

    command_getData = QString::fromStdString(HexToStr(t_buf,8));
}

void QDataStream_ParseThread::dynamic_construct_modbus_cmd(const char byte_address, QString &command_getData)
{
    BYTE t_buf[8] = {0};

    //modbus 协议
    t_buf[0] = byte_address;  //地址码 灵活
    t_buf[1] = 0x03;  //功能码
    t_buf[2] = 0x01;  //起始地址高位
    t_buf[3] = 0x00;  //起始地址低位

    //估计这两位是用来向下方要数据个数
    t_buf[4] = 0x00;  //寄存器地址高位
    t_buf[5] = 0x03;  //寄存器地址低位

    //动态 计算出 校验位 2位
    std::string CRCStr = crc_calc(((unsigned char *)t_buf),6);
    BYTE t_crc_2bits[4] = {0};
    strToHexByte(CRCStr,t_crc_2bits,2);

    t_buf[6] = t_crc_2bits[0];
    t_buf[7] = t_crc_2bits[1];

    command_getData = QString::fromStdString(HexToStr(t_buf,8));
}

void QDataStream_ParseThread::run()
{
    //先开启通信client
    if(!m_communicate_dealer->isRunning())
    {
        m_communicate_dealer->start();
    }

    m_parseCmd_timer = new QTimer();

    m_parseCmd_timer->setInterval(5000);    //5秒去解析一次队列中有的数据
    connect(m_parseCmd_timer,SIGNAL(timeout()),this,SLOT(on_deal_dataStream_fromCommunicator()));
    m_parseCmd_timer->start();

    exec();
}

QMap<QDateTime,FIBRE_TEMP_STRUC> QDataStream_ParseThread::get_parsed_data_list()
{
    QMap<QDateTime,FIBRE_TEMP_STRUC> ret_result;

    m_readyData_container_mutex.lock();
    ret_result = m_data_container_map;
    m_data_container_map.clear();
    m_readyData_container_mutex.unlock();

    return ret_result;

}


void QDataStream_ParseThread::on_deal_dataStream_fromCommunicator()
{
    if(m_communicate_dealer == NULL)
    {
        qCritical() << Q_FUNC_INFO << QString("impossible m_communicate_dealer == NULL,check!");
        return;
    }

    QList<RAW_DATA_AT_TIMESTAMP> dataStreamList = m_communicate_dealer->get_all_dataList_fromList();
    for(int iCnt=0; iCnt < dataStreamList.count() ; iCnt++)
    {
        RAW_DATA_AT_TIMESTAMP one_rec = dataStreamList.at(iCnt);

        parse_dataStream(one_rec.raw_data,one_rec.match_time);
    }
}

//生产出解析好的数据，准备给数据库存库
void QDataStream_ParseThread::parse_dataStream(const QByteArray& float_info,const QDateTime &dateTime_recv)
{
    FIBRE_TEMP_STRUC in_data_singleton;

    unsigned short uShort_A = -1;
    memcpy(&uShort_A,float_info.data()+3,2);
    uShort_A = ((uShort_A&0xFF00)>>8)|((uShort_A&0x00FF) << 8);

    in_data_singleton.phase_A_temp = (float)uShort_A / 100;

    unsigned short uShort_B = -1;
    memcpy(&uShort_B,float_info.data()+5,2);
    uShort_B = ((uShort_B&0xFF00)>>8)|((uShort_B&0x00FF) << 8);

    in_data_singleton.phase_B_temp = (float)uShort_B / 100;

    unsigned short uShort_C = -1;
    memcpy(&uShort_C,float_info.data()+7,2);
    uShort_C = ((uShort_C&0xFF00)>>8)|((uShort_C&0x00FF) << 8);

    in_data_singleton.phase_C_temp = (float)uShort_C / 100;

    in_data_singleton.recv_time = dateTime_recv;

    emit sig_Parse_data_info(m_ip_flag,
                             in_data_singleton.phase_A_temp,
                             in_data_singleton.phase_B_temp,
                             in_data_singleton.phase_C_temp,
                             dateTime_recv);

    m_readyData_container_mutex.lock();
    if(m_data_container_map.count() > DATA_CONTAINER_MAXSIZE)
    {
       qCritical() << Q_FUNC_INFO << QString(" [m_data_container_map size too Large>%1 ,clear!!]").arg(DATA_CONTAINER_MAXSIZE);
       m_data_container_map.clear();
    }
    m_data_container_map.insert(dateTime_recv,in_data_singleton);
    emit sig_container_size_notifier(m_ip_flag,m_data_container_map.count());    //通知前台这个解析列表里有多少数据
    m_readyData_container_mutex.unlock();
}


//检测CRC 校验码
bool QDataStream_ParseThread::crcCheck(unsigned char *cbuf,int iLen)
{
    std::string CRCStr = crc_calc((unsigned char *)(cbuf),iLen-2);
    std::string t_RetStr = HexToStr((unsigned char *)(cbuf+iLen-2),2);

    if(CRCStr == t_RetStr)
        return true;
    return false;
}

std::string QDataStream_ParseThread::crc_calc(unsigned char * pushMsg,unsigned usDataLen)
{
    std::string t_RetStr;
    unsigned char crc_data;
    unsigned int crc_result = 0xffff;
    for(int i=0 ;i<usDataLen; i++)
    {
        crc_data = pushMsg[i];
        crc_kernel_check(crc_data,crc_result);
    }

    unsigned short wcrc_result = crc_result;
    BYTE ReturnData[2];

    ReturnData[0] = wcrc_result;//CRC低位
    ReturnData[1] = wcrc_result >> 8;//CRC高位

    t_RetStr = HexToStr((unsigned char *)ReturnData,2);
    return t_RetStr;
}

#define CRC_CONSTANT 0xa001
void QDataStream_ParseThread::crc_kernel_check(unsigned char crc_data,unsigned int &crc_result)
{
    unsigned char xor_flag = 1;
    unsigned char m;
    unsigned int crc_num;
    crc_result^= crc_data;
    crc_num = crc_result;
    crc_num &= 0x0001;

    for(m=0;m<8;m++)
    {
        if(crc_num) xor_flag =1;
        else
            xor_flag = 0;
        crc_result >>=1;

        if(xor_flag) crc_result^=CRC_CONSTANT;
        crc_num = crc_result;
        crc_num &= 0x0001;
    }
}

unsigned char* QDataStream_ParseThread::strToHexByte(std::string strMsg,unsigned char *pResultBuf,unsigned long nResultBufLength)
{
    unsigned long i;
    std::string str = strMsg;
    char c[3];

    unsigned long length = str.length()/2 > nResultBufLength ? nResultBufLength:str.length()/2;
    unsigned char tmpStr;

    for(i=0; i<length ;i++)
    {
        c[0] = str.at(0);
        str.erase(0,1);
        c[1] = str.at(0);
        str.erase(0,1);
        c[2] = '\0';
        tmpStr = strHexToInt(c);
        pResultBuf[i] = tmpStr;
    }

    return pResultBuf;

}

int QDataStream_ParseThread::strHexToInt(const char* strSource)
{
    int nStrLength = strlen(strSource);
    if(nStrLength > 8)
        return 0;
    int nTmp , nResult , i;
    nTmp = nResult = i =0;
    for( ; i < nStrLength ; i++)
    {
        if(strSource[i] >= '0' && strSource[i] <= '9')
            nTmp = strSource[i] - '0';
        else if(strSource[i] >= 'a' && strSource[i] <= 'f')
            nTmp = strSource[i] - 'a' + 10;
        else if(strSource[i] >= 'A' && strSource[i] <= 'F')
            nTmp = strSource[i] - 'A' + 10;
        else
            nResult = 0;
        nTmp <<= ((nStrLength - i - 1) << 2);
         nResult |= nTmp;
    }
    return nResult;
}


std::string QDataStream_ParseThread::HexToStr(unsigned char * cGPSDataBuf,int length)
{
    std::string strRef = "";
    char str[3];

    memset(str,0,3*sizeof(char));
    for(int i=0; i<length ;i++)
    {
        sprintf(str,"%02X",cGPSDataBuf[i]);
        strRef += std::string(str);
    }

    return strRef;
}
