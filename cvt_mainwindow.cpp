#include "global.h"

#include <iostream>

using namespace std;

Cvt_mainwindow::Cvt_mainwindow(QWidget *parent):QMainWindow(parent)
{
    setWindowTitle(tr("CVT DDC Tool V1.0"));

	QMenuBar *Menubar = new QMenuBar(this);
	QMenu *cOptions = Menubar->addMenu(tr("&Options"));
    QdebugMode = new QAction(tr("&Debug Mode"),this);
    QdebugMode->setCheckable(true);
    cOptions->addAction(QdebugMode);

	QMenu *chelp = Menubar->addMenu(tr("&Help"));
	chelp->addAction(tr("&Help..."));
	chelp->addSeparator();
	chelp->addAction(tr("&About"));
	this->setMenuBar(Menubar);

    Logo_Label = new QLabel("LOGO",this);
    QImage *logo_img = new QImage(":/new/prefix1/Cvte.png");
    QImage *scaled_logo_img = new QImage();
    *scaled_logo_img=logo_img->scaled(200,60,Qt::IgnoreAspectRatio);
    Logo_Label->setPixmap(QPixmap::fromImage(*scaled_logo_img));

    BurnTab *EdidHdcpkeyTab = new BurnTab(this);
    DebugTab *debugTab = new DebugTab(this);

    DDC_Tabs = new QTabWidget(this);

    DDC_Tabs->addTab(EdidHdcpkeyTab,tr("BurnTab"));
    DDC_Tabs->addTab(debugTab,tr("DebugTab"));

    QWidget *widget = new QWidget(this);

    //setup a layout and add widgets to it
    QGridLayout *Gridlayout = new QGridLayout(this);
    Gridlayout->addWidget(Logo_Label,0,0,1,1, Qt::AlignHCenter);
    Gridlayout->addWidget(IspSettingsGroup(),0,1,1,1,Qt::AlignHCenter);
    Gridlayout->addWidget(DDC_Tabs,1,0,1,2);

    this->setCentralWidget(widget);
    widget->setLayout(Gridlayout);


    //xxx->show();

/*

    QHBoxLayout *grid = new QHBoxLayout;
    //grid->addWidget(IspSettingsGroup(), 0, 0, 1, 1);
    //grid->addWidget(Logo_Label, 0, 1, 1, 2);

    grid->addWidget(IspSettingsGroup());
    grid->addWidget(Logo_Label);
    //grid->addWidget(xxx, 1, 0, 1, 3);

    widget->setLayout(grid);


    setFixedSize(800,700);
    */


    //DDC_Tabs = new QTabWidget();
    //DDC_Tabs->addTab(new BurnTab,tr("BurnTab"));

    //BurnTab *xxx = new BurnTab();


    //grid->addWidget(EDIDGroup(), 1, 0, 1, 1);
    //grid->addWidget(HdcpKeyGroup(), 2, 0, 1, 1);
    //grid->addWidget(PROMPTGroup(), 1, 2, 2, 1);
   // grid->addWidget(DDCMsgGroup(), 4, 0, 1, 3);
    //grid->addWidget(DDC_Tabs, 5, 0, 1, 3);


    //readSettings();
    //cvt_edid = new Cvt_EDID[EdidPorts_Num]();//create the edid and append to cvt_edid dynamic class array


    //Hboxlayout->addWidget(IspSettingsGroup());
   // Hboxlayout->addWidget(Logo_Label);
/*

    QHBoxLayout *grid = new QHBoxLayout;
    //grid->addWidget(IspSettingsGroup(), 0, 0, 1, 1);
    //grid->addWidget(Logo_Label, 0, 1, 1, 2);

    grid->addWidget(IspSettingsGroup());
    grid->addWidget(Logo_Label);
    //grid->addWidget(xxx, 1, 0, 1, 3);

    widget->setLayout(grid);


    setFixedSize(800,700);
    */
}

Cvt_mainwindow::~Cvt_mainwindow()
{
}

QGroupBox *Cvt_mainwindow::IspSettingsGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("IIC Settings"));

    QLabel *I2cSpeedText = new QLabel("IIC Speed:", this);

    Burn_IICSpeedSpin = new QSpinBox(this);
    Burn_IICSpeedSpin->setSingleStep(1);
    Burn_IICSpeedSpin->setRange(1,400);
    Burn_IICSpeedSpin->setSuffix("khz");

    Burn_Connectbtn = new QPushButton(tr("&Connect"), this);
    Burn_DisConnectbtn = new QPushButton(tr("&DisConnect"), this);
    Burn_Connectbtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    Burn_DisConnectbtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(Burn_Connectbtn, SIGNAL(clicked()), this, SLOT(DDC_ConnectDevice()));
    connect(Burn_DisConnectbtn, SIGNAL(clicked()), this, SLOT(DDC_DisConnectDevice()));

    QHBoxLayout *Isp_IIC_Layout = new QHBoxLayout(this);
    Isp_IIC_Layout->addWidget(I2cSpeedText);
    Isp_IIC_Layout->addWidget(Burn_IICSpeedSpin);
    Isp_IIC_Layout->addWidget(Burn_Connectbtn);
    Isp_IIC_Layout->addWidget(Burn_DisConnectbtn);

    groupBox->setLayout(Isp_IIC_Layout);
    return groupBox;
}

void Cvt_mainwindow::closeEvent(QCloseEvent *event)
{
    //writeSettings();
	qDebug() << QString::fromLocal8Bit("close windows");
}




