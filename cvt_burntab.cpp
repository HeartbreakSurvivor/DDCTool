#include "cvt_burntab.h"


BurnTab::BurnTab(QWidget *parent):QWidget(parent)
{
    QGridLayout *grid = new QGridLayout;

    grid->addWidget(EDIDGroup(), 0, 0, 1, 1);
    //grid->addWidget(HdcpKeyGroup(), 1, 0, 1, 1);
    //grid->addWidget(PROMPTGroup(), 0, 2, 2, 1);
    //grid->addWidget(DDCMsgGroup(), 3, 0, 1, 3);

    //cvt_edid = new Cvt_EDID[EdidPorts_Num]();//create the edid and append to cvt_edid dynamic class array

    setLayout(grid);

    /*
    QWidget *widget = new QWidget;
    widget->setLayout(grid);

    widget->resize(300, 320);
    //widget->setFixedSize(this->width(),this->height());
    //setFixedSize(800,700);
    setCentralWidget(widget);
    setWindowTitle(tr("CVT DDC Tool V1.0"));

    //DDC_Tabs->addTab(widget,"DDC Burn");

*/
    qDebug() << "hahahaha";
}

BurnTab::~BurnTab()
{

}

QGroupBox *BurnTab::EDIDGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("EDID Info"));

    //Load the EDID and display the path.
    EdidDataPath = new QLineEdit();
    EdidDataPath->setPlaceholderText("the Edid path");
    EdidDataPath->setReadOnly(true);
    LoadEDIDBtn = new QPushButton(tr("&Load"));
    connect(LoadEDIDBtn, SIGNAL(clicked()), this, SLOT(LoadEDID()));

    //Edid Prase result
    QLabel *Edid_ManufacturerName = new QLabel("Manufacturer Name:",this);
    EdidManufacturerName = new QLineEdit(this);
    EdidManufacturerName->setReadOnly(true);
    QLabel *Edid_ProductCode = new QLabel("Product Code:", this);
    EdidProductCode = new QLineEdit(this);
    EdidProductCode->setReadOnly(true);
    QLabel *Edid_ProductYear = new QLabel("Product Year:", this);
    EdidYear = new QLineEdit(this);
    EdidYear->setReadOnly(true);
    QLabel *Edid_ProductWeek = new QLabel("Product Week:", this);
    EdidWeek = new QLineEdit(this);
    EdidWeek->setReadOnly(true);

    //The SN module
    QLabel *Edid_Sn = new QLabel("SN number:", this);
    EdidSerialNumber = new QLineEdit();
    EdidSerialNumber->setPlaceholderText("the Serial Number");

    //All EDID ports options
    _edidports[0] = new QCheckBox(tr("&VGA"));
    _edidports[1] = new QCheckBox(tr("&DVI"));
    _edidports[2] = new QCheckBox(tr("&HDMI1"));
    _edidports[3] = new QCheckBox(tr("&HDMI2"));
    _edidports[4] = new QCheckBox(tr("&HDMI3"));
    _edidports[5] = new QCheckBox(tr("&DP"));

    QHBoxLayout *EdidPorts_Layout = new QHBoxLayout(this);
    EdidPorts_Layout->addWidget(_edidports[0]);
    EdidPorts_Layout->addWidget(_edidports[1]);
    EdidPorts_Layout->addWidget(_edidports[2]);
    EdidPorts_Layout->addWidget(_edidports[3]);
    EdidPorts_Layout->addWidget(_edidports[4]);
    EdidPorts_Layout->addWidget(_edidports[5]);

    //All customer options
    Edid_CustomerGroup = new QButtonGroup(this);
    _edidcustomer[EDID_CVT] = new QRadioButton("Cvte", this);
    _edidcustomer[EDID_AOC] = new QRadioButton("Aoc", this);
    _edidcustomer[EDID_ACER] = new QRadioButton("Acer", this);
    _edidcustomer[EDID_SPT] = new QRadioButton("Spt", this);
    _edidcustomer[EDID_AOC]->setChecked(true);

    Edid_CustomerGroup->addButton(_edidcustomer[EDID_CVT], 1);
    Edid_CustomerGroup->addButton(_edidcustomer[EDID_AOC], 2);
    Edid_CustomerGroup->addButton(_edidcustomer[EDID_ACER], 3);
    Edid_CustomerGroup->addButton(_edidcustomer[EDID_SPT], 4);
    QHBoxLayout *EdidCustomers_Layout = new QHBoxLayout(this);
    EdidCustomers_Layout->addWidget(_edidcustomer[EDID_CVT]);
    EdidCustomers_Layout->addWidget(_edidcustomer[EDID_AOC]);
    EdidCustomers_Layout->addWidget(_edidcustomer[EDID_ACER]);
    EdidCustomers_Layout->addWidget(_edidcustomer[EDID_SPT]);

    Edid_Burnedidbtn = new QPushButton(tr("Write"));
    Edid_Getedidbtn = new QPushButton(tr("Read"));
    connect(Edid_Burnedidbtn, SIGNAL(clicked()), this, SLOT(Edid_BurnAllportsEdid()));
    connect(Edid_Getedidbtn, SIGNAL(clicked()), this, SLOT(Edid_GetAllportsEdid()));

    QHBoxLayout *EdidBurnAndGet_Layout = new QHBoxLayout(this);
    EdidBurnAndGet_Layout->addWidget(Edid_Burnedidbtn);
    EdidBurnAndGet_Layout->addWidget(Edid_Getedidbtn);

    //Set the Layout
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(EdidDataPath, 0, 0, 1, 3);
    gridLayout->addWidget(LoadEDIDBtn, 0, 3, 1, 1);

    gridLayout->addWidget(Edid_ManufacturerName, 1, 0, 1, 1);
    gridLayout->addWidget(EdidManufacturerName, 1, 1, 1, 1);
    gridLayout->addWidget(Edid_ProductCode, 1, 2, 1, 1);
    gridLayout->addWidget(EdidProductCode, 1, 3, 1, 1);

    gridLayout->addWidget(Edid_ProductYear, 2, 0, 1, 1);
    gridLayout->addWidget(EdidYear, 2, 1, 1, 1);
    gridLayout->addWidget(Edid_ProductWeek,2, 2, 1, 1);
    gridLayout->addWidget(EdidWeek, 2, 3, 1, 1);

    gridLayout->addWidget(Edid_Sn,3, 0, 1, 1);
    gridLayout->addWidget(EdidSerialNumber, 3, 1, 1, 3);

    //gridLayout->addLayout(EdidPorts_Layout, 4, 0, 1, 4);
    //gridLayout->addLayout(EdidCustomers_Layout, 5, 0, 1, 3);
    //gridLayout->addLayout(EdidBurnAndGet_Layout, 6, 0, 1, 2);

    groupBox->setLayout(gridLayout);
    return groupBox;
}

