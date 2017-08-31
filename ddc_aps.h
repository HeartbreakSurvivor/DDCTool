#ifndef DDC_APS_H
#define DDC_APS_H

#include "cvt_global.h"
#include "ddc_protocol.h"

namespace ddc {

typedef struct burncmd{
    quint8 *burndata;
    quint8 datalen;
    quint8 feedbacklen;
    quint8 retrycnt;

    cbverify verifyfunc;
    cbassemble assemblefunc;
}burnCmd_t;

extern burnCmd_t hdcpcmd;
}

#endif // DDC_APS_H
