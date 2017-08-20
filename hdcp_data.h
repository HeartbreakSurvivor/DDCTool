#ifndef HDCP_DATA_H
#define HDCP_DATA_H

#include "burn_data.h"

namespace ddc {



class Hdcp_T:public BurnData_T
{
public:
    Hdcp_T() {}
    ~Hdcp_T() {}

    //const quint16 keyidoffset;//can't be initialized must use the constructor initializer list
    static const quint16 keyidoffset = 376;//keyid offset of the reltek hdcpkey
    typedef enum{
        Mstar,
        Reltek,
    }chiptype_e;

private:
    void setChipType(quint8 type);
    chiptype_e getChipType(void);
    void setIsWritekeyid(bool bon);
    void setIsAppedCrc(bool bon);
    void getKeyid(void);
    void appendCrc16(void);

    virtual void getdata(int offset, int bufsize, unsigned char *buf, int Rlen);

private:
    chiptype_e m_type;
    quint8 m_Keyid[8];
    bool m_isWritekeyid;
    bool m_isAppendCrc;

};
}
#endif // HDCP_DATA_H
