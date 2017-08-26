#include "communicationsetting.h"
#include "ui_communicationsetting.h"
#include "qdebug.h"

CommunicationOption::CommunicationOption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationOption)
{
    qDebug()<<"set up the i2c options menu";
    ui->setupUi(this);
}

CommunicationOption::~CommunicationOption()
{
    delete ui;
}
