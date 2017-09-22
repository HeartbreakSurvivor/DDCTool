#ifndef BURN_DATA_H
#define BURN_DATA_H

#include "cvt_global.h"

namespace ddc {

class BurnData_T
{
public:
    BurnData_T();
    BurnData_T(const QString &filepath);
    virtual ~BurnData_T();

    int getLength(void) const;
    QString getfilename(void) const;

    void getdata(int offset, int bufsize, quint8 *buf, int Rlen){
        if (data == NULL) return;
        if (offset*bufsize >= size) return;
        memcpy((void*)buf, (void *)(data + offset*bufsize), Rlen);
    }
    virtual void setdata(int offset, int bufsize, quint8 *buf, int Rlen){
        Q_UNUSED(offset);
        Q_UNUSED(bufsize);
        Q_UNUSED(buf);
        Q_UNUSED(Rlen);
    }
    void syncfile();

public:
    QFile *file;
    QString filename;
    quint8 *data;
    quint64 size = 0;//the size of data read from the file

};

}
#endif // BURN_DATA_H
