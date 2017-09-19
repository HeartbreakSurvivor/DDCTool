#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace ddc;


DDCMainWindow::DDCMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui_preinit();

    readSettings();

    i2coptions = new CommunicationOption(*burnsettings);

    ddcprotocol = new DDCProtocol_T(i2cdevice);
    ddcprotocol->setSlaveAddr(((BurnSetting_T&)i2coptions->getsetting()).m_slaveaddr);

    m_transfer = new Transfer_T(*ddcprotocol,((BurnSetting_T&)i2coptions->getsetting()).m_perpackretrycnt);

    updateATcmds(*m_atcmd.front(),0);

    //initialize signals and slots
    //I2C
    connect(ui->actionCommunication, SIGNAL(triggered()), this, SLOT(displayi2coptions()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(displayhelpmenu()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(displayaboutmenu()));
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(connectI2c()));//connect the isp i2c device
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(disconnetI2c()));//disconnect the isp i2c device
    connect(ui->actionLog, SIGNAL(triggered()), this, SLOT(opendebugmsg()));//open the debug msg dialog

    //EDID Tab
    connect(ui->loadedid_Btn, SIGNAL(clicked()), this, SLOT(loadEdid()));//load the edid to ram.
    connect(ui->nextedid_Btn, SIGNAL(clicked()), this, SLOT(nextEdid()));//dispaly the next edid which load to ram before.
    connect(ui->edidSync_Btn, SIGNAL(clicked()), this, SLOT(syncEdid()));//when change the edid data and sync to ram
    ui->edidSync_Btn->setStatusTip("when change the edid data and sync to ram");
    connect(ui->edidSave_Btn, SIGNAL(clicked()), this, SLOT(saveEdid()));//save the edid as a new edid data
    connect(ui->edidWrite_Btn, SIGNAL(clicked()), this, SLOT(writeEdid()));//write the edid to board
    connect(ui->edidStop_Btn, SIGNAL(clicked()), this, SLOT(stopWriteEdid()));//stop the edid writing operation

    //HDCP Tab
    connect(ui->loadhdcp_Btn, SIGNAL(clicked()), this, SLOT(loadHdcp()));//load the hdcp to ram.
    connect(ui->writehdcp_Btn, SIGNAL(clicked()), this, SLOT(writeHdcp()));//write the hdcp to board
    connect(ui->stopwritehdcp_Btn, SIGNAL(clicked()), this, SLOT(stopWriteHdcp()));//stop the hdcp writing operation
    connect(ui->chiptypecomboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changechiptype()));

    //Debug Tab
    connect(ui->instructionup_Btn,SIGNAL(clicked()),this,SLOT(cmdup()));
    connect(ui->instructiondowm_Btn,SIGNAL(clicked()),this,SLOT(cmddown()));
    connect(ui->instructionstep_Btn,SIGNAL(clicked()),this,SLOT(cmdstep()));
    connect(ui->instructionrun_Btn,SIGNAL(clicked()),this,SLOT(cmdrun()));
    connect(ui->cmdsend_Btn,SIGNAL(clicked()),this,SLOT(cmdsend()));
    connect(ui->cmdclear_Btn,SIGNAL(clicked()),this,SLOT(cmdclear()));
    connect(ui->instructionsetlistWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
    //fuck. the connect func's parameter SIGNAL and SLOT can't own any parameter.
    connect(ddcprotocol,SIGNAL(start_emit(QString)),this,SLOT(updatelogText(QString)));
}

DDCMainWindow::~DDCMainWindow()
{
    delete ui;
    delete i2coptions;
    delete ddcprotocol;
    delete m_transfer;
}

