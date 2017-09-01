#pragma once

#include "cvt_global.h"
#include "ddc_protocol.h"
#include "isp_iic.h"
#include "ddc_aps.h"

namespace ddc {

class Transfer_T : public QThread
{
    //Q_OBJECT
public:
    Transfer_T(DDCProtocol_T& protocol):m_protocol(protocol){}
    ~Transfer_T(){}

    //set the burn control msg (burn head msg and flow control)
    void setburnCmd(burnCmd_t* burnmsg);
    //set the burn data(edid&hdcp&sn data)
    void setburndata(quint8* data,quint32 size);
    bool transferpackage();
	void run();

private:
    burnCmd_t* m_burnmsg;//include the head data or the whole data

    quint8* m_databody;
    qint32 m_bodysize;
    ddc::cbassemble m_assemblefunc;

    quint8 m_retrycnt;

    int m_edidlpdelay=-1;
    int m_erasedelay=-1;
    int m_hdcplpdelay=-1;

    DDCProtocol_T& m_protocol;//must be intialize in constructor function
};

}

