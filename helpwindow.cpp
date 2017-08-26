#include "helpwindow.h"
#include "ui_helpwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

Helpwindow::Helpwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Helpwindow)
{
    ui->setupUi(this);//setuo the ui first and then do other operations.
    connect(ui->githublink_label,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
}

void Helpwindow::openUrl(QString url)
{
   QDesktopServices::openUrl(QUrl(url));
}

Helpwindow::~Helpwindow()
{
    delete ui;
}
