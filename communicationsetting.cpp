#include "communicationsetting.h"
#include "ui_communicationsetting.h"

CommunicationOption::CommunicationOption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationOption)
{
    ui->setupUi(this);
}

CommunicationOption::~CommunicationOption()
{
    delete ui;
}