QGroupBox *BurnTab::HdcpKeyGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("HdcpKey info"));

    HdcpDataPath = new QLineEdit();
    HdcpDataPath->setPlaceholderText("the HDCPKey set path");
    HdcpDataPath->setMinimumWidth(120);
    //HdcpDataPath->setSizePolicy(QSizePolicy::horizontalPolicy, QSizePolicy::verticalPolicy);
    HdcpDataPath->setReadOnly(true);
    LoadHdcpkeyBtn = new QPushButton(tr("&Load"));
    connect(LoadHdcpkeyBtn, SIGNAL(clicked()), this, SLOT(LoadHDCPKey()));

    QLabel *HdcpTotalsize = new QLabel(tr("TotalKey:"), this);
    Hdcp_TotalKeys = new QLineEdit(this);
    QLabel *HdcpRemainingKeys = new QLabel(tr("RemainingKey:"), this);
    Hdcp_RemainingKeys = new QLineEdit(this);
    QLabel *HdcpCurrentKey = new QLabel(tr("CurrentKey:"), this);
    Hdcp_CurrentKey = new QLineEdit(this);
    Hdcp_SetKeyIdx = new QCheckBox(tr("&Set Key Idx"),this);
    Hdcp_Crc = new QCheckBox("&Checksum",this);

    QLabel *BurnEraseHdcpDealy = new QLabel("ErasehdcpDly:", this);
    Burn_EraseHdcpDealy = new QSpinBox(this);
    Burn_EraseHdcpDealy->setSingleStep(1);
    Burn_EraseHdcpDealy->setRange(1, 3000);
    Burn_EraseHdcpDealy->setSuffix("ms");

    Hdcp_Burnhdcpbtn = new QPushButton(tr("&Write"), this);
    Hdcp_Gethdcpbtn = new QPushButton(tr("&Read"), this);
    connect(Hdcp_Burnhdcpbtn, SIGNAL(clicked()), this, SLOT(Hdcp_BurnHdcpkey()));
    connect(Hdcp_Gethdcpbtn, SIGNAL(clicked()), this, SLOT(Hdcp_GetHdcpkey()));
    QHBoxLayout *HdcpBurnAndGet_Layout = new QHBoxLayout(this);
    HdcpBurnAndGet_Layout->addWidget(Hdcp_Burnhdcpbtn);
    HdcpBurnAndGet_Layout->addWidget(Hdcp_Gethdcpbtn);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(HdcpDataPath, 0, 0, 1, 3);
    gridLayout->addWidget(LoadHdcpkeyBtn, 0, 3, 1, 1);

    gridLayout->addWidget(HdcpTotalsize, 1, 0, 1, 1);
    gridLayout->addWidget(Hdcp_TotalKeys,1, 1, 1, 1);
    gridLayout->addWidget(HdcpRemainingKeys, 1, 2, 1, 1);
    gridLayout->addWidget(Hdcp_RemainingKeys, 1, 3, 1, 1);

    gridLayout->addWidget(HdcpCurrentKey, 2, 0, 1, 1);
    gridLayout->addWidget(Hdcp_CurrentKey, 2, 1, 1, 1);
    gridLayout->addWidget(Hdcp_SetKeyIdx, 2, 2, 1, 1);
    gridLayout->addWidget(Hdcp_Crc, 2, 3, 1, 1);

    gridLayout->addWidget(BurnEraseHdcpDealy, 3, 0, 1, 1);
    gridLayout->addWidget(Burn_EraseHdcpDealy, 3, 1, 1, 1);

    gridLayout->addLayout(HdcpBurnAndGet_Layout, 3, 2, 1, 2);

    groupBox->setLayout(gridLayout);
    return groupBox;
}

