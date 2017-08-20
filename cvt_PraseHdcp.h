#pragma once
#include "qwidget"
#include "qfile.h"
#include "qdebug.h"
#include "qstring.h"
#include "cvt_global.h"

class Cvt_HDCPKey:public Cvt_BurnData
{
public:
	Cvt_HDCPKey(const QString &filepath);
	Cvt_HDCPKey(int len);
	~Cvt_HDCPKey();

	Cvt_HDCPKey(Cvt_HDCPKey& hdcpkey);
	Cvt_HDCPKey& operator=(Cvt_HDCPKey& hdcpkey);

	bool is_HDCP14KsvValid(unsigned char ksv[]);
	bool is_HDCP22CRCValid(const unsigned char *octets, int len);
	int getKeyid(void);
	virtual bool getdata(int offset, int bufsize, unsigned char *buf, int Rlen);
	virtual bool setdata(int offset, int bufsize, unsigned char *buf, int Rlen);

private:
	int HdcpKeyid;
};

