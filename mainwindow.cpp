#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace ddc;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

    readSettings();

    qDebug()<<"init the options";
    i2coptions = new CommunicationOption(*burnsettings);

    ddcprotocol = new DDCProtocol_T(i2cdevice);
    ddcprotocol->setSlaveAddr(((BurnSetting_T&)i2coptions->getsetting()).getSlaveaddr());

    hdcptransfer = new Transfer_T(*ddcprotocol,&hdcpmsg,3,3,200);
    //hdcptransfer = new Transfer_T(*ddcprotocol);

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
    connect(ui->edidSave_Btn, SIGNAL(clicked()), this, SLOT(saveEdid()));//save the edid as a new edid data
    connect(ui->edidWrite_Btn, SIGNAL(clicked()), this, SLOT(writeEdid()));//write the edid to board
    connect(ui->edidStop_Btn, SIGNAL(clicked()), this, SLOT(stopWriteEdid()));//stop the edid writing operation

    //HDCP Tab
    connect(ui->loadhdcp_Btn, SIGNAL(clicked()), this, SLOT(loadHdcp()));//load the hdcp to ram.
    connect(ui->writehdcp_Btn, SIGNAL(clicked()), this, SLOT(writeHdcp()));//write the hdcp to board
    connect(ui->stopwritehdcp_Btn, SIGNAL(clicked()), this, SLOT(stopWriteHdcp()));//stop the hdcp writing operation

    ui->statusBar->addWidget(DDC_BurnStatusText);
    ui->statusBar->addWidget(DDC_ProgressBar);
    ui->statusBar->addWidget(DDC_BurnStatus);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete i2coptions;
}

void writeSettings();

void MainWindow::readSettings()
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

    qDebug()<<"read settings";

    burnsettings = new BurnSetting_T(tmp_slaveaddr,tmp_I2cSpeed,tmp_WriteDelay,tmp_ReadDelay,tmp_RetryCnt,tmp_PerPackRetryCnt,
    tmp_EdidlastDelay,tmp_HdcplastDelay,tmp_EraseHdcp,(bool)tmp_IsCreatLogs);

    Burn_Settings.endGroup();
}


void MainWindow::writeSettings()
{
    QSettings Burn_Settings("Cvte", "DDC Tool");

    //const修饰返回值的函数，必须定义const类型的变量去赋值，或者强制转化成非const类型
    //const BurnSetting_T &burnsetting = i2coptions->getsetting();

    BurnSetting_T &i2csettings = (BurnSetting_T&)i2coptions->getsetting();
    qDebug("the i2c speed:%d",i2csettings.getI2cSpeed());

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

void MainWindow::updateEdidTab(QString key)
{
    int row=-1,column=0;

    ui->edidnameLineEdit->clear();
    ui->edidsizeLineEdit->clear();
    ui->manufacturerNameLineEdit->clear();
    ui->manufacturerCodeLineEdit->clear();
    ui->manufacturerSNLineEdit->clear();
    ui->manufacturerWeekLineEdit->clear();
    ui->manufacturerYearLineEdit->clear();
    ui->edidVersionLineEdit->clear();
    ui->customerSNlineEdit->clear();

    ui->edidnameLineEdit->setText(key.toUpper());
    ui->edidsizeLineEdit->setText(QString::number(edid_map[key]->getLength()));

    ui->manufacturerNameLineEdit->setText(edid_map[key]->getManufacturerName());
    ui->manufacturerCodeLineEdit->setText(QString("%1").arg(edid_map[key]->getProductCode(), 4, 16, QLatin1Char('0')));
    ui->manufacturerSNLineEdit->setText(QString("%1").arg(edid_map[key]->getManufacturerSN(), 8, 16, QLatin1Char('0')));
    ui->manufacturerWeekLineEdit->setText(QString::number(edid_map[key]->getProductWeek()));
    ui->manufacturerYearLineEdit->setText(QString::number(edid_map[key]->getProductYear()));
    ui->edidVersionLineEdit->setText(edid_map[key]->getVersion());
    ui->customerSNlineEdit->setText(edid_map[key]->getProductSN());

    ui->EdidtableWidget->clear();
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
        //qDebug()<<"row:"<<row<<"column:"<<column;
    }
    if (edid_map[key]->getLength() == 128)
    {
        qDebug()<<"uncheck";
        //ui->EdidtableWidget->item(8,0)->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::updateHdcpTab()
{
    int row=-1,column=0;
    ui->writekeyidcheckBox->setChecked(false);
    ui->appendCrccheckBox->setChecked(false);
    ui->hdcpsizelineEdit->setText(QString::number(hdcpdata->getLength(),10));
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

//slots
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug("close the window!");
    writeSettings();
}

void MainWindow::qTimeSlot(void)
{
    QDateTime DDC_CurTime = QDateTime::currentDateTime();
    QString str = DDC_CurTime.toString("yyyy-MM-dd hh:mm:ss");

    DDC_TimeLabel->setText(str);
}

//I2C Options
void MainWindow::displayaboutmenu(void)
{
    qDebug("show the help window!");
    helpwindow.show();
}

void MainWindow::displayhelpmenu(void)
{

}

void MainWindow::displayi2coptions(void)
{
    i2coptions->show();
}

//Isp Slots
void MainWindow::connectI2c(void)
{
    //const修饰返回值的函数，必须定义const类型的变量去赋值，或者强制转化成非const类型
    //const BurnSetting_T &burnsetting = i2coptions->getsetting();
    BurnSetting_T &burnsetting = (BurnSetting_T&)i2coptions->getsetting();
    qDebug("the i2c speed:%d",burnsetting.getI2cSpeed());
    if (i2cdevice.openDevice(i2cdevice.gethandle(),burnsetting.getI2cSpeed()) == FTC_SUCCESS)
    {
        ui->actionConnect->setDisabled(true);
        qDebug("Open device successfully!!!");
    }
    else
    {
        QMessageBox::warning(this, tr("Tips"), tr("Open Device Failed."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
}

void MainWindow::disconnetI2c(void)
{
    if (NULL==i2cdevice.gethandle())
    {
        QMessageBox::warning(this, tr("Tips"), tr("pleas open device first!!"), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    ui->actionConnect->setDisabled(false);
    i2cdevice.closeDevice(i2cdevice.gethandle());
    qDebug("Close device successfully!!!");
}

void MainWindow::opendebugmsg(void)
{
    QString logpath = QDir::currentPath();
    QDesktopServices::openUrl(QUrl(logpath,QUrl::TolerantMode));
}

//EDID Tab Slots
void MainWindow::loadEdid(void)
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
                QFileInfo fileInfo = list.at(i);
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

void MainWindow::nextEdid(void)
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

void MainWindow::syncEdid(void)
{
    cout<<"sync edid"<<endl;
}

void MainWindow::saveEdid(void)
{
    cout<<"save edid bin file"<<endl;
}
void MainWindow::writeEdid(void)
{
    cout<<"write Edid"<<endl;
}

void MainWindow::stopWriteEdid(void)
{
    cout<<"stop the writing of edid"<<endl;
}

//Hdcp Tab Slots
void MainWindow::loadHdcp()
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

void MainWindow::writeHdcp()
{

}

void MainWindow::stopWriteHdcp()
{

}

