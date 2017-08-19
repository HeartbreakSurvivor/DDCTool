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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    std::map<QString,Cvt_EDID*> edid_map;

    //the automatically generated ui file
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
