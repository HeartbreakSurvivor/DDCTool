#ifndef CVT_DEBUGTAB_H
#define CVT_DEBUGTAB_H

#include "global.h"

class DebugTab:public QWidget
{
    Q_OBJECT

public:
    explicit DebugTab(QWidget *parent=0);

private:

    QLineEdit *SlaveAddr;
    QLineEdit *CmdLength;

    QPushButton *CreatBtn;

    QTextEdit *DDC_DebugLog;
    QLineEdit *DDC_Msg2Send;

};


#endif // CVT_DEBUGTAB_H
