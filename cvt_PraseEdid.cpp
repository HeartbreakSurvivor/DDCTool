#include "cvt_PraseEdid.h"

Cvt_EDID::Cvt_EDID(const QString &filepath)
{
	QFile *Edidfile = new QFile(filepath);
	if (!Edidfile->open(QIODevice::ReadOnly)) return;
	datalen = Edidfile->size();
	data = new unsigned char[datalen];
	readdata = new unsigned char[datalen];

	if (Edidfile->read((char *)data, datalen) == -1) return;

	for (int i = 0; i < datalen; i++)
		qDebug("Edid%d: %x", i, data[i]);

	Edidfile->close();
}

/*
Cvt_EDID::Cvt_EDID(const QString &filepath, Cvt_DataType _datatype)
{
	QFile *Edidfile = new QFile(filepath);
	if (!Edidfile->open(QIODevice::ReadOnly)) return;
	datalen = Edidfile->size();
	datatype = _datatype;
	data = new unsigned char[datalen];
	readdata = new unsigned char[datalen];

	if (Edidfile->read((char *)data, datalen) == -1) return;
	Edidfile->close();
}
*/
Cvt_EDID::Cvt_EDID()
{
}

Cvt_EDID::Cvt_EDID(int len)
{
	datalen = len;
	data = new unsigned char[datalen]; 
	readdata = new unsigned char[datalen];
}

//深拷贝和浅拷贝
Cvt_EDID::Cvt_EDID(Cvt_EDID& _edid)
{
	datalen = _edid.getLength();/*常规成员变量*/


	//The king point
	data = new unsigned char[datalen];
	memcpy(data, _edid.data, datalen*sizeof(unsigned char));

	readdata = new unsigned char[datalen];
	memcpy(readdata, _edid.readdata, datalen * sizeof(unsigned char));

	qDebug("Copy the memory1 ");
}

Cvt_EDID& Cvt_EDID::operator=(Cvt_EDID& _edid)
{
	if (this == &_edid)  /*自赋值*/
		return *this;
	datalen = _edid.getLength();
    datatype = _edid.datatype;
	//The king point
	data = new unsigned char[datalen];
	memcpy(data, _edid.data, datalen * sizeof(unsigned char));

	readdata = new unsigned char[datalen];
	memcpy(readdata, _edid.readdata, datalen * sizeof(unsigned char));

	qDebug("Copy the memory2");
	return *this;
}

Cvt_EDID::~Cvt_EDID()
{
	if(data!=NULL)
		delete[] data;

	if (readdata != NULL)
		delete[] readdata;
	//delete EdidMname;
	qDebug("delete the memory ");
}

QString Cvt_EDID::getManufacturerName(void)
{
	EdidMname = new QString("");
	unsigned short Tmpname;
	Tmpname = data[8] << 8 | data[9];//*(unsigned short *)&(data[8]);//little endian and this way is wrong.
	for (int i = 0; i < 3; i++)
	{
		unsigned short pletter;
		pletter = (Tmpname >> (i * 5) )& 0x1f;
		//qDebug("pletter:%d, %d", pletter, i);
		if (0x00001 <= pletter&&pletter <= 0x11010)
		{
			pletter += 0x40;
			EdidMname->insert(0,(char)pletter);
		}
		//qDebug("pletter:%c, %d", pletter,i);
	}
	return *EdidMname;
}

int Cvt_EDID::getProductCode(void)
{
	EdidPCode = *(unsigned short*)&(data[10]);
	return EdidPCode;
}

int Cvt_EDID::getProductYear(void)
{
	EdidPyear = (unsigned short)(data[17]+1990);
	return EdidPyear;
}

int Cvt_EDID::getProductWeek(void)
{
	EdidPweek = data[16];
	return EdidPweek;
}

bool Cvt_EDID::setSerialNumber(void)
{
	unsigned char TmpSN[13] = {0};
	unsigned char SNaddr;
	SNaddr = isStoredSN();
	if (!SNaddr) return false;
	ParseCustomerBarCode();
	memcpy(data +SNaddr,TmpSN,sizeof(TmpSN));
	return true;
}

void Cvt_EDID::getCustomerBarCode(unsigned char *buf, int len)
{
	barCode = new unsigned char[len];
}

unsigned char Cvt_EDID::ParseCustomerBarCode(void)
{
	return 0x00;
}

unsigned char Cvt_EDID::isStoredSN(void)
{
	unsigned char addr = 0x48;
	unsigned char Addr = 0;
	unsigned char Data[5] = {0x00,0x00,0x00,0xFF,0x00};

	if (memcmp(data + addr, Data, sizeof(Data))) return addr;
	else if (memcmp(data + addr +18, Data, sizeof(Data))) return addr+18;
	else if (memcmp(data + addr +36, Data, sizeof(Data))) return addr+36;
	else return 0;
}

bool Cvt_EDID::getdata(int offset, int bufsize, unsigned char *buf, int Rlen)
{
	qDebug("Get Edid Data?");
	if (data == NULL) return false;
	if (offset*bufsize >= datalen) return false;
	memcpy((void*)buf, (void *)(data + offset*bufsize), Rlen);
	return true;
}

bool Cvt_EDID::setdata(int offset, int bufsize, unsigned char *buf, int Rlen)
{
	if (readdata == NULL) return false;
	if (offset*bufsize >= datalen) return false;
	memcpy((void *)(readdata + offset*bufsize), (void*)buf, Rlen);
	return true;
}