void DDCMainWindow::ui_preinit()
{
    QLabel *DDC_BurnStatusText = new QLabel(tr("Status:"));

    DDC_ProgressBar = new QProgressBar(this);
    DDC_ProgressBar->setMinimum(0);
    DDC_ProgressBar->setMaximum(100);
    DDC_ProgressBar->setValue(23);

    DDC_BurnStatus = new QLabel(tr("PASS"));

    DDC_BurnStatus->setMinimumWidth(80);
    DDC_BurnStatus->setAlignment(Qt::AlignCenter);
    QFont font("Helvetica [Cronyx]", 22, QFont::Black, false);
    DDC_BurnStatus->setFont(font);
    DDC_BurnStatus->setStyleSheet("color:green");

    DDC_TimeLabel = new QLabel(tr(""));
    ui->statusBar->addPermanentWidget(DDC_TimeLabel);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(qTimeSlot()));
    timer->start(1000);

    ui->statusBar->addWidget(DDC_BurnStatusText);
    ui->statusBar->addWidget(DDC_ProgressBar);
    ui->statusBar->addWidget(DDC_BurnStatus);

    //edid tab
    ui->manufacturerNameLineEdit->setValidator(new QRegExpValidator(QRegExp("[A-Z]{3}"), this));
    ui->manufacturerCodeLineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9A-Fa-f]{4}$"),this));
    ui->manufacturerWeekLineEdit->setValidator(new QRegExpValidator(QRegExp("^[1-9]{1,2}$"),this));
    ui->manufacturerYearLineEdit->setValidator(new QRegExpValidator(QRegExp("^[1-9][0-9]{3}$"),this));
    ui->customerSNlineEdit->setValidator(new QRegExpValidator(QRegExp("^[A-Z0-9-]{1,13}$"),this));
    ui->EdidtableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //hdcp tab
    ui->hdcpkeyidlineEdit->setMaxLength(8);
    ui->hdcptableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //debug tab
    for(int i=0;i<ddc::getATCmdLen();++i)
    {
        m_atcmd.push_back(ATCmds[i]);
        ui->instructionsetlistWidget->insertItem(i+1,new QListWidgetItem(ATCmds[i]->name,ui->instructionsetlistWidget));
    }

    ui->propertytableWidget->setItem(0,0,new QTableWidgetItem("Param"));
    ui->propertytableWidget->setItem(1,0,new QTableWidgetItem("Retry"));
    ui->propertytableWidget->setItem(2,0,new QTableWidgetItem("Timeout"));

    retryspbox.setMaximum(10);
    delayspbox.setMaximum(10000);
    paralineedit.setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f ]{1,}$"),this));
    ui->propertytableWidget->setCellWidget(0,1,&paralineedit);
    ui->propertytableWidget->setCellWidget(1,1,&retryspbox);
    ui->propertytableWidget->setCellWidget(2,1,&delayspbox);

    ui->descriptionplainTextEdit->setReadOnly(true);
    ui->instructiondatatableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->cmdlineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f ]{1,}$"),this));
    ui->cmdlineEdit->setPlaceholderText("Send datagram,i.e.,\"C0 63 07 04 00 01\" to reset monitor.");
}

void DDCMainWindow::readSettings()
{
    QSettings Burn_Settings("Cvte","DDC Tool");

    Burn_Settings.beginGroup("Burn_setting");
    quint8 tmp_slaveaddr = Burn_Settings.value("Burn_SlaveAddr",0x6E).toInt();
    int tmp_I2cSpeed = Burn_Settings.value("Burn_I2cSpeed", 5).toInt();
    int tmp_WriteDelay = Burn_Settings.value("Burn_writeDelay", 300).toInt();
    int tmp_ReadDelay = Burn_Settings.value("Burn_readDelay", 200).toInt();
    int tmp_RetryCnt = Burn_Settings.value("Burn_RetryCnt", 3).toInt();
    int tmp_PerPackRetryCnt = Burn_Settings.value("Burn_PerPackRetryCnt",3).toInt();
    int tmp_EdidlastDelay = Burn_Settings.value("Burn_EdidlastDelay", 600).toInt();
    int tmp_HdcplastDelay = Burn_Settings.value("Burn_HdcplastDelay", 600).toInt();
    int tmp_EraseHdcp = Burn_Settings.value("Burn_eraseHdcpkeyDelay", 444).toInt();
    int tmp_IsCreatLogs =Burn_Settings.value("Burn_isCreatlogs", 0).toInt();

    burnsettings = new BurnSetting_T(tmp_slaveaddr,tmp_I2cSpeed,tmp_WriteDelay,tmp_ReadDelay,tmp_RetryCnt,tmp_PerPackRetryCnt,
    tmp_EdidlastDelay,tmp_HdcplastDelay,tmp_EraseHdcp,(bool)tmp_IsCreatLogs);

    Burn_Settings.endGroup();

    QSettings At_cmds("Cvte","DDC Tool");
    At_cmds.beginGroup("AT_Cmd");

    At_cmds.endGroup();
}

