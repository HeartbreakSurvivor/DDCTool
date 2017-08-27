#include "ddc_protocol.h"

namespace ddc {

DDCProtocol_T::~DDCProtocol_T()
{
}

void DDCProtocol_T::setSlaveAddr(quint8 slaveaddr)
{
    m_slaveaddr = slaveaddr;
}

void DDCProtocol_T::setRetrycnt(quint8 retrycnt)
{
    m_spretrycnt = retrycnt;
}

void DDCProtocol_T::setwritedelay(quint8 writedelay)
{
    m_writedelay = writedelay;
}

void DDCProtocol_T::setfeedbacklen(quint8 fdlen)
{
    m_feedbacklen = fdlen;
}
void DDCProtocol_T::setcbfunc(cbverify verifyfunc)
{
    m_verifyfunc = verifyfunc;
}

void DDCProtocol_T::setPackdata(quint8 *data,quint8 datalen)
{
    if (m_packdata!=nullptr)
    {
        delete m_packdata;
    }

    m_packdatalen = datalen;
    m_packagelen = m_packdatalen + 4;//slaveaddr + 0x51 + m_packlen + m_packdata + checksum
    m_packdata = new quint8[m_packagelen];

    m_packdata[0] = m_slaveaddr;
    m_packdata[1] = kddcSourceAddr;
    m_packdata[2] = m_packdatalen | 0x80;

    memcpy(m_packdata+3,data,datalen);

    m_packdata[m_packagelen-1] = getchecksum(m_packdata,m_packagelen-1);
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

bool DDCProtocol_T::burn(void)
{
    quint8* feedback = new quint8[m_feedbacklen];

    qDebug("writedelay:%d",m_writedelay);
    for (int i = 0; i < m_packagelen; i++)
    {
        qDebug("cmd data:0x%x", m_packdata[i]);
    }

    for (int j = 0; j < m_spretrycnt; j++)//The single pack retry mechanism.
    {
        m_i2c.write(m_i2c.gethandle(),m_slaveaddr,m_packdata,m_packagelen);
        Sleep(m_writedelay);//write delay
        for (int i = 0; i < m_feedbacklen; i++)
        {
            m_i2c.read(m_i2c.gethandle(), m_slaveaddr, &feedback[i], 1);
            qDebug("feedback:0x%x", feedback[i]);
        }

        /* the condition of burn success must be treated differently
        use the callback function register mechanism,the burn success comdition is specified by user*/

        /*remember the delete the feedback data*/
        if(this->m_verifyfunc(m_packdata,feedback))//if (feedback[7] == 0xE0 /*&& memcmp(Cmd_Burnhead + 2, feedback + 2, 5)*/)
        {
            qDebug("Burn success!");
            break;
        }
        else
        {
            if (j = m_spretrycnt - 1)
            {
                qDebug("Burn failed!");
                delete feedback;
                return false;
            }
            qDebug("Single pack retry!");
        }
    }
    delete feedback;
    return true;
}

}
