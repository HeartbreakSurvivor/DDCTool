#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>
#include <QString>

namespace Ui {
class Helpwindow;
}

class Helpwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Helpwindow(QWidget *parent = 0);
    ~Helpwindow();

public slots:
    void openUrl(QString url);

private:
    Ui::Helpwindow *ui;
};

#endif // HELPWINDOW_H
