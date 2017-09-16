#ifndef DDC_APS_H
#define DDC_APS_H

#include "cvt_global.h"

namespace ddc {

#define     COUNTOF(array)       (sizeof(array)/sizeof(array[0]))

using cbverify = bool(*)(quint8 *feedback,quint8 fdlen,quint8 *data,quint8 len);
using cbassemble = burndata_t (*)(quint8 *head,quint8 headsize,quint8 *body,quint8 bodysize);
using setpara = void (*)(QString text,quint8 *head,quint8 headsize);

typedef enum {
    kNull,
    kSPinbox,
    kLineEdit,
}ParaStyle_t;

typedef struct burncmd{
#if 0
    burncmd()
    {
        name = nullptr;
        description = nullptr;
        setparafunc = nullptr;
        burndata = nullptr;
        datalen = 0;
        assemblefunc = nullptr;

        feedbacklen = 0;
        verifyfunc = nullptr;

        retrycnt = 1;
        delay = 0;
        lastpackdelay = 0;
        qDebug()<<"initialize the burncmd";
    }
#endif
    //ui relate
    QString name;
    QString description;
    setpara setparafunc;

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


extern burnCmd_t* ATCmds[];
extern int getATCmdLen(void);

}

#endif // DDC_APS_H
