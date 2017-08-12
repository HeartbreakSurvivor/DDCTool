#include "cvt_PraseHdcp.h"

Cvt_HDCPKey::Cvt_HDCPKey(const QString &filepath)
{
	QFile *Hdcpfile = new QFile(filepath);
	if (!Hdcpfile->open(QIODevice::ReadOnly)) return;

	datatype = DATATYPE_HDCPKEY;
    datalen = Hdcpfile->size();
	data = new unsigned char[datalen];
	readdata = new unsigned char[datalen];
	if (Hdcpfile->read((char *)data, datalen) == -1) return;

	for (int i = 0; i < datalen; i++)
		qDebug("hdcpkey%d: %x", i, data[i]);
	qDebug("hdcpkey length:%d",datalen);
	Hdcpfile->close();
}

Cvt_HDCPKey::Cvt_HDCPKey(int len)
{
	datalen = len;
	data = new unsigned char[datalen];
	readdata = new unsigned char[datalen];
}

Cvt_HDCPKey::~Cvt_HDCPKey()
{
	qDebug() << "The hdcpkey class is destroyed";
	delete[] data;
	delete[] readdata;
}

Cvt_HDCPKey::Cvt_HDCPKey(Cvt_HDCPKey& hdcpkey)
{
	datalen = hdcpkey.getLength();/*常规成员变量*/
	datatype = hdcpkey.datatype;

	//The king point
	data = new unsigned char[datalen];
	memcpy(data, hdcpkey.data, datalen * sizeof(unsigned char));

	readdata = new unsigned char[datalen];
	memcpy(readdata, hdcpkey.readdata, datalen * sizeof(unsigned char));

	qDebug("Copy the hdcpkey memory1 ");
}

Cvt_HDCPKey& Cvt_HDCPKey::operator=(Cvt_HDCPKey& hdcpkey)
{
	if (this == &hdcpkey)  /*自赋值*/
		return *this;
	datalen = hdcpkey.getLength();
	datatype = hdcpkey.datatype;
	//The king point
	data = new unsigned char[datalen];
	memcpy(data, hdcpkey.data, datalen * sizeof(unsigned char));

	readdata = new unsigned char[datalen];
	memcpy(readdata, hdcpkey.readdata, datalen * sizeof(unsigned char));

	qDebug("Copy the hdcpkey memory2");
	return *this;
}

bool Cvt_HDCPKey::is_HDCP14KsvValid(unsigned char ksv[])
{
	typedef long long L;
	L KSV = (L)ksv[0] << 32 | (L)ksv[1] << 24 | (L)ksv[2] << 16 | (L)ksv[3] << 8 | (L)ksv[4];

	int i = 0, count = 0;
	for (i = 0; i<40; i++)
	{
		count += ((KSV >> i) & 0x1);
	}
	if (count != 20)
		return 0;
	else
		return 1;
}

bool Cvt_HDCPKey::is_HDCP22CRCValid(const unsigned char *octets, int len)
{
	unsigned long crc = 0xFFFFFFFF;
	unsigned long temp;
	unsigned long result;
	int j;
	while (len--)
	{
		temp = (unsigned long)((crc & 0xFF) ^ *octets++);
		for (j = 0; j < 8; j++)
		{
			if (temp & 0x1)
			{
				temp = (temp >> 1) ^ 0xEDB88320;
			}
			else
			{
				temp >>= 1;
			}
		}
		crc = (crc >> 8) ^ temp;
	}
	result =  crc ^ 0xFFFFFFFF;

	return  (result == *(unsigned long*)(octets + len)) ? true : false;
}

int Cvt_HDCPKey::getKeyid(void)
{
	HdcpKeyid = *(unsigned int*)data[0x128];
	return HdcpKeyid+1;
}

bool Cvt_HDCPKey::getdata(int offset, int bufsize, unsigned char *buf,int Rlen)
{
	if (data == NULL) return false;
	if (offset*bufsize >= datalen) return false;
	memcpy((void*)buf, (void *)(data+offset*bufsize), Rlen);
	return true;
}

bool Cvt_HDCPKey::setdata(int offset, int bufsize, unsigned char *buf, int Rlen)
{
	if (readdata == NULL) return false;
	if (offset*bufsize >= datalen) return false;
	memcpy((void *)(readdata + offset*bufsize), (void*)buf, Rlen);
	return true;
}
