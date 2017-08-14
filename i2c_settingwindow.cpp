#include "i2c_settingwindow.h"
#include "ui_i2c_settingwindow.h"

i2c_settingwindow::i2c_settingwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::i2c_settingwindow)
{
    ui->setupUi(this);
}

i2c_settingwindow::~i2c_settingwindow()
{
    delete ui;
}
