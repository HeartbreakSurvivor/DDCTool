#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/Qlabel>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QSpinBox>
#include <QUrl>
#include <QLineEdit>
#include <QModelIndex>
#include <QRegExp>
#include <QMutex>
#include <qlogging.h>

#include "cvt_global.h"
#include "edid_data.h"
#include "hdcp_data.h"
#include "isp_iic.h"
#include "burnsetting.h"
#include "communicationsetting.h"
#include "helpwindow.h"
#include "ddc_protocol.h"
#include "transfer.h"

namespace Ui {
class MainWindow;
}

using namespace ddc;

class DDCMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DDCMainWindow(QWidget *parent = 0);
    ~DDCMainWindow();

    void ui_preinit(void);

    void clearEdidTab();
    void updateEdidTab(QString key);
    void updateHdcpTab();

    void updateATcmds(const burnCmd_t& cmd,int options);//the value that reference points to can't be modified.
    void readSettings();
    void writeSettings();

    void getedidtypes();

public slots:
    void closeEvent(QCloseEvent *event);
    void qTimeSlot(void);

private slots:
    //Common Options
    void displayi2coptions(void);
    void displayhelpmenu(void);
    void displayaboutmenu(void);

    //ISP
    void connectI2c();
    void disconnetI2c();
    void opendebugmsg();

    //EDID Tab
    void loadEdid();
    void nextEdid();
    void syncEdid();
    void saveEdid();
    void writeEdid();
    void stopWriteEdid();

    //HDCP Tab
    void loadHdcp();
    void writeHdcp();
    void stopWriteHdcp();
    void changechiptype();

    //Debug Tab
    void updatelogText(QString msg);
    void itemClicked(QModelIndex idx);
    void cmdclear();
    void cmdsend();
    void cmdup();
    void cmddown();
    void cmdstep();
    void cmdrun();

private:
    QProgressBar *DDC_ProgressBar;//indicate the burn progress.
    QLabel *DDC_BurnStatus;//indicate the result of burning.
    QDateTime DDC_CurTime;//indicate the current system time.
    QLabel *DDC_TimeLabel;
    QTimer *timer;

    //I2C
    Helpwindow helpwindow;
    Isp_I2C i2cdevice;
    BurnSetting_T* burnsettings;
    CommunicationOption *i2coptions;
    DDCProtocol_T *ddcprotocol;

    //Transfer layer
    QMutex m_mtx;
    Transfer_T *m_transfer=nullptr;

    //Edid
    QString Cur_Key;
    std::vector<QString> edid_type;
    std::map<QString,Edid_T*> edid_map;

    //Hdcp
    Hdcp_T *hdcpdata=nullptr;

    //DebugTab
    int Cur_cmd=0;
    std::list<burnCmd_t*> m_atcmd;
    QLineEdit paralineedit;
    QSpinBox retryspbox;
    QSpinBox delayspbox;
    //optional ui style
    QLineEdit paralineedid;
    QSpinBox paraspinbox;

    //the automatically generated ui file
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
