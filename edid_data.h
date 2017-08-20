#ifndef EDID_PARSE_H
#define EDID_PARSE_H

#include "burn_data.h"

class BurnData_T;

namespace ddc {

class Edid_T:public BurnData_T
{
public:
    Edid_T();
    Edid_T(const QString &filepath):BurnData_T(filepath){
         qDebug()<<"Edid_T constructor";
    }

    //copy constructor function
    Edid_T(const Edid_T&);
    Edid_T& operator=(Edid_T&);

    ~Edid_T();

    QString getManufacturerName(void);
    QString getVersion(void);
    quint16 getProductYear(void);
    quint8 getProductWeek(void);
    quint16 getProductCode(void);
    QString getProductSN(void);
    quint32 getManufacturerSN(void);

    void setManufacturerName(QString name);
    void setProductYear(quint8 year);
    void setProductWeek(quint8 week);
    void setProductCode(quint16 productcode);
    void setProductSN(QString sn);
    void setManufacturerSN(quint32 manufacturersn);

    quint8 findProductSn(void);

    virtual void getdata(int offset, int bufsize, unsigned char *buf, int Rlen);

private:
    QString m_manufacturername;
    QString m_version;
    quint16 m_productyear;
    quint8 m_productweek;
    quint16 m_productcode;
    QString m_productsn;
    quint32 m_manufacturesn;

};

}


#endif // EDID_PARSE_H
