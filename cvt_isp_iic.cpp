#include "cvt_isp_iic.h"

FTC_STATUS Cvt_Isp_I2C::findDevice(void)
{
	FTC_STATUS status;
	FTC_DEVICE_INFO stFtcDevInfo;
	DWORD dwNumDevices = 0;
	for (int i = 0; i < 10; i++)
	{
		status = I2C_GetNumDevices(&dwNumDevices);
		if (status == FTC_SUCCESS)
			break;
		Sleep(500);
	}
	if (status != FTC_SUCCESS)
	{
		qDebug("Can't find any devices");
		return status;
	}
	qDebug("The num of device:%d", dwNumDevices);
	for (unsigned int i = 0; i < dwNumDevices; i++)
	{
		status = I2C_GetDeviceInfo(i, &stFtcDevInfo);
		qDebug("设备ID: %d, 设备名: %s, 设备类型: 0x%X, LocID: 0x%X, Vendor ID: 0x%04X, Product ID: 0x%04X, 序列号: %s\n",
			i, stFtcDevInfo.szDeviceName, stFtcDevInfo.dwDeviceType, stFtcDevInfo.dwLocationID, stFtcDevInfo.dwProductVendorID >> 16, stFtcDevInfo.dwProductVendorID & 0xFFFF, stFtcDevInfo.szSerialNumber);
		if (status == FTC_SUCCESS)
		{
			dwDeviceNameIndex = i;
            qDebug("查找设备成功，设备ID: %d, 设备名: %s, 设备类型: 0x%X, LocID: 0x%X, Vendor ID: 0x%04X, Product ID: 0x%04X, 序列号: %s\n",
				i, stFtcDevInfo.szDeviceName, stFtcDevInfo.dwDeviceType, stFtcDevInfo.dwLocationID, stFtcDevInfo.dwProductVendorID >> 16, stFtcDevInfo.dwProductVendorID & 0xFFFF, stFtcDevInfo.szSerialNumber);
			break;
		}
		else
		{
			dwDeviceNameIndex = 0xFFFF;
			status = FTC_DEVICE_NOT_FOUND;
		}
	}
	return status;
}

FTC_STATUS Cvt_Isp_I2C::openDevice(FTC_HANDLE &ftHandle, DWORD dwSpeedHz)
{
	FTC_STATUS status;
	if (findDevice() == FTC_SUCCESS)
	{
		status = I2C_OpenByDeviceNameIndex(dwDeviceNameIndex, &ftHandle);
		qDebug("\n%s, %d, ftHandle: 0x%X", __FUNCTION__, __LINE__, ftHandle);
		if (status == FTC_SUCCESS)
		{
			if (setI2Speed(ftHandle, dwSpeedHz) == FTC_SUCCESS)
			{
				qDebug("open device successfully.");
			}
			else
			{
				qDebug("open device failed.");
			}
		}
	}
	else
	{
		qDebug("find device failed.");
	}
	return status;
}

void Cvt_Isp_I2C::closeDevice(FTC_HANDLE &ftHandle)
{
	I2C_Close(ftHandle);
	ftHandle = NULL;
}

FTC_STATUS Cvt_Isp_I2C::setI2Speed(FTC_HANDLE ftHandle, DWORD dwSpeedHz)//unit: Hz
{
	FTC_STATUS status;
	DWORD dwClockFrequencyHz;

	unsigned int dwClockDivisor = 6000000L / dwSpeedHz - 1;
	status = I2C_GetClock(dwClockDivisor, &dwClockFrequencyHz);//formula: frequency = 12MHz/((1 + dwClockDivisor) * 2).
	if (status == FTC_SUCCESS)
	{
		status = I2C_InitDevice((FTC_HANDLE)ftHandle, dwClockDivisor);
		if (status == FTC_SUCCESS)
		{
			if (dwSpeedHz > 100000)
				status = I2C_SetMode(ftHandle, FAST_MODE);
			else
				status = I2C_SetMode(ftHandle, STANDARD_MODE);
		}
    }
	qDebug("Change I2C Speed to %fKHz", dwSpeedHz / 1000.0f);
	return status;
}

FTC_STATUS Cvt_Isp_I2C::read(FTC_HANDLE ftHandle, quint8 SlaveAddr, quint8 *pReadBuffer, quint32 ByteCount)
{
	FTC_STATUS status;
	SlaveAddr |= 0x01;
	status = I2C_Isp_Read(ftHandle, SlaveAddr, pReadBuffer, ByteCount);
	return status;
}

FTC_STATUS Cvt_Isp_I2C::write(FTC_HANDLE ftHandle, quint8 SlaveAddr, quint8* pWriteBuffer, quint32 ByteCount)
{
	FTC_STATUS status;
	//for (unsigned int i = 0; i < 5; i++)
	{
		status = I2C_Isp_Write(ftHandle, SlaveAddr, pWriteBuffer, ByteCount);
		if (status == FTC_SUCCESS)
		{
			qDebug("I2C Write successfully!");
			//break;
		}
		Sleep(10);
	}
	return status;
}

FTC_HANDLE& Cvt_Isp_I2C::gethandle(void)
{
	return fthandle;
}

Cvt_Isp_I2C::Cvt_Isp_I2C()
{
	fthandle = NULL; 
	dwDeviceNameIndex = 0;
}

Cvt_Isp_I2C::~Cvt_Isp_I2C()
{
}