QGroupBox *BurnTab::PROMPTGroup()
{
    int offset = 0;
    QGroupBox *groupBox = new QGroupBox(tr("Prompt"));

    Prompt_Hint = new QLabel(this);
    Prompt_Hint->setMinimumWidth(112);
    Prompt_Hint->setAlignment(Qt::AlignLeft);
    QFont font("hellow", 22, QFont::DemiBold, false);
    Prompt_Hint->setFont(font);
    Prompt_Hint->setStyleSheet("color:green");//CSS
    Prompt_Hint->setStyleSheet("background-color:black");

    QLabel *BurnEdidLastPackDelay = new QLabel("EdidlpDelay:", this);
    Burn_EdidLastPackDelay = new QSpinBox(this);
    Burn_EdidLastPackDelay->setSingleStep(1);
    Burn_EdidLastPackDelay->setRange(1, 3000);
    Burn_EdidLastPackDelay->setSuffix("ms");

    QLabel *BurnHdcpLastPackDelay = new QLabel("HdcplpDelay:", this);
    Burn_HdcpLastPackDelay = new QSpinBox(this);
    Burn_HdcpLastPackDelay->setSingleStep(1);
    Burn_HdcpLastPackDelay->setRange(1, 3000);
    Burn_HdcpLastPackDelay->setSuffix("ms");

    QLabel *BurnWriteDelay = new QLabel("WriteDelay:", this);
    Burn_WriteDelay = new QSpinBox(this);
    Burn_WriteDelay->setSingleStep(1);
    Burn_WriteDelay->setRange(1, 3000);
    Burn_WriteDelay->setSuffix("ms");

    QLabel *BurnReadDelay = new QLabel("ReadDelay:", this);
    Burn_ReadDelay = new QSpinBox(this);
    Burn_ReadDelay->setSingleStep(1);
    Burn_ReadDelay->setRange(1, 3000);
    Burn_ReadDelay->setSuffix("ms");

    QLabel *BurnRetryCnt = new QLabel("RetryCnt:", this);
    Burn_RetryCnt = new QSpinBox(this);
    Burn_RetryCnt->setSingleStep(1);
    Burn_RetryCnt->setRange(0, 5);

    QLabel *BurnSinglePackRetryCnt = new QLabel("SpRetryCnt:", this);
    Burn_SinglePackRetryCnt = new QSpinBox(this);
    Burn_SinglePackRetryCnt->setSingleStep(1);
    Burn_SinglePackRetryCnt->setRange(0, 5);


    QGridLayout *gridlayout = new QGridLayout(this);


    offset = 0;
    gridlayout->addWidget(BurnWriteDelay, 0, offset, 1, 1);
    gridlayout->addWidget(Burn_WriteDelay, 0, offset+1, 1, 1);
    gridlayout->addWidget(BurnReadDelay, 0, offset+2, 1, 1);
    gridlayout->addWidget(Burn_ReadDelay, 0, offset+3, 1, 1);
    gridlayout->addWidget(BurnEdidLastPackDelay, 1, offset, 1, 1);
    gridlayout->addWidget(Burn_EdidLastPackDelay, 1, offset + 1, 1, 1);
    gridlayout->addWidget(BurnHdcpLastPackDelay, 1, offset + 2, 1, 1);
    gridlayout->addWidget(Burn_HdcpLastPackDelay, 1, offset + 3, 1, 1);
    gridlayout->addWidget(BurnRetryCnt, 2, offset , 1, 1);
    gridlayout->addWidget(Burn_RetryCnt, 2, offset + 1, 1, 1);
    gridlayout->addWidget(BurnSinglePackRetryCnt, 2, offset + 2, 1, 1);
    gridlayout->addWidget(Burn_SinglePackRetryCnt, 2, offset + 3, 1, 1);

    gridlayout->addWidget(Prompt_Hint, 3, 0, 4, 4);
    groupBox->setLayout(gridlayout);
    return groupBox;
}

