#include "hdcp_data.h"

namespace ddc {

bool isKsvValid(unsigned char ksv[])
{
    typedef long long L;
    L KSV = (L)ksv[0] << 32 | (L)ksv[1] << 24 | (L)ksv[2] << 16 | (L)ksv[3] << 8 | (L)ksv[4];

    int i = 0, count = 0;
    for (i = 0; i<40; i++)
    {
        count += ((KSV >> i) & 0x1);
    }
    if (count != 20)
        return 0;
    else
        return 1;
}

unsigned short CalCRC16(unsigned char* datas, int len)
{
    unsigned short crc = 0;
    unsigned char da = 0;
    unsigned char *ptr = datas;

    unsigned short crc_table[16] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    };

    crc = 0;

    while( len-- != 0 ) {
        da = ((unsigned char)(crc / 256)) / 16; /* temp crc high 4bits */
        crc <<= 4;
        crc ^= crc_table[da ^ ( *ptr / 16 )];

        da = ((unsigned char)(crc / 256)) / 16;
        crc <<= 4;
        crc ^= crc_table[da ^ (*ptr & 0x0F)];
        ptr++;
    }
    return (crc);
}

unsigned long CalCRC32(const unsigned char *datas, int len)
{
    unsigned long crc = 0xFFFFFFFF;
    unsigned long temp;
    int j;
    while (len--)
    {
        temp = (unsigned long) ( (crc & 0xFF) ^ *datas++);
        for (j = 0; j < 8; j++)
        {
            if (temp & 0x1)
            {
                temp = (temp >> 1) ^ 0xEDB88320;
            }
            else
            {
                temp >>= 1;
            }
        }
        crc = (crc >> 8) ^ temp;
    }
    return crc ^ 0xFFFFFFFF;
}

void Hdcp_T::setChipType(quint8 type)
{
    m_type = (chiptype_e)type;
}

chiptype_e Hdcp_T::getChipType(void)
{
    return m_type;
}

void Hdcp_T::setIsWritekeyid(bool bon)
{
    m_isWritekeyid = bon;
}

void Hdcp_T::setIsAppedCrc(bool bon)
{
    m_isAppendCrc = bon;
}

void Hdcp_T::getKeyid(void)
{
    if(m_type == Mstar)//parse the keyid from the file name
    {
        memset(m_Keyid,0,sizeof(m_Keyid));
        int sz = filename.size();
            qDebug() << "filename size:"<< sz ;
        for(int i=sz-1,j=8;i>=0;--i)
        {
            QChar tmpchar = filename.data()[i];
            qDebug() << "filename char:"<< tmpchar ;
            if(tmpchar<'0' || tmpchar>'9')
                break;
            m_Keyid[--j] = tmpchar;
            if(j<=0) break;
        }
    }
    else if (m_type == Reltek)
    {
        memcpy(m_Keyid,data+keyidoffset,8);
    }
}

void Hdcp_T::appendCrc16(void)
{
    quint16 crc=ddc::CalCRC16(data,size);
    std::cout<<"the crc is"<<crc<<std::endl;

    data = realloc(data,size+2);
    if(data != nullptr)
    {
        data[size+1] = (quint8)crc;
        data[size+2] = (quint8)crc>>8;
    }
}

virtual bool Hdcp_T::getdata(int offset, int bufsize, unsigned char *buf, int Rlen)
{
    if (data == NULL) return false;
    if (offset*bufsize >= size) return false;
    memcpy((void*)buf, (void *)(data+offset*bufsize), Rlen);
    return true;
}


}
