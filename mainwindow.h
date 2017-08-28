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
#include <QUrl>

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateEdidTab(QString key);
    void clearEdidTab();
    void updateHdcpTab();

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

    //Edid
    QString Cur_Key;
    std::vector<QString> edid_type;
    std::map<QString,Edid_T*> edid_map;

    //Hdcp
    Hdcp_T *hdcpdata=nullptr;
    Transfer_T *hdcptransfer=nullptr;


    //the automatically generated ui file
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
