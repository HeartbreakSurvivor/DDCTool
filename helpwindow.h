#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

namespace Ui {
class Helpwindow;
}

class Helpwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Helpwindow(QWidget *parent = 0);
    ~Helpwindow();

private:
    Ui::Helpwindow *ui;
};

#endif // HELPWINDOW_H
