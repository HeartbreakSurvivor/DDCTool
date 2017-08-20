#ifndef EDID_PARSE_H
#define EDID_PARSE_H

#include "burn_data.h"

namespace ddc {

class Edid_T:public BurnData_T
{
public:
    Edid_T() {}

    //copy constructor function
    Edid_T(Edid_T&);
    Edid_T& operator=(Edid_T&);

    ~Edid_T();

    QString getManufacturerName(void);
    QString getVersion(void);
    quint8 getProductYear(void);
    quint8 getProductWeek(void);
    quint16 getProductCode(void);
    void getProductSN(void);
    quint32 getManufacturerSN(void);

    void setManufacturerName();
    void setProductYear(quint8 year);
    void setProductWeek(quint8 week);
    void setProductCode(quint16 productcode);
    void setProductSN(quint8 *buf,int len);
    void setManufacturerSN(quint32 manufacturersn);

    quint8 findProductSn(void);
    bool setProductSN(void);

    virtual bool getdata(int offset, int bufsize, unsigned char *buf, int Rlen);
    virtual bool setdata(int offset, int bufsize, unsigned char *buf, int Rlen);

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
