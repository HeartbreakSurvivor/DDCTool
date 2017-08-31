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
    Transfer_T(DDCProtocol_T& protocol,burnCmd_t* burnmsg,quint8 retrycnt,quint8 writedelay);
    Transfer_T(DDCProtocol_T& protocol,burnCmd_t* burnmsg,quint8 retrycnt,quint8 writedelay,int edidlpdelay);
    Transfer_T(DDCProtocol_T& protocol,burnCmd_t* burnmsg,quint8 retrycnt,quint8 writedelay,int erasehdcpdelay,int hdcplpdelay);
    ~Transfer_T(){}

    void setburnCmd(burnCmd_t* burnmsg);
    void setdelayandretry(quint8 retrycnt,quint8 writedelay);
    bool transferpackage();
	void run();

private:
    quint8 m_retrycnt;
    int m_edidlpdelay=-1;
    int m_erasedelay=-1;
    int m_hdcplpdelay=-1;

    quint8* m_data2send;
    int m_datasize;

    ddc::cbassemble m_assemblefunc;

    DDCProtocol_T& m_protocol;//must be intialize in constructor function
};

}