QGroupBox *BurnTab::DDCMsgGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("DDC Msg"));
    DDC_BurnLog = new QTextEdit(this);

    //QPalette pe;
    //pe.setColor(QPalette::WindowText, Qt::green);
    //DDC_BurnLog->setPalette(pe);
    DDC_BurnLog->setAlignment(Qt::AlignLeft);
    DDC_BurnLog->setCurrentFont(QFont("Times New Roman", 12));

    DDC_BurnLog->setStyleSheet(("background-color:black"));
       // DDC_BurnLog->setStyleSheet("color:white");
    QPalette p=QPalette();
    p.setColor(QPalette::Base,Qt::green);
    DDC_BurnLog->setPalette(p);

    QGridLayout *ddcMsg_Layout = new QGridLayout(this);
    ddcMsg_Layout->addWidget(DDC_BurnLog,0,0,3,4);

    /*
    ddcMsg_Layout->addWidget(DDC_Msg2Send, 6, 0, 1, 6);
    ddcMsg_Layout->addWidget(DDC_Sendbtn, 6, 6, 1, 1);
    ddcMsg_Layout->addWidget(DDC_Clearbtn, 6, 7, 1, 1);
    ddcMsg_Layout->addWidget(DDC_OpenLogs, 6, 8, 1, 1);
    */

    groupBox->setLayout(ddcMsg_Layout);
    return groupBox;
}

void BurnTab::readSettings()
{
    QSettings Edid_Settings("Cvte","DDC tool");
    /*
    Edid_Settings.beginGroup("Edid_setting");
    unsigned char ports = 0;
    ports = (unsigned char)Edid_Settings.value("Edid_Ports", 0).toInt();
    for (int i = 0; i < 8; i++)
    {
        if (i >= 6) break;
        if (ports >> i & 0x0001)
        {
            _edidports[i]->setChecked(true);
        }
    }
    Edid_Settings.endGroup();
    //Edid_CustomerGroup->setId(,tmp_customer);
    qDebug("--customerId %d:", Edid_CustomerGroup->checkedId());
    qDebug("---ports %d:", ports);
    */
    int CustomerID = Edid_Settings.value("Edid_Customer", 0).toInt();
    if (CustomerID >= 3)CustomerID = 3;
    _edidcustomer[CustomerID]->setChecked(true);

    Edid_Settings.beginGroup("Burn_setting");
    int tmp_I2cSpeed = Edid_Settings.value("Burn_I2cSpeed", 5).toInt();
    int tmp_WriteDelay = Edid_Settings.value("Burn_writeDelay", 300).toInt();
    int tmp_ReadDelay = Edid_Settings.value("Burn_readDelay", 200).toInt();
    int tmp_RetryCnt = Edid_Settings.value("Burn_RetryCnt", 3).toInt();
    int tmp_PerPackRetryCnt = Edid_Settings.value("Burn_PerPackRetryCnt",3).toInt();
    int tmp_EdidlastDelay = Edid_Settings.value("Burn_EdidlastDelay", 600).toInt();
    int tmp_HdcplastDelay = Edid_Settings.value("Burn_HdcplastDelay", 600).toInt();
    int tmp_EraseHdcp = Edid_Settings.value("Burn_eraseHdcpkeyDelay", 1000).toInt();
    int tmp_IsCreatLogs =Edid_Settings.value("Burn_isCreatlogs", 0).toInt();

    cvt_burnsetting = { //The class init constructor list
        tmp_I2cSpeed,
        tmp_WriteDelay,
        tmp_ReadDelay,
        tmp_RetryCnt,
        tmp_PerPackRetryCnt,
        tmp_EdidlastDelay,
        tmp_HdcplastDelay,
        tmp_EraseHdcp,
        (bool)tmp_IsCreatLogs,
    };

    Burn_IICSpeedSpin->setValue(tmp_I2cSpeed);
    Burn_EraseHdcpDealy->setValue(tmp_EraseHdcp);
    Burn_WriteDelay->setValue(tmp_WriteDelay);
    Burn_ReadDelay->setValue(tmp_ReadDelay);
    Burn_RetryCnt->setValue(tmp_RetryCnt);
    Burn_SinglePackRetryCnt->setValue(tmp_PerPackRetryCnt);
    Burn_EdidLastPackDelay->setValue(tmp_EdidlastDelay);
    Burn_HdcpLastPackDelay->setValue(tmp_HdcplastDelay);

    Edid_Settings.endGroup();
}

void BurnTab::DDC_GetBurnSettings()
{
    cvt_burnsetting.setI2cSpeed(Burn_IICSpeedSpin->value());
    cvt_burnsetting.setwriteDelay(Burn_WriteDelay->value());
    cvt_burnsetting.setreadDelay(Burn_ReadDelay->value());
    cvt_burnsetting.setRetryCnt(Burn_RetryCnt->value());
    cvt_burnsetting.setPerpackRetryCnt(Burn_SinglePackRetryCnt->value());
    cvt_burnsetting.setEdidlastDelay(Burn_EdidLastPackDelay->value());
    cvt_burnsetting.setHdcplastDelay(Burn_HdcpLastPackDelay->value());
    cvt_burnsetting.setEraseHdcpDelay(Burn_EraseHdcpDealy->value());
}

