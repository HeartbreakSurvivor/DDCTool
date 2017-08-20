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
    connect(ui->edidStop_Btn, SIGNAL(clicked()), this, SLOT(stopEdid()));//stop the edid write operation

    //HDCP Tab





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
    QStringList  fileNameList;
    QString fileName;
    QFileDialog* fd = new QFileDialog(this, tr("Select the EDID"));
    fd->setFileMode(QFileDialog::Directory);//chose the directory
    fd->setDirectory(".");
    fd->setViewMode(QFileDialog::List);
    if (fd->exec() == QDialog::Accepted)
    {
        fileNameList = fd->selectedFiles();
        fileName = fileNameList[0];
        if (fileName != NULL)
        {
            QDir dir(fileName);
            QStringList filters;
            filters << QString("vga.bin") << QString("dvi.bin") << QString("hdmi.bin") <<QString("hdmi1.bin") << QString("hdmi2.bin")<< QString("dp.bin");
            dir.setNameFilters(filters);
            dir.setFilter(QDir::Files | QDir::NoSymLinks);
            QFileInfoList list = dir.entryInfoList();

            QChar separator = QChar('/');//get the separtor '/'
            if (!fileName.contains(separator))
            {
                separator = QChar('\\');
            }
            QChar last_char = fileName.at(fileName.length() - 1);
            if (last_char == separator)
            {
                separator = QChar();
            }

            QString key;
            for (int i = 0; i < list.size(); ++i)
            {
                QFileInfo fileInfo = list.at(i);
                QString file_name = dir[i];
                QString file_path = fileName + separator + file_name;//the file full path
                key = file_name.left(file_name.indexOf(QString(".bin")));//get the file's pure name

                //create the edid map
                //edid_map[key] = new Cvt_EDID(file_path);

                qDebug() <<"key:"<< key;
                qDebug() << qPrintable(QString("%1 %2").arg(fileInfo.size(), 10).arg(file_path));
                //filename2datatypeAndIdx(file_name, tmp_type_Idx,edidports);
                //cvt_edid[tmp_type_Idx] = Cvt_EDID(file_path, tmp_type_Idx);//create the edid and append to cvt_edid dynamic class array
                //qDebug("filename Idx:%d,DATAEXISTED_IDX:%d", i, tmp_type_Idx);
            }

            //qDebug("EdidPorts num:%x", edidports);
            /*
            qDebug("Edidlist size:%d",cvt_edid[tmp_type_Idx].getLength());
            for (int j = 0; j < 2; ++j)
            {
                unsigned char buf[128];
                cvt_edid[tmp_type_Idx].getdata(j, 128, buf, 128);
                for (int k = 0; k < 128; ++k)
                {
                    qDebug("copy edid data:%x",buf[k]);
                }
            }

            QString tr;
            tr = QString("%1").arg(cvt_edid[tmp_type_Idx].getProductCode(), 4, 16, QLatin1Char('0'));
            EdidManufacturerName->setText(cvt_edid[tmp_type_Idx].getManufacturerName());
            EdidProductCode->setText(tr);
            EdidYear->setText(QString::number(cvt_edid[tmp_type_Idx].getProductYear(), 10));
            EdidWeek->setText(QString::number(cvt_edid[tmp_type_Idx].getProductWeek(), 10));*/

            //redraw operations
            ui->edidpathLineEdit->setText(fileName);
            ui->edidnameLineEdit->setText(key);
        }
    }
    else
        fd->close();
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


