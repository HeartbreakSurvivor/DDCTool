#ifndef COMMUNICATIONSETTING_H
#define COMMUNICATIONSETTING_H

#include <QWidget>
#include "burnsetting.h"

namespace Ui {
class CommunicationOption;
}

class CommunicationOption : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicationOption(QWidget *parent = 0);
    CommunicationOption(BurnSetting_T &burnsetting);
    ~CommunicationOption();
    const BurnSetting_T& getsetting();

public slots:
    void pressok();
    void presscancel();

private:
    BurnSetting_T m_burnsetting;
    Ui::CommunicationOption *ui;
};

#endif // COMMUNICATIONSETTING_H
