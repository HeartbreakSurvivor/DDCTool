#ifndef BURN_DATA_H
#define BURN_DATA_H

#include "cvt_global.h"

namespace ddc {

class BurnData_T
{
public:
    BurnData_T();
    BurnData_T(const QString &filepath);
    ~BurnData_T();

    int getLength(void) const;
    QString getfilename(void) const;

    virtual void getdata(int offset, int bufsize, quint8 *buf, int Rlen){}
    virtual void setdata(int offset, int bufsize, quint8 *buf, int Rlen){}

public:
    QFile *file;
    QString filename;
    quint8 *data;
    quint64 size = 0;//the size of data read from the file

};

}
#endif // BURN_DATA_H