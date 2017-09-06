#include "ddc_aps.h"

namespace ddc {

#define     FEEDBACK_LEN         9
#define     RETRY_CNT            3
#define     COMMON_DELAY         800
#define     LASTPACK_DELAY       1600


//HDCP related Instructions
quint8 erasehdcpCmdtab[]={0xD0,0x65,0x0A,0xFF,0x00,0x00};
quint8 hdcpkeyidCmdtab[]={0xD0,0x73,0x05,0x08};
quint8 hdcpCmdtab[]={0xD0,0x65,0x07,0x00,0x10};

//EDID related Instructions
quint8 edidvgacmdtab[]={0xD0,0x19,0x24,0x00,0x10};
quint8 ediddvicmdtab[]={0xD0,0x23,0x24,0x00,0x10};
quint8 edidhdmi1cmdtab[]={0xD0,0x27,0x24,0x00,0x10};
quint8 edidhdmi2cmdtab[]={0xD0,0x31,0x24,0x00,0x10};
quint8 edidhdmi3cmdtab[]={0xD0,0x41,0x24,0x00,0x10};
quint8 ediddpcmdtab[]={0xD0,0x35,0x0D,0x00,0x10};

//Common releated Instructions
quint8 enterAtcmdtab[]={0xc0,0x43,0x56,0x54,0x45,0x41,0x54,0x4f,0x4e};
quint8 setsourcecmdtab[]={0xc0,0x63,0x07,0x0A,0x00,0x01};
quint8 getsourcecmdtab[]={0xC0,0x65,0x07,0x0A,0x00,0x00};
quint8 resetcmdtab[]={0xc0,0x63,0x07,0x04,0x00,0x01};
quint8 getchecksumcmdtab[]={0xc0,0x73,0x07,0x1d,0x00,0x00};
quint8 getfixedchecksumcmdtab[]={0xc0,0x73,0x07,0x52,0x00,0x00};
quint8 factorymodecmdtab[]={0xc0,0x63,0x07,0x02,0x00,0x01};
quint8 burnmodecmdtab[]={0xc0,0x63,0x07,0x08,0x00,0x00};
quint8 autocolorcmdtab[]={0xc0,0x63,0x07,0x06,0x00,0x01};
quint8 getautocolorcmdtab[]={0xc0,0x65,0x07,0x06,0x00,0x00};
quint8 autoadjustcmdtab[]={0xc0,0x63,0x07,0x1e,0x00,0x00};
quint8 brightnesscmdtab[]={0xc0,0x63,0x07,0x10,0x00,0x34};
quint8 volumecmdtab[]={0xc0,0x63,0x07,0x62,0x00,0x00};
quint8 colortmpcmdtab[]={0xc0,0x63,0x07,0x14,0x00,0x01};
quint8 keylockcmdtab[]={0xc0,0x73,0x07,0xe3,0x00,0x00};
quint8 getkeycmdtab[]={0xc0,0x73,0x07,0x69,0x00,0x00};
quint8 getksvcmdtab[]={0x73,0x07,0x69,0x00,0x00};
quint8 writesncmdtab[]={0xc0,0x63,0x0a,0x00,0x10};
quint8 getsncmdtab[]={0xc0,0x65,0x0a,0x00,0x10,0x00};
quint8 panelcmdtab[]={0xc0,0x63,0x07,0x18,0x00,0x02};

//verify function declarations
bool CommonFeedbackverify(quint8 *feedback,quint8 fdlen,quint8 *data,quint8 len)
{
    Q_UNUSED(fdlen);
    Q_UNUSED(data);
    Q_UNUSED(len);

    if(*(feedback+7)==0xE0)
        return true;
    else
        return false;
}

burndata_t CommonAssemble_Alloc(quint8 *head,quint8 headsize,quint8 *body,quint8 bodysize)
{
    quint8 *wholepackage = new quint8[headsize+bodysize];
    burndata_t tmpburndata;

    memcpy(wholepackage,head,headsize);
    memcpy(wholepackage+headsize,body,bodysize);

    tmpburndata.data = wholepackage;
    tmpburndata.size = headsize+bodysize;
    return tmpburndata;
}

//Enter AT Status
burnCmd_t enterATcmd =
{
    QString("EnterATStatus"),
    QString("enter the AT status, so can the other instructions can be executed!"),
    kNull,
    enterAtcmdtab,
    sizeof(enterAtcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    COMMON_DELAY,
    COMMON_DELAY,
};

//Set Source

burndata_t SetSourceAssemble(quint8 *head,quint8 headsize,quint8 *body,quint8 bodysize)
{
    burndata_t tmpburndata;
    //if(bodysize>1) return;

    *(head+5) = *body;

    tmpburndata.data = head;
    tmpburndata.size = headsize;
    return tmpburndata;
}

burnCmd_t setSourcecmd =
{
    QString("Set Source"),
    QString("Set the input source, source can be the follows:\
    0x01:VGA 0x04:DVI 0x08:HDMI1 0x09:HDMI2 0x0A:HDMI3 0x0C:DP"),
    kLineEdit,
    setsourcecmdtab,
    sizeof(setsourcecmdtab),
    &SetSourceAssemble,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    COMMON_DELAY,
    COMMON_DELAY,
};

//HDCP relevant
burnCmd_t erasehdcpcmd =
{
    nullptr,
    nullptr,
    kNull,
    erasehdcpCmdtab,
    sizeof(erasehdcpCmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t hdcpkeyidcmd =
{
    nullptr,
    nullptr,
    kNull,
    hdcpkeyidCmdtab,
    sizeof(hdcpkeyidCmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t hdcpburncmd =
{
    nullptr,
    nullptr,
    kNull,
    hdcpCmdtab,
    sizeof(hdcpCmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    LASTPACK_DELAY,
};

//EDID relevant
burnCmd_t edid_vgacmd =
{
    nullptr,
    nullptr,
    kNull,
    edidvgacmdtab,
    sizeof(edidvgacmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    LASTPACK_DELAY,
};

burnCmd_t edid_dvicmd =
{
    nullptr,
    nullptr,
    kNull,
    ediddvicmdtab,
    sizeof(ediddvicmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    LASTPACK_DELAY,
};

burnCmd_t edid_hdmi1cmd =
{
    nullptr,
    nullptr,
    kNull,
    edidhdmi1cmdtab,
    sizeof(edidhdmi1cmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    LASTPACK_DELAY,
};

burnCmd_t edid_hdmi2cmd =
{
    nullptr,
    nullptr,
    kNull,
    edidhdmi2cmdtab,
    sizeof(edidhdmi2cmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    LASTPACK_DELAY,
};

burnCmd_t edid_hdmi3cmd =
{
    nullptr,
    nullptr,
    kNull,
    edidhdmi3cmdtab,
    sizeof(edidhdmi3cmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    LASTPACK_DELAY,
};

burnCmd_t edid_dpcmd =
{
    nullptr,
    nullptr,
    kNull,
    ediddpcmdtab,
    sizeof(ediddpcmdtab),
    &CommonAssemble_Alloc,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    LASTPACK_DELAY,
};

}
