#ifndef DDC_APS_H
#define DDC_APS_H

#include "cvt_global.h"

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
    ddc::cbverify verifyfunc;
    ddc::cbassemble assemblefunc;
}burnMsg_t;

}

#endif // DDC_APS_H
