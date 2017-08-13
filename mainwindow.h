#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/Qlabel>
#include <QTimer>

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

private:
    QProgressBar *DDC_ProgressBar;//indicate the burn progress.
    QLabel *DDC_BurnStatus;//indicate the result of burning.
    QDateTime DDC_CurTime;//indicate the current system time.
    QLabel *DDC_TimeLabel;
    QTimer *timer;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
