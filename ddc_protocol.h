#pragma once
#include "cvt_global.h"
#include "isp_iic.h"

namespace ddc {

class DDCProtocol_T
{
    static const quint8 kddcSourceAddr = 0x51;

public:
    //The reference must be initialized at the time of the definition,and can't be reassigned.
    DDCProtocol_T(Isp_I2C& i2c):m_i2c(i2c){
           qDebug("Initialize the Protocol ");
    }
    ~DDCProtocol_T();

    void setSlaveAddr(quint8 slaveaddr);

    bool write(quint8* data,quint8 len);
    bool read(quint8* buf,quint8 len);

private:
    quint8 getchecksum(quint8 *data,quint8 datalen);

private:
    quint8 m_slaveaddr;
    Isp_I2C &m_i2c;
};

}
