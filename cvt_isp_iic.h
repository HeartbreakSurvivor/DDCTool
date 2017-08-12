#pragma once
#include <Windows.h>
#include "lib/FTCI2C.h"
#include "qdebug.h"

class Cvt_Isp_I2C
{
public:
	Cvt_Isp_I2C();
	~Cvt_Isp_I2C();

	FTC_HANDLE& gethandle(void);
	FTC_STATUS openDevice(FTC_HANDLE &ftHandle, DWORD dwSpeedHz);
	void closeDevice(FTC_HANDLE &ftHandle);
	FTC_STATUS setI2Speed(FTC_HANDLE ftHandle, DWORD dwSpeedHz);
	FTC_STATUS read(FTC_HANDLE ftHandle, quint8 SlaveAddr, quint8 *pReadBuffer, quint32 ByteCount);
	FTC_STATUS write(FTC_HANDLE ftHandle, quint8 SlaveAddr, quint8* pWriteBuffer, quint32 ByteCount);
private:
	FTC_STATUS findDevice(void);
	FTC_HANDLE fthandle;//the device handle
	DWORD dwDeviceNameIndex;//indicate the current deviceNameIdx
};
