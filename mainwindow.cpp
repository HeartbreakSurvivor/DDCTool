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
    DDC_BurnStatus->setStyleSheet("color:green");//CSS

    DDC_TimeLabel = new QLabel(tr(""));
    ui->statusBar->addPermanentWidget(DDC_TimeLabel);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(qTimeSlot()));
    timer->start(1000);

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
    QDateTime DDC_CurTime = QDateTime::currentDateTime();//获取系统现在的时间
    QString str = DDC_CurTime.toString("yyyy-MM-dd hh:mm:ss"); //设置显示格式

    DDC_TimeLabel->setText(str);
}
