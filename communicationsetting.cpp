#include "communicationsetting.h"
#include "ui_communicationsetting.h"
#include "qdebug.h"
#include <QMessageBox>

CommunicationOption::CommunicationOption(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicationOption)
{
    qDebug()<<"set up the i2c options menu";
    ui->setupUi(this);
    //set the window to modal
    this->setWindowFlags(Qt::Dialog);
    this->setWindowModality(Qt::ApplicationModal);
}

CommunicationOption::~CommunicationOption()
{
    qDebug()<<"delete the ui pointer";
    delete ui;
}

CommunicationOption::CommunicationOption(BurnSetting_T &burnsetting)
{
    qDebug()<<"show the communication options menu";
    ui = new Ui::CommunicationOption();
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);//set the window to modal
    this->setWindowModality(Qt::ApplicationModal);

    m_burnsetting = burnsetting;

    ui->readdelayspinBox->setValue(m_burnsetting.getreadDelay());
    ui->i2cspeedspinBox->setValue(m_burnsetting.getI2cSpeed());
    ui->writedelayspinBox->setValue(m_burnsetting.getwriteDelay());

    QString slaveaddr = QString("0x")+QString("%1").arg(m_burnsetting.getSlaveaddr()&0xFF,2,16,QLatin1Char('0')).toUpper();
    ui->i2cslaveaddrlineEdit->setMaxLength(4);
    ui->i2cslaveaddrlineEdit->setText(slaveaddr);

    //ui->createlogcheckBox->setChecked(m_burnsetting.getCreatlogs()?true:false);
    ui->edidlpdelayspinBox->setValue(m_burnsetting.getEdidlastDelay());
    ui->erasedelayspinBox->setValue(m_burnsetting.getEraseHdcpDelay());
    ui->spretryspinBox->setValue(m_burnsetting.getPerpackRetryCnt());
    ui->hdcplpdelayspinBox->setValue(m_burnsetting.getHdcplastDelay());
    ui->retryspinBox->setValue(m_burnsetting.getRetryCnt());

    //slots
    connect(ui->i2cok_Btn,SIGNAL(clicked()),this,SLOT(pressok()));
    connect(ui->i2ccancel_Btn,SIGNAL(clicked()),this,SLOT(presscancel()));
}

bool ishex(QChar character)
{
    if(character.isDigit()) return true;
    if((character>='a'&&character<='F')||character>='A'&&character<='F') return true;
    return false;
}

void CommunicationOption::pressok(void)
{
    QString slaveaddr = ui->i2cslaveaddrlineEdit->text();
    qDebug()<<"slaveaddr:"<<slaveaddr;

    if((slaveaddr.left(2)!=QString("0x"))||(!ishex(slaveaddr.at(2))||!ishex(slaveaddr.at(3))))
    {
        QMessageBox::warning(this, tr("Tips"), tr("Please input valid slave addr,such as 0x6E"));
        return;
    }
    bool ok;
    qDebug()<<"slaveaddrasda:"<<(unsigned char)slaveaddr.right(2).toInt(&ok,16);
    m_burnsetting.setSlaveaddr((unsigned char)slaveaddr.right(2).toInt(&ok,16));
    m_burnsetting.setI2cSpeed(ui->i2cspeedspinBox->value());
    m_burnsetting.setwriteDelay(ui->writedelayspinBox->value());
    m_burnsetting.setreadDelay(ui->readdelayspinBox->value());
    //m_burnsetting.setCreatlogs((bool)ui->createlogcheckBox->isChecked()?1:0);

    m_burnsetting.setEraseHdcpDelay(ui->erasedelayspinBox->value());
    m_burnsetting.setEdidlastDelay(ui->edidlpdelayspinBox->value());
    m_burnsetting.setHdcplastDelay(ui->hdcplpdelayspinBox->value());
    m_burnsetting.setRetryCnt(ui->retryspinBox->value());
    m_burnsetting.setPerpackRetryCnt(ui->spretryspinBox->value());

    this->close();
}

void CommunicationOption::presscancel(void)
{
    ui->readdelayspinBox->setValue(m_burnsetting.getreadDelay());
    ui->i2cspeedspinBox->setValue(m_burnsetting.getI2cSpeed());
    ui->writedelayspinBox->setValue(m_burnsetting.getwriteDelay());

    QString slaveaddr = QString("0x")+QString("%1").arg(m_burnsetting.getSlaveaddr()&0xFF,2,16,QLatin1Char('0')).toUpper();
    ui->i2cslaveaddrlineEdit->setText(slaveaddr);

    //ui->createlogcheckBox->setChecked(m_burnsetting.getCreatlogs()?true:false);
    ui->edidlpdelayspinBox->setValue(m_burnsetting.getEdidlastDelay());
    ui->erasedelayspinBox->setValue(m_burnsetting.getEraseHdcpDelay());
    ui->spretryspinBox->setValue(m_burnsetting.getPerpackRetryCnt());
    ui->hdcplpdelayspinBox->setValue(m_burnsetting.getHdcplastDelay());
    ui->retryspinBox->setValue(m_burnsetting.getRetryCnt());
    this->close();
}

const BurnSetting_T& CommunicationOption::getsetting(void)
{
    return m_burnsetting;
}



