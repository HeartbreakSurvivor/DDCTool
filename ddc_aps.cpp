#include "ddc_aps.h"

namespace ddc {

//HDCP related Instructions
quint8 erasehdcpCmdtab[]={0x86,0xD0,0x65,0x0A,0xFF,0x00,0x00};
quint8 hdcpkeyidCmdtab[]={0x8D,0xD0,0x73,0x05,0x00};
quint8 hdcpCmdtab[]={0x95,0xD0,0x65,0x0A,0xFF,0x00,0x00};

//EDID related Instructions
quint8 edidvgacmdtab[]={0x95,0xD0,0x19,0x24,0x00,0x10};
quint8 ediddvicmdtab[]={0x95,0xD0,0x23,0x24,0x00,0x10};
quint8 edidhdmi1cmdtab[]={0x95,0xD0,0x27,0x24,0x00,0x10};
quint8 edidhdmi2cmdtab[]={0x95,0xD0,0x31,0x24,0x00,0x10};
quint8 edidhdmi3cmdtab[]={0x95,0xD0,0x41,0x24,0x00,0x10};
quint8 ediddpcmdtab[]={0x95,0xD0,0x37,0x24,0x00,0x10};

//Common releated Instructions
quint8 enterAtcmdtab[]={0x89,0xc0,0x43,0x56,0x54,0x45,0x41,0x54,0x4f,0x4e};
quint8 setsourcecmdtab[]={0x86,0xc0,0x63,0x0a,0x00,0x01};
quint8 getsourcecmdtab[]={0x86,0xC0,0x65,0x07,0x0A,0x00,0x00};
quint8 resetcmdtab[]={0x86,0xc0,0x63,0x07,0x04,0x00,0x01};
quint8 getchecksumcmdtab[]={0x86,0xc0,0x73,0x07,0x1d,0x00,0x00};
quint8 getfixedchecksumcmdtab[]={0x86,0xc0,0x73,0x07,0x52,0x00,0x00};
quint8 factorymodecmdtab[]={0x86,0xc0,0x63,0x07,0x02,0x00,0x01};
quint8 burnmodecmdtab[]={0x86,0xc0,0x63,0x07,0x08,0x00,0x00};
quint8 autocolorcmdtab[]={0x86,0xc0,0x63,0x07,0x06,0x00,0x01};
quint8 getautocolorcmdtab[]={0x86,0xc0,0x65,0x07,0x06,0x00,0x00};
quint8 autoadjustcmdtab[]={0x86,0xc0,0x63,0x07,0x1e,0x00,0x00};
quint8 brightnesscmdtab[]={0x86,0xc0,0x63,0x07,0x10,0x00,0x34};
quint8 volumecmdtab[]={0x86,0xc0,0x63,0x07,0x62,0x00,0x00};
quint8 colortmpcmdtab[]={0x86,0xc0,0x63,0x07,0x14,0x00,0x01};
quint8 keylockcmdtab[]={0x86,0xc0,0x73,0x07,0xe3,0x00,0x00};
quint8 getkeymdtab[]={0x86,0xc0,0x73,0x07,0x69,0x00,0x00};
quint8 writesncmdtab[]={0x95,0xc0,0x63,0x0a,0x00,0x10};
quint8 getsncmdtab[]={0x86,0xc0,0x65,0x0a,0x00,0x10,0x00};
quint8 panelcmdtab[]={0x86,0xc0,0x63,0x07,0x18,0x00,0x02};

//verify function declarations
bool commoncmdverify(quint8 *senddata,quint8 *feedback)
{
    Q_UNUSED(senddata);
    if(*(feedback+7)==0xE0)
        return true;
    else
        return false;
}

#if 0
burnCmd_t erasehdcpcmd =
{
    erasehdcpCmdtab,//erase hdcp
    8,
    9,
    3,
    &commoncmdverify,
    nullptr
};

burnCmd_t hdcpkeyidcmd =
{
    erasehdcpCmdtab,//erase hdcp
    8,
    9,
    3,
    &commoncmdverify,
    nullptr
};

void assemblehdcp(quint8 offset,quint8 len)
{
    quint32 length = PERPACK_LEN;
    quint32 Rlength = length;
    quint32 remainder = m_bodysize % 16;
    quint32 nTimes = (remainder == 0) ? m_bodysize/(length-1) : m_bodysize/length;//how many packs we should send to board.

    quint8 buf[30] = {0};//temp buffer to store the send data

    for (quint32 i = 0; i <= nTimes; i++)
    {
        qDebug("The %d times\n", i);

        if (i == nTimes)
        {
            qDebug("The last times\n");
            if (remainder != 0)
            {
                Rlength = remainder;
            }
        }

        //assemble the data
        memcpy((void*)buf, (void *)(m_databody+i*length), Rlength);//get data from the transfer

        if(m_assemblefunc)
        {
            this->m_assemblefunc(i,Rlength);//assemble the data,(append the head and set the offset and package data len)
        }

        m_protocol.setPackdata(buf,Rlength);

        if(m_protocol.burn())
        {
            qDebug("The singale pack burn success and start next pack\n");
            if (i == nTimes) return true;
        }
        else
        {
            qDebug("The single pack rerty failed and return\n");
            return false;
        }
    }
    return true;
}
burnCmd_t hdcpcmd =
{
    hdcpCmdtab,//burn hdcp
    7,
    9,
    3,
    &commoncmdverify,
    nullptr
};

burnCmd_t edid_vgacmd =
{
    edidvgacmdtab,//burn vga edid
    7,
    9,
    3,
    &commoncmdverify,
    nullptr
};

burnCmd_t edid_dvicmd =
{
    ediddvicmdtab,//burn dvi edid
    7,
    9,
    3,
    &commoncmdverify,
    nullptr
};

burnCmd_t edid_hdmi1cmd =
{
    edidhdmi1cmdtab,//burn hdmi1 edid
    7,
    9,
    3,
    &commoncmdverify,
    nullptr
};

burnCmd_t edid_hdmi2cmd =
{
    edidhdmi2cmdtab,//burn hdmi2 edid
    7,
    9,
    3,
    &commoncmdverify,
    nullptr
};

burnCmd_t edid_hdmi3cmd =
{
    edidhdmi3cmdtab,//burn hdmi3 edid
    7,
    9,
    3,
    &commoncmdverify,
    nullptr
};

burnCmd_t edid_dpcmd =
{
    ediddpcmdtab,//burn dp edid
    7,
    9,
    3,
    &commoncmdverify,
    nullptr
};
#endif



}
