#include "cvt_burndata.h"

Cvt_BurnData::Cvt_BurnData()
{
}

Cvt_BurnData::~Cvt_BurnData()
{
}

int Cvt_BurnData::getLength(void)
{
	return datalen;
}

bool Cvt_BurnData::getdata(int offset, int bufsize, unsigned char *buf, int Rlen)
{
	return true;
}

bool Cvt_BurnData::setdata(int offset, int bufsize, unsigned char *buf, int Rlen)
{
	/*
	if (readdata == NULL) return false;
	if (offset*bufsize >= datalen) return false;
	memcpy((void *)(readdata + offset*bufsize), (void*)buf, Rlen);*/
	return true;
}

bool Cvt_BurnData::verifydata(void)
{
	if (data == NULL || readdata == NULL) return false;
	for (int i = 0; i < datalen; i++)
	{
		qDebug("Data[%d]:0x%x,ReadData[%d]:0x%x", i, data[i], i, readdata[i]);
	}

	for (int i = 0; i < datalen; i++)
	{
		if (data[i] != readdata[i])
		{
			return false;
		}
	}
	return true;
}

void Cvt_BurnData::setdatatype(Cvt_DataType type)
{
	datatype = type;
}

Cvt_DataType Cvt_BurnData::getdatatype()
{
	return datatype;
}
