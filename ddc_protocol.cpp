#include "ddc_protocol.h"

namespace ddc {

DDCProtocol_T::~DDCProtocol_T()
{
}

void DDCProtocol_T::setSlaveAddr(quint8 slaveaddr)
{
    m_slaveaddr = slaveaddr;
}

quint8 DDCProtocol_T::getchecksum(quint8 *data,quint8 datalen)
{
    if (data == NULL) return false;
    quint8 checksum=m_slaveaddr;
    for (int i = 0; i < datalen; i++)
    {
        checksum^=*data++;
    }
    return checksum;
}

bool DDCProtocol_T::write(quint8* data,quint8 len)
{
    quint8 packagelen = len+4;
    quint8 *package = new quint8[len + 4];//slaveaddr + 0x51 + m_packlen + package + checksum

    package[0] = m_slaveaddr;
    package[1] = kddcSourceAddr;
    package[2] = len | 0x80;

    memcpy(package+3,data,len);

    package[packagelen-1] = getchecksum(package,packagelen-1);

    m_i2c.write(m_i2c.gethandle(),m_slaveaddr,package,packagelen);

    delete package;
    return true;
}

bool DDCProtocol_T::read(quint8* buf,quint8 len)
{
    for (int i = 0; i < len; i++)
    {
        m_i2c.read(m_i2c.gethandle(), m_slaveaddr, &buf[i], 1);
        qDebug("feedback:0x%x", buf[i]);
    }
    return true;
}

}
