#ifndef COMMUNICATIONSETTING_H
#define COMMUNICATIONSETTING_H

#include <QWidget>

namespace Ui {
class CommunicationOption;
}

class CommunicationOption : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicationOption(QWidget *parent = 0);
    ~CommunicationOption();

private:
    Ui::CommunicationOption *ui;
};

#endif // COMMUNICATIONSETTING_H
