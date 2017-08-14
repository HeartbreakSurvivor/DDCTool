#ifndef COMMUNICATIONSETTING_H
#define COMMUNICATIONSETTING_H

#include <QWidget>

namespace Ui {
class CommunicationSetting;
}

class CommunicationSetting : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicationSetting(QWidget *parent = 0);
    ~CommunicationSetting();

private:
    Ui::CommunicationSetting *ui;
};

#endif // COMMUNICATIONSETTING_H
