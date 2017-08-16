#include "instructionwindow.h"
#include "ui_instructionwindow.h"

InstructionWindow::InstructionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InstructionWindow)
{
    ui->setupUi(this);
}

InstructionWindow::~InstructionWindow()
{
    delete ui;
}
