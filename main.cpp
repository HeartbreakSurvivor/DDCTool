#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/new/prefix1/ddc.ico"));

	QCoreApplication::setOrganizationName("CVTE");
	QCoreApplication::setOrganizationDomain("zhanghao3126@cvte.com");
    QCoreApplication::setOrganizationName("DDC Tool");

    MainWindow window;//initialize the mainwindow but not show.
    window.show();//show the mainwindow and start the event handler.
	return a.exec();
}
