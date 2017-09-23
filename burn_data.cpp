#include "burn_data.h"

namespace ddc {

BurnData_T::BurnData_T()
{
    //qDebug()<<"BurnData_T constructor !";
}

BurnData_T::BurnData_T(const QString &filepath)
{
    //qDebug()<<"BurnData_T parameter constructor !";
    file = new QFile(filepath);
    if (!file->open(QIODevice::ReadWrite)) return;

    size = file->size();
    filename = filepath.left(filepath.indexOf(QString(".bin")));//get the file's pure name
    data = new quint8[size];
    if (file->read((char*)data, size) == -1) return;
}

BurnData_T::~BurnData_T()
{
    //qDebug()<<"BurnData_T close file!";
    delete[] data;
    file->close();
}

int BurnData_T::getLength(void) const
{
    return size;
}

QString BurnData_T::getfilename(void) const
{
    return filename;
}

void BurnData_T::syncfile()
{
    //qDebug()<<"sync file";
    file->seek(0);

    QDataStream out(file);
    for(int i=0;i<size;i++)
    {
        out<<data[i];
    }
    file->flush();
}

}
