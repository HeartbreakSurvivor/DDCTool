#pragma once
#include "cvt_global.h"
#include "qdebug.h"

class Cvt_BurnData
{
public:
	Cvt_BurnData();
	~Cvt_BurnData();
	bool verifydata(void);
	int getLength(void);
	void setdatatype(Cvt_DataType type);
	Cvt_DataType getdatatype(void);
	virtual bool getdata(int offset, int bufsize, unsigned char *buf, int Rlen);
	virtual bool setdata(int offset, int bufsize, unsigned char *buf, int Rlen);

protected:
	int datalen = 0;
	Cvt_DataType datatype;
	unsigned char *data=NULL;//the data to write
	unsigned char *readdata=NULL;//to save the read data
};
