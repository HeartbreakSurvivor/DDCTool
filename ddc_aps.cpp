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

bool CommonSetpara(QString& text,quint8 *head,quint8 headsize)
{
    Q_UNUSED(headsize);
    QStringList paralist = text.split(' ', QString::SkipEmptyParts);

    for(auto x:paralist)
    {
        if(x.length()>2)
        {
            qDebug()<<"error format.";
            return false;
        }
    }
    if(paralist.size()>1)
    {
        qDebug()<<"you may send too much.";
        return true;
    }

    //some defense condition
    bool *ok=0;
    int value = text.toInt(ok,16);
    qDebug()<<"para:"<<value;
    *(head+5) = (quint8)value;
    return true;
}

/*******************************************************
 ** No Parameter Instructions.
 *******************************************************/
//Enter AT Status
burnCmd_t enterATcmd =
{
    QString("EnterATStatus"),
    QString("Enter the AT status, so can the other instructions execute!"),
    nullptr,
    enterAtcmdtab,
    sizeof(enterAtcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};

//Reset
burnCmd_t resetcmd =
{
    QString("Reset"),
    QString("Reset the Monitor's user settings!"),
    nullptr,
    resetcmdtab,
    sizeof(resetcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Get Source
burnCmd_t getSourcecmd =
{
    QString("Get Source"),
    QString("Get the current input source,which might be follows:\n"
    "0x01:VGA 0x04:DVI 0x08:HDMI1 0x09:HDMI2 0x0A:HDMI3 0x0C:DP"),
    nullptr,
    getsourcecmdtab,
    sizeof(getsourcecmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Get Checksum
burnCmd_t getChecksumcmd =
{
    QString("Get Checksum"),
    QString("Get the real Checksum.\n"),
    nullptr,
    getchecksumcmdtab,
    sizeof(getchecksumcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Get Fixed Checksum
burnCmd_t getFixedChecksumcmd =
{
    QString("Get Fixed Checksum"),
    QString("Get the Fixed Checksum.\n"),
    nullptr,
    getfixedchecksumcmdtab,
    sizeof(getfixedchecksumcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Get HDCP KSV
burnCmd_t gethdcpksvcmd =
{
    QString("Get HDCP KSV"),
    QString("Get the HDCPKEY's KSV.\n"),
    nullptr,
    getksvcmdtab,
    sizeof(getksvcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Get Key Press
burnCmd_t getkeypresscmd =
{
    QString("Get Key Press"),
    QString("get the keypad press status.D0~D7 --> Key0~Key7 \n"
    "0 means not pressd,1 for pressed."
    "for example: 0x02 means the key2 has been pressed."),
    nullptr,
    getkeycmdtab,
    sizeof(getkeycmdtab),
    nullptr,
    FEEDBACK_LEN,
    nullptr,
    3,
    500,
    COMMON_DELAY,
};

//ADC autocolor
burnCmd_t autocolorcmd =
{
    QString("ADC autocolor"),
    QString("ADC autocolor only for VGA input source.\n"),
    nullptr,
    autocolorcmdtab,
    sizeof(autocolorcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Auto Adjust
burnCmd_t autoadjustcmd =
{
    QString("Auto Adjust"),
    QString("Auto Adjust only for VGA input source.\n"),
    nullptr,
    autoadjustcmdtab,
    sizeof(autoadjustcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Get AutoColor Status
burnCmd_t getautocolorstatuscmd =
{
    QString("Get AutoColor Status"),
    QString("Get the ADC auto color status,result can be follows.\n"
    "0x00:NONE 0x01:DONE 0x02:FAIL"),
    nullptr,
    getautocolorcmdtab,
    sizeof(getautocolorcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

//Get SN
burnCmd_t getsncmd =
{
    QString("Get SN"),
    QString("Get the factory serianl number.\n"),
    nullptr,
    getsncmdtab,
    sizeof(getsncmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    500,
    COMMON_DELAY,
};

/*******************************************************
 ** Set Instructions.
 *******************************************************/
//Set Source
burnCmd_t setSourcecmd =
{
    QString("Set Source"),
    QString("Set the input source, source can be the follows:\n\
    0x01:VGA 0x04:DVI 0x08:HDMI1\n    0x09:HDMI2 0x0A:HDMI3 0x0C:DP"),
    CommonSetpara,
    setsourcecmdtab,
    sizeof(setsourcecmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    COMMON_DELAY,
    COMMON_DELAY,
};

//Set volume
burnCmd_t setVolumecmd =
{
    QString("Set Volume"),
    QString("Set the volume,the range of parameter between 0x00~0x64:\n"),
    CommonSetpara,
    volumecmdtab,
    sizeof(volumecmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};

//Set brightness
burnCmd_t setBrightnesscmd =
{
    QString("Set Brightness"),
    QString("Set the brightness,the range of parameter between 0x00~0x64:\n"),
    CommonSetpara,
    brightnesscmdtab,
    sizeof(brightnesscmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};

//Set Colortemp
burnCmd_t setColortempcmd =
{
    QString("Set ColorTemperature"),
    QString("Set the color temperature,the parameters as fllows:\n"
    "0x00:user 0x01:SRGB 0x02:Native 0x05:6500K\n"
    "0x06:7500K 0x08:9300K 0x0A:11500K"),
    CommonSetpara,
    colortmpcmdtab,
    sizeof(colortmpcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};

//Set Keypad lock
burnCmd_t setKeypadlockcmd =
{
    QString("Set keypad lock status"),
    QString("Set the keypad lock status, 0x00 for unlock,0x01 for lock.\n"),
    CommonSetpara,
    keylockcmdtab,
    sizeof(keylockcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};

//Set Factory Mode
burnCmd_t setFactorymodecmd =
{
    QString("Set Factory Mode"),
    QString("Enter or exit the facctory mode.\n"
    "0x00:exit the factory mode.\n"
    "0x01:enter the factory mode."),
    CommonSetpara,
    factorymodecmdtab,
    sizeof(factorymodecmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};

//Set Burning Mode
burnCmd_t setBurningmodecmd =
{
    QString("Set Burning Mode"),
    QString("Enter or exit the Burning mode.\n"
    "0x00:exit the burning mode.\n"
    "0x01:enter the burning mode."),
    CommonSetpara,
    burnmodecmdtab,
    sizeof(burnmodecmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};

//Set Factory panel
burnCmd_t setFactorypanelcmd =
{
    QString("Set factory panel parameter"),
    QString("Set the factory panel parameters, parameters can be follows.\n"
    "0x00: default 0x01: 1366 0x02:1920 0x03:120hz 0x04:4k"),
    CommonSetpara,
    panelcmdtab,
    sizeof(panelcmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    20,
    COMMON_DELAY,
};


//Write SN
burnCmd_t writesncmd =
{
    QString("Write SN"),
    QString("Write the factory serial number.\n"
    "for example,please input the SN like follows:"
    "PY-SA400840124287"),
    CommonSetpara,
    writesncmdtab,
    sizeof(writesncmdtab),
    nullptr,
    FEEDBACK_LEN,
    &CommonFeedbackverify,
    3,
    COMMON_DELAY,
    COMMON_DELAY,
};

burnCmd_t* ATCmds[]={
    &enterATcmd,
    &resetcmd,
    &setSourcecmd,
    &setVolumecmd,
    &setBrightnesscmd,
    &setBurningmodecmd,
    &setFactorymodecmd,
    &setColortempcmd,
    &setKeypadlockcmd,
    &setFactorypanelcmd,
    &writesncmd,
    &autoadjustcmd,
    &getautocolorstatuscmd,
    &getChecksumcmd,
    &getFixedChecksumcmd,
    &gethdcpksvcmd,
    &autocolorcmd,
    &getsncmd,
    &getkeypresscmd,
    &getSourcecmd,
};


int getATCmdLen(void)
{
   return COUNTOF(ATCmds);
}

//HDCP relevant
burnCmd_t erasehdcpcmd =
{
    nullptr,
    nullptr,
    nullptr,
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
    nullptr,
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
    nullptr,
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
    nullptr,
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
    nullptr,
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
    nullptr,
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
    nullptr,
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
    nullptr,
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
    nullptr,
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