void BurnTab::writeSettings()
{
    QSettings Edid_Settings("Cvte", "DDC tool");
    /*
    unsigned char tmp_ports = 0;
    Edid_Settings.beginGroup("Edid_setting");

    getEdidportsCheckbox(&tmp_ports);
    Edid_Settings.setValue("Edid_Ports",tmp_ports);
    Edid_Settings.endGroup();
    qDebug("saved ports %d:", tmp_ports);
    */
    int CustomerID = Edid_CustomerGroup->checkedId();
    Edid_Settings.setValue("Edid_Customer",CustomerID-1);

    Edid_Settings.beginGroup("Burn_setting");

    int tmp_I2cSpeed = Burn_IICSpeedSpin->value();
    int tmp_WriteDelay = Burn_WriteDelay->value();
    int tmp_ReadDelay = Burn_ReadDelay->value();
    int tmp_RetryCnt = Burn_RetryCnt->value();
    int tmp_PerPackRetryCnt = Burn_SinglePackRetryCnt->value();
    int tmp_EdidlastDelay = Burn_EdidLastPackDelay->value();
    int tmp_HdcplastDelay = Burn_HdcpLastPackDelay->value();
    int tmp_EraseHdcp = Burn_EraseHdcpDealy->value();

    Edid_Settings.setValue("Burn_I2cSpeed", tmp_I2cSpeed);
    Edid_Settings.setValue("Burn_eraseHdcpkeyDelay", tmp_EraseHdcp);
    Edid_Settings.setValue("Burn_writeDelay", tmp_WriteDelay);
    Edid_Settings.setValue("Burn_readDelay", tmp_ReadDelay);
    Edid_Settings.setValue("Burn_RetryCnt", tmp_RetryCnt);
    Edid_Settings.setValue("Burn_PerPackRetryCnt", tmp_PerPackRetryCnt);
    Edid_Settings.setValue("Burn_EdidlastDelay", tmp_EdidlastDelay);
    Edid_Settings.setValue("Burn_HdcplastDelay", tmp_HdcplastDelay);

    Edid_Settings.endGroup();
}

void BurnTab::closeEvent(QCloseEvent *event)
{
   // writeSettings();
    qDebug() << QString::fromLocal8Bit("close Burn Tab");
}

void BurnTab::getEdidportsCheckbox(unsigned char *ports)
{
    unsigned char port_num = 0;
    for (int i=0;i<EdidPorts_Num;i++)
    {
        if (_edidports[i]->isChecked())
        {
            port_num |= BIT(i);
        }
    }
    *ports=port_num;
}

void BurnTab::getEdidCustomer(Edid_Customer customer)
{

}

