#include "cvt_debugtab.h"

DebugTab::DebugTab(QWidget *parent):QWidget(parent)
{
    QLabel *slave = new QLabel(tr("SlaveAddr"));
    QLabel *len = new QLabel(tr("Length"));

    SlaveAddr = new QLineEdit(this);
    CmdLength = new QLineEdit(this);

    DDC_DebugLog = new QTextEdit(this);
    DDC_Msg2Send = new QLineEdit(this);

    QPushButton *DDC_Sendbtn = new QPushButton(tr("&Send"));
    //connect(DDC_Sendbtn, SIGNAL(clicked()), this, SLOT(DDC_DebugMsgSend()));
    QPushButton *DDC_OpenLogs = new QPushButton(tr("&Open Logs"));
    //connect(DDC_OpenLogs, SIGNAL(clicked()), this, SLOT(DDC_OpenLogs()));
    QPushButton *DDC_Clearbtn = new QPushButton(tr("&Clear"));
    //connect(DDC_Clearbtn, SIGNAL(clicked()), this, SLOT(DDC_MsgClear()));

    QHBoxLayout *MsgSendLayout = new QHBoxLayout(this);
    MsgSendLayout->addWidget(DDC_Msg2Send);
    MsgSendLayout->addWidget(DDC_Sendbtn);
    MsgSendLayout->addWidget(DDC_Clearbtn);

    QVBoxLayout *Debugloglayout = new QVBoxLayout(this);
    //Debugloglayout->addWidget(DDC_DebugLog);
    Debugloglayout->addLayout(MsgSendLayout);

    QHBoxLayout *Slavelayout = new QHBoxLayout(this);
    Slavelayout->addWidget(slave);
    Slavelayout->addWidget(SlaveAddr);
    Slavelayout->addWidget(len);
    Slavelayout->addWidget(CmdLength);

    QGridLayout *Grid = new QGridLayout(this);

    Grid->addLayout(Slavelayout,0,0,1,1);
    Grid->addLayout(Debugloglayout,1,0,1,1);
    setLayout(Grid);

    qDebug() << "debugTab";

}
