#pragma once

#include "global.h"



class Cvt_mainwindow : public QMainWindow
{
	Q_OBJECT
public:
	Cvt_mainwindow(QWidget *parent = Q_NULLPTR);
	~Cvt_mainwindow();

public slots:
    void closeEvent(QCloseEvent *event);

private:
    QAction *QdebugMode;
    QLabel *Logo_Label;

    QPushButton* Burn_Connectbtn;
    QPushButton* Burn_DisConnectbtn;
    QSpinBox *Burn_IICSpeedSpin;

    QTabWidget *DDC_Tabs;//multiple tabs

    QGroupBox *IspSettingsGroup();

};
