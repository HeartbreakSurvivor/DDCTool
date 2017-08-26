#pragma once
#include "cvt_global.h"
#include "isp_iic.h"


namespace ddc {

class DDCProtocol_T
{
public:
    //The reference must be initialized at the time of the definition,and can't be reassigned.
    DDCProtocol_T(Isp_I2C& i2c):m_i2c(i2c){
           qDebug("Initialize the Protocol ");
    }
    ~DDCProtocol_T();

    void setSlaveAddr(quint8 slaveaddr);
    void setPackdata(quint8 *data,quint8 datalen);
    void setRetrycnt(quint8 retrycnt);
    void setwritedelay(quint8 writedelay);
    void setfeedbacklen(quint8 fdlen);
    bool burn();

    static const quint8 kddcSourceAddr = 0x51;

private:
    quint8 getchecksum(quint8 *data,quint8 datalen);

private:
    quint8 m_slaveaddr;
    quint8 m_packdatalen;
    quint8 *m_packdata=NULL;//data ready to transfer.
    quint8 m_packagelen;

    quint8 m_writedelay;
    quint8 m_feedbacklen;
    quint8 m_spretrycnt;
    Isp_I2C &m_i2c;
};

}