//The Slots functions
void BurnTab::Edid_BurnAllportsEdid()
{
    unsigned char ports;
    Prompt_Hint->setText(tr(""));
    if (cvt_edid == NULL)
    {
        QMessageBox::warning(this, tr("Tips"), tr("Please Select the EDID directory."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if(cvt_I2cdevice.gethandle()==NULL)
    {
        QMessageBox::warning(this, tr("Tips"), tr("Please Connect the Device first."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    getEdidportsCheckbox(&ports);
    qDebug("selected ports %d:", ports);

    if (ports == 0)
    {
        QMessageBox::warning(this, tr("Tips"), tr("Please select at least one EDID port."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (ports > edidports)
    {
        QMessageBox message(QMessageBox::NoIcon, "Tips", "The selected ports doesn't match the actuall Edid bin files");
        message.exec();
        return;
    }

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::green);
    Prompt_Hint->setPalette(pe);
    Prompt_Hint->setText(tr("Burning EDIDs..."));

    memset(&cvt_flowcontrol,1,sizeof(cvt_flowcontrol));//initialize the flow control struct.
    cvt_flowcontrol.isErasehdcp = 0;

    //both of following way can do well. 基类指针可以不进行显式类型转换的情况下直接指向派生类对象，基类引用也可以不经过显式类型转换直接引用派生类对象，
    //cvt_transferdata = static_cast<Cvt_BurnData*>(cvt_edid);
    //Cvt_BurnData **burnDatas = new Cvt_BurnData *[EdidPorts_Num];

    //cvt_transferdata = cvt_edid;
    //cvt_transferdata->setdatatype(DATATYPE_EDID_VGA);

    //派生类必须使用基类的构造函数，如果不显示调用，默认会调用基类的默认构造函数。
    //派生类构造函数应该通过成员初始化列表将基类的信息传递给基类构造函数，就像下面这样。
    //派生类构造函数应该初始化派生类新增的数据成员(比如 edidports)
    EdiddataThread = new Cvt_DataManage(ports, cvt_edid,&cvt_flowcontrol, &cvt_burnsetting,&cvt_I2cdevice);
    DDC_EnableDisableMenuItems(true);
    connect(EdiddataThread, SIGNAL(finished()), this, SLOT(DDC_FinishBurnEdid()));
    connect(EdiddataThread, SIGNAL(EdidBurnMsg(int)), this, SLOT(DDC_EdidBurningInfo(int)));
    //connect(EdiddataThread, SIGNAL(BurnMsg(int)), this, SLOT(DDC_HdcpBurningInfo(int)));
    EdiddataThread->start();
}

void BurnTab::Edid_GetAllportsEdid()
{
    unsigned char ports;
    getEdidportsCheckbox(&ports);
    qDebug("selected ports %d:", ports);

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::green);
    Prompt_Hint->setPalette(pe);
    Prompt_Hint->setText(tr("Get EDID..."));
    if (ports == 0)
    {
        QMessageBox::warning(this, tr("Tips"),tr("Please select at least one EDID port."),QMessageBox::Ok,QMessageBox::Ok);
    }
    if (ports > edidports)
    {
        QMessageBox message(QMessageBox::NoIcon, "Tips", "The selected ports doesn't match the actuall Edid bin files");
        message.exec();
    }
}

void BurnTab::Hdcp_BurnHdcpkey()
{
    Prompt_Hint->setText(tr(""));
    if (cvt_I2cdevice.gethandle() == NULL)
    {
        QMessageBox::warning(this, tr("Tips"), tr("Please Connect the Device first."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (cvt_hdcpkey == NULL)
    {
        QMessageBox::warning(this, tr("Tips"), tr("Please Select the Hdcpkey first."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    memset(&cvt_flowcontrol,1,sizeof(cvt_flowcontrol));//initialize the flow control struct.

    //both of following way can do well.
    cvt_transferdata = static_cast<Cvt_BurnData*>(cvt_hdcpkey);
    //cvt_transferdata = cvt_hdcpkey;
    cvt_burnsetting.setSlaveaddr(0x6E);
    cvt_transferdata->setdatatype(DATATYPE_HDCPKEY);

    DDC_EnableDisableMenuItems(true);

    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::green);
    Prompt_Hint->setPalette(pe);
    Prompt_Hint->setText(tr("Burning HDCPKEY..."));

    HdcpkeydataThread = new Cvt_DataThread(&cvt_flowcontrol,&cvt_burnsetting,cvt_transferdata,&cvt_I2cdevice);
    connect(HdcpkeydataThread, SIGNAL(finished()), this, SLOT(DDC_FinishBurnHdcpkey()));
    connect(HdcpkeydataThread, SIGNAL(BurnMsg(int)), this, SLOT(DDC_HdcpBurningInfo(int)));
    HdcpkeydataThread->start();
}

void BurnTab::Hdcp_GetHdcpkey()
{
    Prompt_Hint->setText(tr(""));
    if (cvt_hdcpkey == NULL)
    {
        cvt_hdcpkey = new Cvt_HDCPKey(1024);
    }

    memset(&cvt_flowcontrol, 0, sizeof(cvt_flowcontrol));//initialize the flow control struct.
    cvt_flowcontrol.isRead = 1;

    //both of following way can do well.
    cvt_transferdata = static_cast<Cvt_BurnData*>(cvt_hdcpkey);
    //cvt_transferdata = cvt_hdcpkey;

    cvt_burnsetting.setSlaveaddr(0x6E);
    //cvt_transferdata->setdatatype(DATATYPE_HDCPKEY);
    cvt_transferdata->setdatatype(DATATYPE_EDID_VGA);

    HdcpkeydataThread = new Cvt_DataThread(&cvt_flowcontrol, &cvt_burnsetting, cvt_transferdata, &cvt_I2cdevice);
    HdcpkeydataThread->start();
    qDebug() << QString::fromLocal8Bit("Get HDCPKEY");
}

//DDC Slots set

void BurnTab::DDC_EnableDisableMenuItems(bool isDisable)
{
    LoadHdcpkeyBtn->setDisabled(isDisable);
    LoadEDIDBtn->setDisabled(isDisable);
    //Edid_Getedidbtn->setDisabled(isDisable);
    Edid_Burnedidbtn->setDisabled(isDisable);
    Hdcp_Burnhdcpbtn->setDisabled(isDisable);
    //Hdcp_Gethdcpbtn->setDisabled(isDisable);
}

void BurnTab::DDC_EnableDisableDeviceItems(bool isDisable)
{
    Burn_Connectbtn->setDisabled(isDisable);

    Burn_IICSpeedSpin->setDisabled(isDisable);
    Burn_EraseHdcpDealy->setDisabled(isDisable);
    Burn_EdidLastPackDelay->setDisabled(isDisable);
    Burn_HdcpLastPackDelay->setDisabled(isDisable);
    Burn_WriteDelay->setDisabled(isDisable);
    Burn_ReadDelay->setDisabled(isDisable);
    Burn_RetryCnt->setDisabled(isDisable);
    Burn_SinglePackRetryCnt->setDisabled(isDisable);
}

void BurnTab::DDC_FinishBurnEdid()
{
    qDebug() << QString::fromLocal8Bit("Burn EDID finished");
    DDC_EnableDisableMenuItems(false);
    disconnect(EdiddataThread);
    delete EdiddataThread;
}

void BurnTab::DDC_EdidBurningInfo(int num)
{
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::green);
    Prompt_Hint->setPalette(pe);
    switch (num)
    {
    default:
    case 0://Burn VGA
        Prompt_Hint->setText(tr("Ready to Burn VGA"));
        break;
    case 1://Burn DVI
        Prompt_Hint->setText(tr("Ready to Burn DVI"));
        break;
    case 2://Burn HDMI1
        Prompt_Hint->setText(tr("Ready to Burn HDMI1"));
        break;
    case 3://Burn HDMI2
        Prompt_Hint->setText(tr("Ready to Burn HDMI2"));
        break;
    case 4://Burn HDMI3
        Prompt_Hint->setText(tr("Ready to Burn HDMI3"));
        break;
    case 5: //Burn DP
        Prompt_Hint->setText(tr("Ready to Burn DP"));
        break;
    case 10://erase failed
        Prompt_Hint->setText(tr("Erase Failed"));
        break;
    case 11://Burning failed
        Prompt_Hint->setText(tr("Burn Failed"));
        break;
    case 12://read failed
        Prompt_Hint->setText(tr("Read Failed"));
        break;
    case 13://verify failed
        Prompt_Hint->setText(tr("Verify Failed"));
        break;
    case 14://burn hadpckey success
        pe.setColor(QPalette::WindowText, Qt::green);
        Prompt_Hint->setPalette(pe);
        Prompt_Hint->setText(tr("Burn Success"));
        break;
    }
}

void BurnTab::DDC_FinishBurnHdcpkey()
{
    qDebug() << QString::fromLocal8Bit("Burn HDCPKEY finished");
    DDC_EnableDisableMenuItems(false);
    disconnect(HdcpkeydataThread);
    delete HdcpkeydataThread;
}

void BurnTab::DDC_HdcpBurningInfo(int num)
{
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::red);
    Prompt_Hint->setPalette(pe);
    switch (num)
    {
    default:
    case 0://erase failed
        Prompt_Hint->setText(tr("Erase Failed"));
        break;
    case 1://Burning failed
        Prompt_Hint->setText(tr("Burn Failed"));
        break;
    case 2://read failed
        Prompt_Hint->setText(tr("Read Failed"));
        break;
    case 3://verify failed
        Prompt_Hint->setText(tr("Verify Failed"));
        break;
    case 4://burn hadpckey success
        pe.setColor(QPalette::WindowText, Qt::green);
        Prompt_Hint->setPalette(pe);
        Prompt_Hint->setText(tr("Burn Success"));
        break;
    }
}

void BurnTab::DDC_DebugMsgSend()
{

}

void BurnTab::DDC_OpenLogs()
{

}

void BurnTab::DDC_MsgClear()
{

}

void BurnTab::DDC_ConnectDevice()
{
    DDC_GetBurnSettings();
    if (cvt_I2cdevice.openDevice(cvt_I2cdevice.gethandle(), cvt_burnsetting.getI2cSpeed() * 1000) == FTC_SUCCESS)
    {
        DDC_EnableDisableDeviceItems(true);
        qDebug("Open device successfully!!!");
    }
    else
    {
        QMessageBox::warning(this, tr("Tips"), tr("Open Device Failed."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
}

void BurnTab::DDC_DisConnectDevice()
{
    if (!Burn_Connectbtn->isEnabled())
    {
        cvt_I2cdevice.closeDevice(cvt_I2cdevice.gethandle());
        DDC_EnableDisableDeviceItems(false);
    }
    else
    {
        QMessageBox::warning(this, tr("Tips"), tr("Have not yet open device!."), QMessageBox::Ok, QMessageBox::Ok);
    }
}

void filename2datatypeAndIdx(QString file_name, Cvt_DataType &_datatype ,unsigned char &_edidports)
{
    if (file_name == "vga.bin")
    {
        _datatype = DATATYPE_EDID_VGA;
        _edidports |= EDID_VGA;
    }
    else if (file_name == "dvi.bin")
    {
        _datatype = DATATYPE_EDID_DVI;
        _edidports |= EDID_DVI;
    }
    else if (file_name == "hdmi.bin")
    {
        _datatype = DATATYPE_EDID_HDMI1;
        _edidports |= EDID_HDMI1;
    }
    else if (file_name == "hdmi2.bin")
    {
        _datatype = DATATYPE_EDID_HDMI2;
        _edidports |= EDID_HDMI2;
    }
    else if (file_name == "hdmi3.bin")
    {
        _datatype = DATATYPE_EDID_HDMI3;
        _edidports |= EDID_HDMI3;
    }
    else if (file_name == "dp.bin")
    {
        _datatype = DATATYPE_EDID_DP;
        _edidports |= EDID_DP;
    }
}

void BurnTab::LoadEDID()
{
    QStringList  fileNameList;
    QString fileName;
    QFileDialog* fd = new QFileDialog(this, tr("Select the EDID"));
    fd->setFileMode(QFileDialog::Directory);
    //fd->setDirectory(".");
    fd->setViewMode(QFileDialog::List);
    if (fd->exec() == QDialog::Accepted)
    {
        fileNameList = fd->selectedFiles();
        fileName = fileNameList[0];
        if (fileName != NULL)
        {
            EdidDataPath->setText(fileName);
            QDir dir(fileName);
            QStringList Filefullpath;
            QStringList filters;
            //filters << QString("vga.txt") << QString("hdmi1.txt") << QString("dvi.txt") << QString("hdmi.txt") << QString("hdmi2.txt") << QString("dp.txt")
            filters << QString("vga.bin") << QString("dvi.bin") << QString("hdmi.bin") <<QString("hdmi1.bin") << QString("hdmi2.bin")<< QString("dp.bin");
            dir.setNameFilters(filters);
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list = dir.entryInfoList();
            qDebug("Qfileinfolist's size:%d",list.size());

            //get the separtor '/'
            QChar separator = QChar('/');
            if (!fileName.contains(separator))
            {
                separator = QChar('\\');
            }
            QChar last_char = fileName.at(fileName.length() - 1);
            if (last_char == separator)
            {
                separator = QChar();
            }
            edidports = 0;
            Cvt_DataType tmp_type_Idx;
            for (int i = 0; i < list.size(); ++i)
            {
                QFileInfo fileInfo = list.at(i);
                QString file_name = dir[i];
                QString file_path = fileName + separator + file_name;   //文件全路径
                Filefullpath.append(file_path);
                filename2datatypeAndIdx(file_name, tmp_type_Idx,edidports);
                cvt_edid[tmp_type_Idx] = Cvt_EDID(file_path, tmp_type_Idx);//create the edid and append to cvt_edid dynamic class array
                qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(file_path));
                qDebug("filename Idx:%d,DATAEXISTED_IDX:%d", i, tmp_type_Idx);

                //tmp_type_Idx = cvt_edid[tmp_type_Idx].getdatatype();
                //qDebug("File data Type:%d",i, tmp_type_Idx);

                qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(file_path));
            }
            qDebug("EdidPorts num:%x", edidports);

            /*
            qDebug("Edidlist size:%d",cvt_edid[tmp_type_Idx].getLength());
            for (int j = 0; j < 2; ++j)
            {
                unsigned char buf[128];
                cvt_edid[tmp_type_Idx].getdata(j, 128, buf, 128);
                for (int k = 0; k < 128; ++k)
                {
                    qDebug("copy edid data:%x",buf[k]);
                }
            }
            */
            QString tr;
            tr = QString("%1").arg(cvt_edid[tmp_type_Idx].getProductCode(), 4, 16, QLatin1Char('0'));
            EdidManufacturerName->setText(cvt_edid[tmp_type_Idx].getManufacturerName());
            EdidProductCode->setText(tr);
            EdidYear->setText(QString::number(cvt_edid[tmp_type_Idx].getProductYear(), 10));
            EdidWeek->setText(QString::number(cvt_edid[tmp_type_Idx].getProductWeek(), 10));
        }
    }
    else
        fd->close();
}

void BurnTab::LoadHDCPKey()
{
    QStringList  fileNameList;
    QString fileName;

    QFileDialog* fd = new QFileDialog(this,tr("Select the HDCPKey"));
    QString filters = "Binfile(*.bin)";
    fd->setNameFilter(filters);
    fd->setViewMode(QFileDialog::List);
    if (fd->exec() == QDialog::Accepted)
    {
        fileNameList = fd->selectedFiles();
        fileName = fileNameList[0];
        if (fileName != NULL)
        {
            HdcpDataPath->setText(fileName);
            if (cvt_hdcpkey != NULL)
            {
                qDebug("delete the last hdcpkey class");
                delete cvt_hdcpkey;
            }
            cvt_hdcpkey = new Cvt_HDCPKey(fileName);
            qDebug("Length:%d", cvt_hdcpkey->getLength());
            qDebug() << "delete the malloc array and close the window" << endl;
        }
    }
    else
        fd->close();
}

