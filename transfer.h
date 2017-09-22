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
    Transfer_T(DDCProtocol_T& protocol,quint8 spretry):m_protocol(protocol),m_spretrycnt(spretry){}
    ~Transfer_T(){}

    void setburnCmd(burnCmd_t *burnmsg,quint8 source);//just set the burncmd readyto to write to device.
    void setburnCmd(burnCmd_t *burnmsg,quint8 *data, quint32 size,quint8 source);//set the burn head or the whole package.
    void setburndata(quint8* data,quint32 size);//set the burn data body and body size.

    bool transfermultipackage();
    bool transferpackage();

	void run();

private:
    quint8* m_databody=nullptr;
    quint32 m_bodysize=0;
    burnCmd_t* m_burnmsg;//include the head data or the whole data

    quint8 m_spretrycnt;//the singlae package retry mechanism.
    quint8 m_source=0;

    DDCProtocol_T& m_protocol;//must be intialize in constructor function
};

}

