#include "helpwindow.h"
#include "ui_helpwindow.h"

Helpwindow::Helpwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Helpwindow)
{
    ui->setupUi(this);
}

Helpwindow::~Helpwindow()
{
    delete ui;
}
