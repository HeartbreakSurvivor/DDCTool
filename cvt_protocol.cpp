#include "cvt_protocol.h"

Cvt_protocol::Cvt_protocol(quint8 *burnhead, quint8 *readhead)
{
	if (!burnhead || !readhead) return;

	memcpy(Cmd_Burnhead, burnhead, sizeof(Cmd_Burnhead));
	memcpy(Cmd_Readhead, readhead, sizeof(Cmd_Readhead));
	qDebug("Initialize the Protocol ");
}

Cvt_protocol::~Cvt_protocol()
{
}

quint8 Cvt_protocol::getchecksum(quint8 slaveaddr,quint8 *bytes2cal, quint32 len)
{
	quint8 checksum=slaveaddr;
	if (bytes2cal == NULL) return false;
	for (int i = 0; i < len; i++)
	{
		checksum^=*bytes2cal++;
	}
	return checksum;
}

void Cvt_protocol::setPacklen(quint8 packagelen)
{
	Cmd_Burnhead[1] = packagelen;
	Cmd_Readhead[1] = packagelen;
}

void Cvt_protocol::setPackoffset(quint8 offset)
{
	Cmd_Burnhead[5] = offset;
	Cmd_Readhead[5] = offset;
}

void Cvt_protocol::setPackdatalen(quint8 len)
{
	Cmd_Burnhead[6] = len;
	Cmd_Readhead[6] = len;
}

bool Cvt_protocol::Erasehdcpkey(Cvt_Isp_I2C *I2cDevice, quint8 SlaveAddr, quint8 *Erasecmd, quint32 ByteCount,quint32 erasehdcpkeytime)
{
	quint8 feedback[9];

	for (int i = 0; i < ByteCount; i++)
	{
		qDebug("Erase cmd:0x%x", Erasecmd[i]);
	}

	I2cDevice->write(I2cDevice->gethandle(), SlaveAddr, Erasecmd, ByteCount);
	Sleep(erasehdcpkeytime);//erase the hdcpkey wait time.
	for (int i = 0; i < sizeof(feedback); i++)
	{
		I2cDevice->read(I2cDevice->gethandle(), SlaveAddr, &feedback[i], 1);
		qDebug("feedback:0x%x", feedback[i]);
	}
	if (feedback[7] != 0xE0)
	{
		qDebug("Erase hdcpkey failed");
		return false;
	}
	else
	{
		qDebug("Erase hdcpkey successfully");
		return true;
	}
}

bool Cvt_protocol::Getdata(Cvt_Isp_I2C *I2cDevice,quint8 SlaveAddr,quint8 *pReadBuffer, quint32 ByteCount,quint8 offset, quint32 readdelay, quint32 retrycnt)
{
	quint8 *cmd = new quint8[sizeof(Cmd_Readhead) + 1];
	quint8 feedback[25];

	setPackoffset(offset);
	setPackdatalen(ByteCount);
	setPacklen(0x86);//reset the whole package length

	memcpy(cmd, Cmd_Readhead, sizeof(Cmd_Readhead));
	cmd[sizeof(Cmd_Readhead)] = getchecksum(SlaveAddr,cmd, sizeof(Cmd_Readhead));

	for (int i = 0; i < sizeof(Cmd_Readhead)+1; i++)
	{
		qDebug("Getdata cmd:0x%x", cmd[i]);
	}
	qDebug("Getdata retrycnt:0x%x", retrycnt);
	for (int j = 0; j < retrycnt; j++)//The single pack retry mechanism.
	{
		//send the cmd and raedy to read data from board.
		I2cDevice->write(I2cDevice->gethandle(),SlaveAddr,cmd,sizeof(Cmd_Readhead)+1);
		Sleep(readdelay);
		for (int i = 0; i < 25; i++)
		{
			I2cDevice->read(I2cDevice->gethandle(), SlaveAddr, &feedback[i], 1);
			qDebug("feedback:0x%x", feedback[i]);
		}
		if (feedback[0] == 0x6E && feedback[1] == ((ByteCount+5)|0x80))
		{
			qDebug("get data success and break retry cycles");
			for (int j = 0; j < ByteCount; j++)
			{
				*(pReadBuffer + j) = feedback[7 + j];
				//qDebug("ReadBuf:%x", *(pReadBuffer + j));
			}
			break;
		}
		else
		{
			if (j == retrycnt - 1)
			{
				delete[] cmd;
				qDebug("feedback[0] ~=0x6E  and this the last chance and return false!");
				return false;
			}
			qDebug("feedback[0] ~=0x6E  and single pack retry!");
		}
	}
	delete[] cmd;
	return true;
}

bool Cvt_protocol::Burn(Cvt_Isp_I2C *I2cDevice, quint8 SlaveAddr, quint8* pWriteBuffer, quint32 ByteCount, quint8 offset, quint32 writedelay,quint32 retrycnt)
{
	quint8 Cmd_length = sizeof(Cmd_Burnhead) + 1 + ByteCount;
	quint8 *cmd = new quint8[Cmd_length]; 
	quint8 feedback[9] = {0};

	setPackoffset(offset);
	setPackdatalen(ByteCount);
	setPacklen(ByteCount+5+0x80);//reset the whole package length
	 
	memcpy(cmd, Cmd_Burnhead, sizeof(Cmd_Burnhead));
	memcpy(cmd + sizeof(Cmd_Burnhead), pWriteBuffer, ByteCount);
	cmd[Cmd_length - 1] = getchecksum(SlaveAddr,cmd, Cmd_length - 1);
	for (int i = 0; i < Cmd_length; i++)
	{ 
		qDebug("Cmd:0x%x", cmd[i]);
	}
	qDebug("writedelay:%d", writedelay);

	for (int j = 0; j < retrycnt; j++)//The single pack retry mechanism.
	{
		//package finished and ready to send the data.
		I2cDevice->write(I2cDevice->gethandle(),SlaveAddr, cmd, Cmd_length);
		Sleep(writedelay);//write delay
		for (int i = 0; i < 9; i++)
		{
			I2cDevice->read(I2cDevice->gethandle(), SlaveAddr, &feedback[i], 1);
			qDebug("feedback:0x%x", feedback[i]);
		}

		if (feedback[7] == 0xE0 /*&& memcmp(Cmd_Burnhead + 2, feedback + 2, 5)*/)
		{
			qDebug("get data success and break retry cycles");
			break;
		}
		else
		{
			if (j = retrycnt - 1)
			{
				qDebug("feedback[7] ~=0xE0 Tha last chance and return false!");
				delete[] cmd;
				return false;
			}
			qDebug("feedback[7] ~=0xE0  and single pack retry!");
		}
	}
	delete[] cmd;
	return true;
}

