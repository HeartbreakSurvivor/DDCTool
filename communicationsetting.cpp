#include "communicationsetting.h"
#include "ui_communicationsetting.h"

CommunicationSetting::CommunicationSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationSetting)
{
    ui->setupUi(this);
}

CommunicationSetting::~CommunicationSetting()
{
    delete ui;
}
