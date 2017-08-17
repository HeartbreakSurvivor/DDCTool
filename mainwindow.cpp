#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

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


    //initialize signals and slots
    //EDID Tab
    connect(ui->loadedid_Btn, SIGNAL(clicked()), this, SLOT(loadEdid()));//load the edid to ram.
    connect(ui->nextedid_Btn, SIGNAL(clicked()), this, SLOT(nextEdid()));//dispaly the next edid which load to ram before.
    connect(ui->edidSync_Btn, SIGNAL(clicked()), this, SLOT(syncEdid()));//when change the edid data and sync to ram
    connect(ui->edidSave_Btn, SIGNAL(clicked()), this, SLOT(saveEdid()));//save the edid as a new edid data
    connect(ui->edidWrite_Btn, SIGNAL(clicked()), this, SLOT(writeEdid()));//write the edid to board
    connect(ui->edidStop_Btn, SIGNAL(clicked()), this, SLOT(stopEdid()));//stop the write operation

    ui->statusBar->addWidget(DDC_BurnStatusText);
    ui->statusBar->addWidget(DDC_ProgressBar);
    ui->statusBar->addWidget(DDC_BurnStatus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//slots
void MainWindow::qTimeSlot(void)
{
    QDateTime DDC_CurTime = QDateTime::currentDateTime();
    QString str = DDC_CurTime.toString("yyyy-MM-dd hh:mm:ss");

    DDC_TimeLabel->setText(str);
}

//EDID Tab Slots
void MainWindow::loadEdid(void)
{
    cout<<"load edid"<<endl;
}

void MainWindow::nextEdid(void)
{
    cout<<"next edid bin file"<<endl;
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

void MainWindow::stopEdid(void)
{
    cout<<"stop the writing of edid"<<endl;
}