void DDCMainWindow::writeSettings()
{
    QSettings Burn_Settings("Cvte", "DDC Tool");

    //const修饰返回值的函数，必须定义const类型的变量去赋值，或者强制转化成非const类型
    //const BurnSetting_T &burnsetting = i2coptions->getsetting();

    BurnSetting_T &i2csettings = (BurnSetting_T&)i2coptions->getsetting();
    //qDebug("the i2c speed:%d",i2csettings.getI2cSpeed());

    Burn_Settings.beginGroup("Burn_setting");

    Burn_Settings.setValue("Burn_SlaveAddr",i2csettings.getSlaveaddr());
    Burn_Settings.setValue("Burn_I2cSpeed", i2csettings.getI2cSpeed());
    Burn_Settings.setValue("Burn_eraseHdcpkeyDelay", i2csettings.getEraseHdcpDelay());
    Burn_Settings.setValue("Burn_writeDelay", i2csettings.getwriteDelay());
    Burn_Settings.setValue("Burn_readDelay", i2csettings.getreadDelay());
    Burn_Settings.setValue("Burn_RetryCnt", i2csettings.getRetryCnt());
    Burn_Settings.setValue("Burn_PerPackRetryCnt", i2csettings.getPerpackRetryCnt());
    Burn_Settings.setValue("Burn_EdidlastDelay", i2csettings.getEdidlastDelay());
    Burn_Settings.setValue("Burn_HdcplastDelay", i2csettings.getHdcplastDelay());
    Burn_Settings.setValue("Burn_isCreatlogs",i2csettings.getCreatlogs());

    Burn_Settings.endGroup();
}

void DDCMainWindow::clearEdidTab()
{
    ui->edidnameLineEdit->clear();
    ui->edidsizeLineEdit->clear();
    ui->manufacturerNameLineEdit->clear();
    ui->manufacturerCodeLineEdit->clear();
    ui->manufacturerSNLineEdit->clear();
    ui->manufacturerWeekLineEdit->clear();
    ui->manufacturerYearLineEdit->clear();
    ui->edidVersionLineEdit->clear();
    ui->customerSNlineEdit->clear();
    ui->EdidtableWidget->clear();
}

void DDCMainWindow::updateEdidTab(QString key)
{
    int row=-1,column=0;

    clearEdidTab();

    ui->edidnameLineEdit->setText(key.toUpper());
    ui->edidsizeLineEdit->setText(QString::number(edid_map[key]->getLength()));

    ui->manufacturerNameLineEdit->setText(edid_map[key]->getManufacturerName());
    ui->manufacturerCodeLineEdit->setText(QString("%1").arg(edid_map[key]->getProductCode(), 4, 16, QLatin1Char('0')));
    ui->manufacturerSNLineEdit->setText(QString("%1").arg(edid_map[key]->getManufacturerSN(), 8, 16, QLatin1Char('0')));
    ui->manufacturerWeekLineEdit->setText(QString::number(edid_map[key]->getProductWeek()));
    ui->manufacturerYearLineEdit->setText(QString::number(edid_map[key]->getProductYear()));
    ui->edidVersionLineEdit->setText(edid_map[key]->getVersion());
    ui->customerSNlineEdit->setText(edid_map[key]->getProductSN());

    for(int sz=0;sz<edid_map[key]->getLength();++sz)
    {
        QString str = QString("%1").arg((edid_map[key]->data[sz])&0xFF,2,16,QLatin1Char('0'));
        QTableWidgetItem *newItem = new QTableWidgetItem(str.toUpper());
        if(sz%16==0)
        {
            row++;
            column=0;
        }
        ui->EdidtableWidget->setItem(row, column++, newItem);
        //std::cout<<"row:"<<row<<"column:"<<column;
    }
    if (edid_map[key]->getLength() == 128)
    {
        qDebug()<<"uncheck";
        //ui->EdidtableWidget->item(8,0)->setCheckState(Qt::Unchecked);
    }
}

