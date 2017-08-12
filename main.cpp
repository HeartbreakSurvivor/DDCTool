#include "cvt_mainwindow.h"
#include <QtWidgets/QApplication>
#include "formtest2.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("CVTE");
	QCoreApplication::setOrganizationDomain("zhanghao3126@cvte.com");
	QCoreApplication::setOrganizationName("Cvt DDC tool");

    //QApplication::addLibraryPath("./Images");
    a.setWindowIcon(QIcon(":/new/prefix1/ddc.ico"));

//	Cvt_mainwindow w;
//	w.show();
    FormTest2 form;
    form.show();
	return a.exec();
}
