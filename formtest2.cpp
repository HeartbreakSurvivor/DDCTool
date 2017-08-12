#include "formtest2.h"
#include "ui_formtest2.h"

FormTest2::FormTest2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTest2)
{
    ui->setupUi(this);


}

FormTest2::~FormTest2()
{
    delete ui;
}
