#ifndef I2C_SETTINGWINDOW_H
#define I2C_SETTINGWINDOW_H

#include <QWidget>

namespace Ui {
class i2c_settingwindow;
}

class i2c_settingwindow : public QWidget
{
    Q_OBJECT

public:
    explicit i2c_settingwindow(QWidget *parent = 0);
    ~i2c_settingwindow();

private:
    Ui::i2c_settingwindow *ui;
};

#endif // I2C_SETTINGWINDOW_H
