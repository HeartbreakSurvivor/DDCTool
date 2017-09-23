#include "edid_data.h"

namespace ddc {

Edid_T::~Edid_T()
{
    //qDebug()<<"Edid_T destructor!";
}

Edid_T::Edid_T(const Edid_T& edid_t)
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
    m_manufacturername.clear();
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
    m_version.clear();
    m_version.insert(0,QString::number(rversion,10));
    m_version.insert(0,'.');
    m_version.insert(0,QString::number(version,10));
    m_version.insert(0,'v');

    //qDebug()<<m_version;

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

    m_productsn.clear();
    (*(data+snaddr+12)==0x0A)?(i=12):(i=13);
    for(--i;i>=0;--i)
    {
        char tmp = *(data+snaddr+i);
        m_productsn.insert(0,QChar(tmp));
        //qDebug()<<"i: "<<i;
        /*//so so so so stupid .
        qDebug("tmp:%c,0x%x", tmp,tmp);
        if(tmp>=0x30 && tmp<=0x39)
        {
            m_productsn.insert(0,QChar(tmp));
            qDebug()<<"Numberical "<<QChar(tmp-0x30);
        }
        else if(tmp>=0x41 && tmp<=0x5A)//A~Z
        {
            m_productsn.insert(0,QChar(tmp));//65
            qDebug()<<"Case char "<<QChar(tmp);
        }
        else if((tmp>=0x61) && (tmp<=0x7A))//a~z
        {
            m_productsn.insert(0,QChar(tmp-0x61)); //0x57
            qDebug()<<"case char "<<QChar(tmp);
        }
        */
    }
    return m_productsn;
}

quint32 Edid_T::getManufacturerSN(void)
{
    m_manufacturesn = data[15]<<24|data[14]<<16|data[13]<<8|data[12];
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
     //qDebug()<<"name :"<<name.at(0)<<name.at(1)<<name.at(2);
     quint16 base = 0;
     quint16 Tmpname = 0;
     for (int i = 0; i <3; i++)
     {
         QChar tmpchar = name.at(i);
         base = tmpchar.unicode()-0x40;
         Tmpname|=base<<((2-i)*5);
     }
     data[8] = Tmpname>>8;
     data[9] = Tmpname;
     qDebug("DATA[8]: %x %x",data[8],data[9]);
}

void Edid_T::setProductYear(quint8 year)
{
    data[17] = year;
}

void Edid_T::setProductWeek(quint8 week)
{
    data[16] = week;
}

void Edid_T::setProductCode(quint16 productcode)
{
    //qDebug("productcode: %x",productcode);
    data[10] = productcode;
    data[11] = productcode>>8;
}

void Edid_T::setProductSN(QString sn)
{
    quint8 snaddr = findProductSn();
    if(!snaddr) return;
    snaddr+=5;
    int i=0;
    for(;i<12;i++)
    {
        *(data+snaddr+i) = 0x30;
    }
    *(data+snaddr+i) = 0x0A;

    for(int i=0;i<sn.size();i++)
    {
        QChar tmpchar = sn.at(i);
        //qDebug("tmpchar:0x%x",tmpchar);
        *(data+snaddr+i) = tmpchar.unicode();
        qDebug("datasn:0x%x",*(data+snaddr+i));
    }
}

void Edid_T::setManufacturerSN(quint32 manufacturersn)
{
    Q_UNUSED(manufacturersn);
}

void Edid_T::recalchecksum()
{
    quint8 checksum=0;
    for(int i=0;i<127;i++)
    {
        checksum+=data[i];
    }
    checksum=0x100-checksum;
    data[127] = checksum;
}


}
