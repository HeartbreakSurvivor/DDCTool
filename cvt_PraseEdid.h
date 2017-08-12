#pragma once
#include "cvt_burndata.h"
#include "qstring.h"
#include "qfile.h"
#include "cvt_global.h"


class Cvt_EDID:public Cvt_BurnData
{
public:
	Cvt_EDID(const QString &filepath);
	Cvt_EDID(const QString &filepath, Cvt_DataType _datatype);
	Cvt_EDID(int len);
	Cvt_EDID();
	Cvt_EDID(Cvt_EDID&);
	Cvt_EDID& operator=(Cvt_EDID&);
	~Cvt_EDID();

	QString getManufacturerName(void);
	int getProductCode(void);
	int getProductYear(void);
	int getProductWeek(void);

	void getCustomerBarCode(unsigned char *buf,int len);
	unsigned char ParseCustomerBarCode(void);
	unsigned char isStoredSN(void);
	bool setSerialNumber(void);
	virtual bool getdata(int offset, int bufsize, unsigned char *buf, int Rlen);
	virtual bool setdata(int offset, int bufsize, unsigned char *buf, int Rlen);

private:
	QString *EdidMname;
	unsigned short EdidPCode;
	unsigned short EdidPyear;
	unsigned char EdidPweek;

	unsigned char *barCode;
	unsigned char SerialNumber[13];
};



