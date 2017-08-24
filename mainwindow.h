#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/Qlabel>
#include <QTimer>
#include <QFileDialog>
#include <QDebug>

#include "cvt_global.h"
#include "edid_data.h"

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

public slots:
    void qTimeSlot(void);

private slots:
    //EDID Tab
    void loadEdid();
    void nextEdid();
    void syncEdid();
    void saveEdid();
    void writeEdid();
    void stopEdid();


private:
    QProgressBar *DDC_ProgressBar;//indicate the burn progress.
    QLabel *DDC_BurnStatus;//indicate the result of burning.
    QDateTime DDC_CurTime;//indicate the current system time.
    QLabel *DDC_TimeLabel;
    QTimer *timer;

    //ADT
    QString Cur_Key;
    std::map<QString,Edid_T*> edid_map;

    //the automatically generated ui file
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