void DDCMainWindow::updateHdcpTab()
{
    QString keyid;
    int row=-1,column=0;

    std::cout<<"current idx:"<<ui->chiptypecomboBox->currentIndex();
    hdcpdata->setChipType(ui->chiptypecomboBox->currentIndex());
    if(hdcpdata==nullptr)
    {
        QMessageBox::warning(this, tr("Tips"), tr("Select HDCPKey first please."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    quint8 *pkeyid = hdcpdata->getKeyid();
    for(int i=0;i<8;i++)
    {
        //qDebug()<<"char:"<<pkeyid[i];
        keyid.append(QChar(*(pkeyid+i)));
    }

    ui->hdcpsizelineEdit->setText(QString::number(hdcpdata->getLength(),10));
    ui->hdcpkeyidlineEdit->setText(keyid);
    ui->hdcptableWidget->clear();
    for(int sz=0;sz<hdcpdata->getLength();++sz)
    {
        QString str = QString("%1").arg((hdcpdata->data[sz])&0xFF,2,16,QLatin1Char('0'));
        QTableWidgetItem *newItem = new QTableWidgetItem(str.toUpper());
        if(sz%16==0)
        {
            row++;
            column=0;
        }
        ui->hdcptableWidget->setItem(row, column++, newItem);
        //qDebug()<<"row:"<<row<<"column:"<<column;
    }
}

void DDCMainWindow::updateATcmds(const burnCmd_t& cmd,int options)
{
    if(options) goto cmddatasonly;
    paralineedit.clear();
    if(cmd.setparafunc != nullptr)
    {
        paralineedit.setReadOnly(false);
        if(cmd.name == "Write SN")
        {
            paralineedit.setValidator(new QRegExpValidator(QRegExp("[0-9A-Za-z-]{1,20}$"),this));//remove the validator.
        }
        else
        {
            paralineedit.setValidator(new QRegExpValidator(QRegExp("[0-9A-Fa-f ]{1,}$"),this));
        }
    }
    else
    {
        paralineedit.setReadOnly(true);
    }

    retryspbox.setValue(cmd.retrycnt);
    delayspbox.setValue(cmd.delay);
    ui->descriptionplainTextEdit->setPlainText(cmd.description);

cmddatasonly:
    ui->instructiondatatableWidget->clear();

    quint8 *tmpdata = new quint8[cmd.datalen+1];
    *tmpdata = cmd.datalen|0x80;
    memcpy(tmpdata+1,cmd.burndata,cmd.datalen);

    int row=-1,column=0;
    for(int sz=0;sz<cmd.datalen+1;++sz)
    {
        QString str = QString("%1").arg((tmpdata[sz])&0xFF,2,16,QLatin1Char('0'));
        QTableWidgetItem *newItem = new QTableWidgetItem(str.toUpper());
        if(sz%8==0)
        {
            row++;
            column = 0;
        }
        //std::cout<<"row:"<<row<<"column:"<<column;
        ui->instructiondatatableWidget->setItem(row, column++, newItem);
    }
    delete[] tmpdata;
}

//slots
void DDCMainWindow::updatelogText(QString msg)
{
    ui->logtextBrowser->append(msg);
}

void DDCMainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    writeSettings();
}

void DDCMainWindow::qTimeSlot(void)
{
    QDateTime DDC_CurTime = QDateTime::currentDateTime();
    QString str = DDC_CurTime.toString("yyyy-MM-dd hh:mm:ss");

    DDC_TimeLabel->setText(str);
}

//I2C Options
void DDCMainWindow::displayaboutmenu(void)
{
    helpwindow.show();
}

void DDCMainWindow::displayhelpmenu(void)
{

}

void DDCMainWindow::displayi2coptions(void)
{
    i2coptions->show();
}

//Isp Slots
void DDCMainWindow::connectI2c(void)
{
    //const修饰返回值的函数，必须定义const类型的变量去赋值，或者强制转化成非const类型
    //const BurnSetting_T &burnsetting = i2coptions->getsetting();
    BurnSetting_T &burnsetting = (BurnSetting_T&)i2coptions->getsetting();
    qDebug("the i2c speed:%d",burnsetting.getI2cSpeed());
    if (i2cdevice.openDevice(i2cdevice.gethandle(),burnsetting.getI2cSpeed()*1000.0f) == FTC_SUCCESS)
    {
        ui->actionConnect->setDisabled(true);
    }
    else
    {
        QMessageBox::warning(this, tr("Tips"), tr("Open Device Failed."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
}

void DDCMainWindow::disconnetI2c(void)
{
    if (NULL==i2cdevice.gethandle())
    {
        QMessageBox::warning(this, tr("Tips"), tr("pleas open device first!!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    ui->actionConnect->setDisabled(false);
    i2cdevice.closeDevice(i2cdevice.gethandle());
    qDebug("Close device successfully!!");
}

void DDCMainWindow::opendebugmsg(void)
{
    QString logpath = QDir::currentPath();
    QDesktopServices::openUrl(QUrl(logpath,QUrl::TolerantMode));
}

//EDID Tab Slots
void DDCMainWindow::loadEdid(void)
{
    QStringList  fileNameList;
    QString fileName;
    QFileDialog* fd = new QFileDialog(this, tr("Select the EDID"));
    fd->setFileMode(QFileDialog::Directory);//chose the directory
    fd->setDirectory(".");
    fd->setViewMode(QFileDialog::List);
    if (fd->exec() == QDialog::Accepted)
    {
        qDebug()<<"map's size :"<<edid_map.size();
        if(!edid_map.empty())
        {
            qDebug()<<"clear the edid map";
            for(map<QString,Edid_T*>::iterator it=edid_map.begin();it!=edid_map.end();++it)
            {
                qDebug()<<"destory the edid data:"<<it->first;
                delete it->second;
            }
            edid_map.clear();
        }
        fileNameList = fd->selectedFiles();
        fileName = fileNameList[0];
        if (fileName != NULL)
        {
            QDir dir(fileName);
            QStringList filters;
            filters << QString("vga.bin") << QString("dvi.bin") << QString("hdmi.bin") <<QString("hdmi1.bin") << QString("hdmi2.bin")<< QString("dp.bin");
            dir.setNameFilters(filters);
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list = dir.entryInfoList();

            if(list.size()==0)
            {
                qDebug()<<"there is no edid data";
                ui->edidpathLineEdit->setText(fileName);
                clearEdidTab();
                return;
            }

            QChar separator = QChar('/');//get the separtor '/'
            if (!fileName.contains(separator))
            {
                separator = QChar('\\');
            }
            QChar last_char = fileName.at(fileName.length() - 1);
            if (last_char == separator)
            {
                separator = QChar();
            }

            QString key;
            for (int i = 0; i < list.size(); ++i)
            {
                QString file_name = dir[i];
                QString file_path = fileName + separator + file_name;//the file full path
                key = file_name.left(file_name.indexOf(QString(".bin")));//get the file's pure name

                //create the edid map
                edid_map[key] = new Edid_T(file_path);
                //qDebug() <<"key:"<< key;
                //qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(file_path));
            }
            ui->edidpathLineEdit->setText(fileName);
            Cur_Key = key;
            updateEdidTab(Cur_Key);
        }
    }
    else
        fd->close();
}

void DDCMainWindow::nextEdid(void)
{
    cout<<"next edid bin file"<<endl;
    if (edid_map.empty()) return;
    map<QString,Edid_T*>::iterator it=edid_map.find(Cur_Key);
    qDebug() <<"Cur_key:"<< it->first;
    it++;
    if (it == edid_map.end())
    {
        it = edid_map.begin();
        qDebug()<<"turn to the begin";
    }
    Cur_Key = it->first;
    updateEdidTab(Cur_Key);
}

//judge the v1 is the subset of v2 or not.
//there must be a more effective way to calculate the subset
bool IsSubset(std::vector<QString> &v1,std::vector<QString> &v2)
{
    if(v2.size()>v1.size())
        return false;
    quint32 i=0,j=0;
    std::sort(v1.begin(),v1.end());
    std::sort(v2.begin(),v2.end());
    while(i<v1.size()&&j<v2.size())
    {
        if(v1[i]<v2[j])
        {
            i++;
        }
        else if(v1[i]==v2[j])
        {
            i++;
            j++;//if the same and the j++.
        }
        else if(v1[i]>v2[j])
        {
            return false;
        }
    }
    if(j<v2.size())//j hasn't been the end of v2
        return false;
    else
        return true;
}

void DDCMainWindow::getedidtypes()
{
    edid_type.clear();
    if(ui->checkBox_vga->isChecked())
       edid_type.push_back("vga");
    if(ui->checkBox_dvi->isChecked())
        edid_type.push_back("dvi");
    if(ui->checkBox_hdmi1->isChecked())
        edid_type.push_back("hdmi");
    if(ui->checkBox_hdmi2->isChecked())
        edid_type.push_back("hdmi2");
    if(ui->checkBox_hdmi3->isChecked())
        edid_type.push_back("hdmi3");
    if(ui->checkBox_dp->isChecked())
        edid_type.push_back("dp");
}

void DDCMainWindow::syncEdid(void)
{
    bool ok;
    cout<<"sync edid"<<endl;
    if (edid_map.empty()) return;
    //find the current edid.
    map<QString,Edid_T*>::iterator it=edid_map.find(Cur_Key);
    qDebug() <<"Cur_key:"<< it->first;

    //manufacturer name
    QString name = ui->manufacturerNameLineEdit->text();
    it->second->setManufacturerName(name);

    //manufacturer code
    int code = ui->manufacturerCodeLineEdit->text().toInt(&ok, 16);
    it->second->setProductCode(code);

    //year and week
    int year = ui->manufacturerYearLineEdit->text().toInt(&ok, 10);
    int week = ui->manufacturerWeekLineEdit->text().toInt(&ok, 10);
    if(year<1990)
    {
        QMessageBox::warning(this, tr("Tips"), tr("The year must more than 1990 at least!"),QMessageBox::Ok);
    }
    if(week>52)
    {
        QMessageBox::warning(this, tr("Tips"), tr("you gonna be kiding me,how's it possible that a year has more than 52 weeks?"),QMessageBox::Ok);
    }
    it->second->setProductYear((quint8)(year-1990));
    it->second->setProductWeek((quint8)(week));

    //product serial number
    QString sn = ui->customerSNlineEdit->text();
    it->second->setProductSN(sn);

    it->second->recalchecksum();
    updateEdidTab(Cur_Key);
}

void DDCMainWindow::saveEdid(void)
{
    cout<<"save edid bin file"<<endl;
    edid_map[Cur_Key]->syncfile();
}

void DDCMainWindow::writeEdid(void)
{
    qDebug()<<"write Edid start!"<<endl;
    if (NULL==i2cdevice.gethandle() || edid_map.size()==0)return;
    std::vector<QString> tmp;
    for(map<QString,Edid_T*>::iterator it=edid_map.begin();it!=edid_map.end();++it)
    {
        tmp.push_back(it->first);
    }
    getedidtypes();
    if(edid_type.size()==0)
    {
        QMessageBox::warning(this, tr("Tips"), tr("Please select at least one edid to burn!"),QMessageBox::Ok);
        return;
    }
    if(!IsSubset(tmp,edid_type))
    {
        QMessageBox::warning(this, tr("Tips"), tr("The selected edid doesn't match the real edid!"),QMessageBox::Ok);
        return;
    }
    qDebug()<<"write Edid end"<<endl;
}

void DDCMainWindow::stopWriteEdid(void)
{
    cout<<"stop the writing of edid"<<endl;
}

//Hdcp Tab Slots
void DDCMainWindow::loadHdcp()
{
    QStringList  fileNameList;
    QString fileName;

    QFileDialog* fd = new QFileDialog(this,tr("Select the HDCPKey"));
    QString filters = "Binfile(*.bin)";
    fd->setNameFilter(filters);
    fd->setViewMode(QFileDialog::List);
    fd->setDirectory(".");
    if (fd->exec() == QDialog::Accepted)
    {
        fileNameList = fd->selectedFiles();
        fileName = fileNameList[0];
        if (fileName != NULL)
        {
            ui->hdcppathLineEdit->setText(fileName);

            if (hdcpdata != NULL)
            {
                delete hdcpdata;
                qDebug("delete the last hdcpkey");
            }
            hdcpdata = new Hdcp_T(fileName);
            qDebug("Length:%d", hdcpdata->getLength());
            updateHdcpTab();
        }
    }
    else
        fd->close();
}

void DDCMainWindow::writeHdcp()
{
    if (NULL==i2cdevice.gethandle() || hdcpdata==nullptr)return;
}

void DDCMainWindow::stopWriteHdcp()
{

}

void DDCMainWindow::changechiptype()
{
    qDebug()<<"current idx:"<<ui->chiptypecomboBox->currentIndex();
    hdcpdata->setChipType(ui->chiptypecomboBox->currentIndex());
    quint8 *pkeyid = hdcpdata->getKeyid();
    QString keyid;
    for(int i=0;i<8;i++)
    {
        qDebug()<<"char:"<<pkeyid[i];
        keyid.append(QChar(*(pkeyid+i)));
    }
    ui->hdcpkeyidlineEdit->setText(keyid);
}

//Debug Tab Slots
void DDCMainWindow::itemClicked(QModelIndex idx)
{
    std::list<burnCmd_t*>::iterator it=m_atcmd.begin();
    advance(it,idx.row());
    Cur_cmd = idx.row();
    burnCmd_t* tmp = *it;
    updateATcmds(*tmp,0);
}

void DDCMainWindow::cmdclear()
{
    ui->cmdlineEdit->clear();
    ui->logtextBrowser->clear();
}

void DDCMainWindow::cmdsend()
{
    if (NULL==i2cdevice.gethandle())
    {
        QMessageBox::warning(this, tr("Tips"), tr("pleas open device first!!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    QString CmdStr = ui->cmdlineEdit->text();
    //first:find all the substring and judge the length. must less than 2
    QStringList cmdlist = CmdStr.split(' ', QString::SkipEmptyParts);

    ui->logtextBrowser->setTextColor(QColor(Qt::red));
    for(auto x:cmdlist)
    {
        if(x.length()>2)
        {
            ui->logtextBrowser->append("Commands format error.");
            ui->logtextBrowser->setTextColor(QColor(Qt::black));
            return ;
        }
    }
    if(cmdlist.size()>40)
    {
        ui->logtextBrowser->append("you may send too much.");
        ui->logtextBrowser->setTextColor(QColor(Qt::black));
        return ;
    }
    if(cmdlist.size()==0)
    {
        ui->logtextBrowser->append("Just type something,man~");
        ui->logtextBrowser->setTextColor(QColor(Qt::black));
        return ;
    }
    ui->logtextBrowser->setTextColor(QColor(Qt::black));

    quint8* ins = new quint8[cmdlist.size()];
    bool ok;
    ui->logtextBrowser->append(" ");
    ui->logtextBrowser->append("User defined Instrucitons:");
    QString _usrstr;
    for (int i = 0; i < cmdlist.size(); ++i)
    {
        ins[i] = cmdlist.at(i).toInt(&ok,16);
        _usrstr.append(cmdlist.at(i));
        _usrstr.append(" ");
    }
    //ui->logtextBrowser->append(_usrstr.toUpper());

    //send
    burnCmd_t c =
    {
        nullptr,
        nullptr,
        nullptr,
        ins,
        (quint8)cmdlist.size(),
        nullptr,
        9,
        nullptr,
        1,
        (quint32)((BurnSetting_T&)i2coptions->getsetting()).m_writedelay,
        (quint32)((BurnSetting_T&)i2coptions->getsetting()).m_writedelay,
    };

    m_transfer->setburnCmd(&c);
    m_transfer->run();
    m_transfer->wait();//wait for the end of transfer thread.
    delete[] ins;
}

void DDCMainWindow::cmdup()
{
    std::list<burnCmd_t*>::iterator it=m_atcmd.begin(),it1;
    advance(it,Cur_cmd);

    if(it==m_atcmd.begin()||Cur_cmd==0) return;

    burnCmd_t *tmpburncmd = (*it);
    it1 = it;
    tmpburncmd = *it;
    *it = *(--it1);
    *(it1) = tmpburncmd;

    ui->instructionsetlistWidget->item(Cur_cmd)->setText((*it)->name);
    ui->instructionsetlistWidget->item(--Cur_cmd)->setText((*it1)->name);
    ui->instructionsetlistWidget->setCurrentRow(Cur_cmd);
}

void DDCMainWindow::cmddown()
{
    std::list<burnCmd_t*>::iterator it=m_atcmd.begin(),it1;
    advance(it,Cur_cmd);

    if(it==m_atcmd.end()||Cur_cmd==ui->instructionsetlistWidget->count()-1) return;

    burnCmd_t *tmpburncmd = (*it);
    it1 = it;
    tmpburncmd = *it;
    *it = *(++it1);
    *(it1) = tmpburncmd;

    ui->instructionsetlistWidget->item(Cur_cmd)->setText((*it)->name);
    ui->instructionsetlistWidget->item(++Cur_cmd)->setText((*it1)->name);
    ui->instructionsetlistWidget->setCurrentRow(Cur_cmd);
}

void DDCMainWindow::cmdstep()
{
    if (NULL==i2cdevice.gethandle())
    {
        QMessageBox::warning(this, tr("Tips"), tr("pleas open device first!!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    ddcprotocol->setSlaveAddr(((BurnSetting_T&)i2coptions->getsetting()).m_slaveaddr);

    std::list<burnCmd_t*>::iterator it=m_atcmd.begin();
    advance(it,Cur_cmd);

    QDateTime time = QDateTime::currentDateTime();
    QString _str = time.toString("yy-MM-dd hh:mm:ss -->");
    _str.append((*it)->name);
    _str.append(":");
    ui->logtextBrowser->append(QObject::tr("<font color = black>%1</font>").arg("\n"));
    ui->logtextBrowser->append(_str);

    if((*it)->setparafunc != nullptr)
    {
        int ret = (*it)->setparafunc(paralineedit.text(),(*it)->burndata,(*it)->datalen);
        if (ret == -1)
        {
            ui->logtextBrowser->setTextColor(QColor(Qt::red));
            ui->logtextBrowser->append("argument format error.");
            ui->logtextBrowser->setTextColor(QColor(Qt::black));
            return;
        }
        else if (ret == -2)
        {
            ui->logtextBrowser->setTextColor(QColor(Qt::red));
            ui->logtextBrowser->append("too many arguments to this instrcution.");
            ui->logtextBrowser->setTextColor(QColor(Qt::black));
            return;
        }

        updateATcmds(**it,1);
    }

    //TODO:set dynamic parameter.
    (*it)->retrycnt = retryspbox.value();
    (*it)->delay = delayspbox.value();

    m_transfer->setburnCmd(*it);
    m_transfer->run();
    m_transfer->wait();
}

void DDCMainWindow::cmdrun()
{
    int size = ui->instructionsetlistWidget->count();
    while(Cur_cmd < size)
    {
        cmdstep();
        Cur_cmd++;
        ui->instructionsetlistWidget->setCurrentRow(Cur_cmd);
        //std::cout<<"Current row:"<<Cur_cmd<<std::endl;
    }
    if(Cur_cmd == size)
    {
        Cur_cmd = size-1;
    }
    //std::cout<<"Finialize row:"<<Cur_cmd<<std::endl;
}
