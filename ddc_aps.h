#ifndef DDC_APS_H
#define DDC_APS_H

#include "cvt_global.h"
#include "ddc_protocol.h"

namespace ddc {

typedef struct burncmd{
    //data to send and assemble function
    quint8 *burndata;
    quint8 datalen;
    cbassemble assemblefunc;
    //get feedback and verify
    quint8 feedbacklen;
    cbverify verifyfunc;
    //flow control parameter
    quint8 retrycnt;
    quint32 delay;
    quint32 lastdelay;
}burnCmd_t;

extern burnCmd_t hdcpcmd;
}

#endif // DDC_APS_H
