#pragma once
#include <QObject>
#include "cvt_global.h"
#include "isp_iic.h"

namespace ddc {

class DDCProtocol_T : public QObject
{
    Q_OBJECT
    static const quint8 kddcSourceAddr = 0x51;

public:
    //The reference must be initialized at the time of the definition,and can't be reassigned.
    DDCProtocol_T(Isp_I2C& i2c):m_i2c(i2c){
           qDebug("Initialize the Protocol ");
    }
    virtual ~DDCProtocol_T();

    void setSlaveAddr(quint8 slaveaddr);

    bool write(quint8* data,quint8 len,bool c);
    bool read(quint8* buf,quint8 len,bool c);

signals:
    void start_emit(QString msg);

private:
    quint8 getchecksum(quint8 *data,quint8 datalen);

private:
    quint8 m_slaveaddr;
    Isp_I2C &m_i2c;
};

}
