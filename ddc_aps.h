#ifndef DDC_APS_H
#define DDC_APS_H

#include "cvt_global.h"
#include "ddc_protocol.h"

namespace ddc {

typedef struct
{
    unsigned char burnhead[7];
    unsigned char readhead[8];
}Cvt_Burningcmd;

typedef struct burnmsg{
    quint8 *burndata;
    quint8 datalen;
    quint8 feedbacklen;

    cbverify verifyfunc;
    cbassemble assemblefunc;
}burnMsg_t;

extern burnMsg_t hdcpmsg;
}

#endif // DDC_APS_H
