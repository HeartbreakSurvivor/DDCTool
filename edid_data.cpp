#include "edid_data.h"

namespace ddc {

Edid_T::~Edid_T()
{
}

Edid_T::Edid_T(Edid_T& edid_t)
{
    size = edid_t.getLength();
    data = new quint8[size];
    memcpy(data, edid_t.data, size*sizeof(quint8));

    qDebug("Copy the memory1 ");
}

Edid_T& Edid_T::operator=(Edid_T& edid_t)
{
    if (this == &edid_t)
        return *this;

    size = edid_t.getLength();
    data = new quint8[size];
    memcpy(data, edid_t.data, size * sizeof(quint8));

    qDebug("Copy the memory2");
    return *this;
}

QString Edid_T::getManufacturerName(void)
{
    quint16 Tmpname = data[8] << 8 | data[9];//*(unsigned short *)&(data[8]);//little endian and this way is wrong.
    for (int i = 0; i < 3; ++i)
    {
        quint16 pletter = (Tmpname >> (i * 5) )& 0x1f;
        if (0x00001 <= pletter&&pletter <= 0x11010)
        {
            pletter += 0x40;
            m_manufacturername.insert(0,(char)pletter);
        }
        //qDebug("pletter:%c, %d", pletter,i);
    }
    return m_manufacturername;
}

QString Edid_T::getVersion(void)
{
    quint8 version=data[18],rversion=data[19];
    m_version.insert(0,rversion);
    m_version.insert(0,'.');
    m_version.insert(0,version);
    m_version.insert(0,'v');

    qDebug()<<m_version;

    return m_version;
}

quint16 Edid_T::getProductCode(void)
{
    m_productcode = *(quint16*)&(data[10]);
    return m_productcode;
}

quint16 Edid_T::getProductYear(void)
{
    m_productyear = (quint16)(data[17]+1990);
    return m_productyear;
}

quint8 Edid_T::getProductWeek(void)
{
    m_productweek = data[16];
    return m_productweek;
}

QString Edid_T::getProductSN(void)
{
    quint8 snaddr = findProductSn();
    int i=0;
    if(!snaddr) return 0;
    snaddr+=5;

    for((*(data+snaddr+13)==0x0A)?(i=12):(i=13);i>0; --i)
    {
        const char tmp = *(data+snaddr+i);
        if(tmp>=0x30 && tmp<=0x39)
        {
            m_productsn.insert(0,tmp-0x30);
        }
        else if(tmp>=0x41 && tmp<=0x46)
        {
            m_productsn.insert(0,tmp-0x41);//65
        }
        else if((tmp>=0x61) && (tmp<=0x66))
        {
            m_productsn.insert(0,tmp-0x61); //0x57
        }
    }
    qDebug("product sn:%s", m_productsn);
    return m_productsn;
}

quint32 Edid_T::getManufacturerSN(void)
{
    m_manufacturesn = data[12]<<24|data[13]<<16|data[14]<<8|data[15];
    return m_manufacturesn;
}

quint8 Edid_T::findProductSn()
{
    quint8 addr = 0x48;
    quint8 Data[5] = {0x00,0x00,0x00,0xFF,0x00};

    if (!memcmp(data + addr, Data, sizeof(Data))) return addr;
    else if (!memcmp(data + addr + 18, Data, sizeof(Data))) return addr+18;
    else if (!memcmp(data + addr + 36, Data, sizeof(Data))) return addr+36;
    else return 0;
}

void Edid_T::setManufacturerName(QString name)
{

}

void Edid_T::setProductYear(quint8 year)
{

}

void Edid_T::setProductWeek(quint8 week)
{

}

void Edid_T::setProductCode(quint16 productcode)
{

}

void Edid_T::setProductSN(QString sn)
{

}

void Edid_T::setManufacturerSN(quint32 manufacturersn)
{

}

void Edid_T::getdata(int offset, int bufsize, unsigned char *buf, int Rlen)
{
    if (data == NULL) return;
    if (offset*bufsize >= size) return;
    memcpy((void*)buf, (void *)(data + offset*bufsize), Rlen);
}

}
