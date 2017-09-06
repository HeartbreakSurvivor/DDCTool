#ifndef DDC_APS_H
#define DDC_APS_H

#include "cvt_global.h"

namespace ddc {

using cbverify = bool(*)(quint8 *feedback,quint8 fdlen,quint8 *data,quint8 len);
using cbassemble = burndata_t (*)(quint8 *head,quint8 headsize,quint8 *body,quint8 bodysize);

typedef enum {
    kNull,
    kSPinbox,
    kLineEdit,
}ParaStyle_t;

typedef struct burncmd{
    //ui relate
    QString name;
    QString description;
    ParaStyle_t parastyle;

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
    quint32 lastpackdelay;
}burnCmd_t;


//external declarations
extern burnCmd_t enterATcmd;
extern burnCmd_t setSourcecmd;
}

#endif // DDC_APS_H
