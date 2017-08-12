#pragma once
#include "cvt_isp_iic.h"
#include "qdebug.h"
#include <Windows.h>


class Cvt_protocol
{
public:
	Cvt_protocol(quint8 *burnhead,quint8 *readhead);
	~Cvt_protocol();
	void setPacklen(quint8 packagelen);
	void setPackoffset(quint8 offset);
	void setPackdatalen(quint8 len);
	quint8 getchecksum(quint8 slaveaddr, quint8 *bytes2cal, quint32 len);
	bool Erasehdcpkey(Cvt_Isp_I2C *I2cDevice, quint8 SlaveAddr, quint8 *erasecmd, quint32 ByteCount, quint32 erasehdcpkeytime);
	bool Getdata(Cvt_Isp_I2C *I2cDevice, quint8 SlaveAddr, quint8 *pReadBuffer, quint32 ByteCount,quint8 offset, quint32 readdelay, quint32 retrycnt);
	bool Burn(Cvt_Isp_I2C *I2cDevice, quint8 SlaveAddr, quint8* pWriteBuffer, quint32 ByteCount,quint8 offset, quint32 writedelay, quint32 retrycnt);
	quint8 Cmd_Burnhead[7] = {0};
	quint8 Cmd_Readhead[8] = {0};
private:
	//Cvt_Isp_I2C *I2c_device;
};
