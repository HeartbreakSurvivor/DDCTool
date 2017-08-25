#ifndef CVT_TABDIALOG_H
#define CVT_TABDIALOG_H

#include "global.h"

struct Edid_Allports
{
    int edid_ports;
    QCheckBox *ports_checkbox;
};

class BurnTab:public QWidget
{
    Q_OBJECT//if you wanna use the signal and slots ,you must add this.

public:
    BurnTab(QWidget *parent=0);
    ~BurnTab();

    void readSettings();
    void writeSettings();

    void getEdidportsCheckbox(unsigned char *ports);
    void getEdidCustomer(Edid_Customer customer);

    void DDC_EnableDisableMenuItems(bool isDisable);
    void DDC_EnableDisableDeviceItems(bool isDisable);

    void DDC_GetBurnSettings(void);

    public slots:
    void closeEvent(QCloseEvent *event);

    void LoadHDCPKey();
    void LoadEDID();

    //Connect and disconnect device
    void DDC_ConnectDevice();
    void DDC_DisConnectDevice();

    //EDID module
    void Edid_BurnAllportsEdid();
    void Edid_GetAllportsEdid();
    //HDCPKEY module
    void Hdcp_BurnHdcpkey();
    void Hdcp_GetHdcpkey();
    //DDC Msg and Logs
    void DDC_DebugMsgSend();
    void DDC_MsgClear();
    void DDC_OpenLogs();

    //Thread Slots
    void DDC_FinishBurnEdid();
    void DDC_FinishBurnHdcpkey();

    void DDC_HdcpBurningInfo(int num);
    void DDC_EdidBurningInfo(int num);

private:
    unsigned char edidports=0;

    Cvt_flowControlPar cvt_flowcontrol;
    Cvt_BurnSetting cvt_burnsetting;
    Isp_I2C cvt_I2cdevice;

    Cvt_DataThread *HdcpkeydataThread=NULL;//data transfer thread.
    Cvt_DataManage *EdiddataThread=NULL;

    QSettings Edid_Settings;//store the user customer and edid ports
    QSettings Burn_Settings;//restore the Burn Settings

    QAction *QdebugMode;

    //Main Window
    QTabWidget *DDC_Tabs;

    QGroupBox *EDIDGroup();
    QGroupBox *DDCMsgGroup();
    QGroupBox *HdcpKeyGroup();
    QGroupBox *PROMPTGroup();

    QLabel *Logo_Label;

    //EDID groupbox
    QLineEdit *EdidDataPath;
    QPushButton *LoadEDIDBtn;
    QLineEdit *EdidManufacturerName;
    QLineEdit *EdidProductCode;
    QLineEdit *EdidYear;
    QLineEdit *EdidWeek;
    QLineEdit *EdidSerialNumber;

    QCheckBox *Edid_VGA;
    QCheckBox *Edid_DVI;
    QCheckBox *Edid_HDMI1;
    QCheckBox *Edid_HDMI2;
    QCheckBox *Edid_HDMI3;
    QCheckBox *Edid_DP;

    QCheckBox* _edidports[EdidPorts_Num] = {
        Edid_VGA,
        Edid_DVI,
        Edid_HDMI1,
        Edid_HDMI2,
        Edid_HDMI3,
        Edid_DP,
    };

    QButtonGroup *Edid_CustomerGroup;
    QRadioButton *Edid_Cvt;
    QRadioButton *Edid_Aoc;
    QRadioButton *Edid_Acer;
    QRadioButton *Edid_Spt;

    QPushButton *Edid_Getedidbtn;
    QPushButton *Edid_Burnedidbtn;

    //HDCPKey groupbox
    QLineEdit *HdcpDataPath;
    QPushButton *LoadHdcpkeyBtn;
    QLineEdit *Hdcp_TotalKeys;
    QLineEdit *Hdcp_RemainingKeys;
    QLineEdit *Hdcp_CurrentKey;
    QCheckBox *Hdcp_SetKeyIdx;//Assign the specific Key Index and burn it all the time.
    QCheckBox *Hdcp_Crc;//Cal the CRC or not(realtek hdcpkey has no checksum which mstar has)

    QPushButton *Hdcp_Burnhdcpbtn;
    QPushButton *Hdcp_Gethdcpbtn;

    //Burn IIC Settings
    QLabel *Prompt_Hint;
    QPushButton* Burn_Connectbtn;
    QPushButton* Burn_DisConnectbtn;
    QSpinBox *Burn_IICSpeedSpin;
    QSpinBox *Burn_EraseHdcpDealy;//The first package must delay some time wait the boar to erase the flash etc.
    QSpinBox *Burn_EdidLastPackDelay;
    QSpinBox *Burn_HdcpLastPackDelay;//when finish transfer the hdcpkey and Sleep for a while until the board has finished save the data to eeprom of flash.
    QSpinBox *Burn_WriteDelay;
    QSpinBox *Burn_ReadDelay;
    QSpinBox *Burn_RetryCnt;
    QSpinBox *Burn_SinglePackRetryCnt;//The single package retry cnt,if one package filed ,It doesn't mean the whole package is failed,so we try again.

    //DDC Msg groupBox
    QTextEdit *DDC_BurnLog;

    QRadioButton* _edidcustomer[4] = {
        Edid_Cvt,
        Edid_Aoc,
        Edid_Acer,
        Edid_Spt,
    };
};


#endif // CVT_TABDIALOG_H
