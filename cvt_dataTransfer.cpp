#include "cvt_dataTransfer.h"
#include "cvt_global.h"
#include "qobject.h"

Cvt_Burningcmd cmd[8] = {
	0x51,0x95,0xD0,0x19,0x24,0x00,0x10,   0x51,0x86,0xD0,0x21,0x24,0x00,0x10,0x00,//DATATYPE_EDID_VGA
	0x51,0x95,0xD0,0x23,0x24,0x00,0x10,   0x51,0x86,0xD0,0x25,0x24,0x00,0x10,0x00,//DATATYPE_EDID_DVI
	0x51,0x95,0xD0,0x27,0x24,0x00,0x10,   0x51,0x86,0xD0,0x29,0x24,0x00,0x10,0x00,//DATATYPE_EDID_HDMI1
	0x51,0x95,0xD0,0x31,0x24,0x00,0x10,   0x51,0x86,0xD0,0x33,0x24,0x00,0x10,0x00,//DATATYPE_EDID_HDMI2
	0x51,0x95,0xD0,0x41,0x24,0x00,0x10,   0x51,0x86,0xD0,0x43,0x24,0x00,0x10,0x00,//DATATYPE_EDID_HDMI3
	0x51,0x95,0xD0,0x35,0x24,0x00,0x10,   0x51,0x86,0xD0,0x37,0x0D,0x00,0x10,0x00,//DATATYPE_EDID_DP
	0x51,0x95,0xD0,0x65,0x07,0x00,0x10,   0x51,0x86,0xD0,0x73,0x07,0x00,0x10,0x00,//DATATYPE_HDCPKEY
};

unsigned char ErasehdcpkeyCmd[9] = {0x51,0x86,0xD0,0x65,0x0A,0xFF,0x00,0x00,0xF9};

Cvt_DataThread::Cvt_DataThread()
{
}

Cvt_DataThread::~Cvt_DataThread()
{
}

bool Cvt_DataThread::TransferWholePackage(Cvt_protocol* protocol, quint32 nTimes, quint8 remainder, quint8 Rlength, quint8 length)
{
	unsigned char tmpbuf[Cvt_PerPackLen] = { 0 };//temp buffer to store the send data
	quint32 writedelay = _burn_Setting->getwriteDelay();
	quint32 singlepackrertycnt = _burn_Setting->getPerpackRetryCnt();
	for (int i = 0; i <= nTimes; i++)
	{
		qDebug("The %d times\n", i);
		if (i == nTimes)
		{
			qDebug("The last times\n");
			if (remainder != 0)
			{
				Rlength = remainder;
			}
			if (_burn_data->getdatatype() != DATATYPE_HDCPKEY)
			{
				writedelay = _burn_Setting->getEdidlastDelay();
			}
			else
			{
				writedelay = _burn_Setting->getHdcplastDelay();
			}
		}

		_burn_data->getdata(i, length, tmpbuf, Rlength);//get data from Edid and hdcpkey

		if (protocol->Burn(_I2cDevice, _burn_Setting->getSlaveaddr(), tmpbuf, Rlength, i, writedelay, singlepackrertycnt))
		{
			qDebug("The singale pack burn  success and start next pack\n");
			if (i == nTimes) return true;
		}
		else
		{
			qDebug("The single pack rerty failed and return\n");
			return false;
		}
	}
	return true;
}

bool Cvt_DataThread::ReadWholePackage(Cvt_protocol* protocol, quint32 nTimes, quint8 remainder, quint8 Rlength, quint8 length)
{
	unsigned char tmpbuf[Cvt_PerPackLen] = { 0 };
	for (int i = 0; i <= nTimes; i++)
	{
		qDebug("The %d times\n", i);
		if (i == nTimes)
		{
			qDebug("The last times\n");
			if (remainder != 0)
			{
				Rlength = remainder;
			}
		}
		if (protocol->Getdata(_I2cDevice, _burn_Setting->getSlaveaddr(), tmpbuf, Rlength, i, _burn_Setting->getreadDelay(), _burn_Setting->getPerpackRetryCnt()))
		{
			qDebug("Getdata success---\n");
			_burn_data->setdata(i, length, tmpbuf, Rlength);//get data from Edid and hdcpkey
			for (int l = 0; l<16; l++)
				qDebug("tmpbuf:0x%x", tmpbuf[l]);
			if (i == nTimes) return true;
		}
		else
		{
			qDebug("Getdata failed and retry\n");
			return false;
		}
	}
	return true;
}

void Cvt_DataThread::run()
{
	Cvt_protocol *protocol = new Cvt_protocol(cmd[_burn_data->getdatatype()].burnhead, cmd[_burn_data->getdatatype()].readhead);

	quint32 length = Cvt_PerPackLen;
	quint32 Rlength = length;
	quint32 remainder = _burn_data->getLength() % 16;
	quint32 nTimes = (remainder == 0) ? _burn_data->getLength() / length - 1 : _burn_data->getLength() / length;//how many packs we should send to board.
	unsigned char tmpbuf[Cvt_PerPackLen] = { 0 };//temp buffer to store the send data

	qDebug("datalen:%d\n", _burn_data->getLength());
	qDebug("remainder:%d,nTimes:%d\n", remainder, nTimes);
	qDebug("Retry Cnt:%d", _burn_Setting->getRetryCnt());
	qDebug("Single pack Retry Cnt:%d", _burn_Setting->getPerpackRetryCnt());
	qDebug("Erase HDCP time:%d", _burn_Setting->getEraseHdcpDelay());
	//-----Erase the hdcpkey if need.
	if (_flowcontrol->isErasehdcp)
	{
		qDebug("Erase the HDCPKEY ");
		for (int k = 0; k < _burn_Setting->getRetryCnt(); k++)
		{
			if (protocol->Erasehdcpkey(_I2cDevice, _burn_Setting->getSlaveaddr(), ErasehdcpkeyCmd, sizeof(ErasehdcpkeyCmd), _burn_Setting->getEraseHdcpDelay()))
			{
				qDebug("Erase the hdcpkey successfully !");
				break;
			}
			else
			{
				if (k == _burn_Setting->getRetryCnt()-1)
				{
					qDebug("The last retry choice and failed!");
					qDebug("Quit the Thread!!!");
					emit BurnMsg(0);//erase failed
					delete protocol;
					return;
				}
				qDebug("Erase the hdcpkey failed !");
			}
		}
	}
	
	//Ready to Burn the data
	if (_flowcontrol->isBurn)
	{
		for (int k = 0; k < _burn_Setting->getRetryCnt(); k++)
		{
			if (TransferWholePackage(protocol, nTimes, remainder, Rlength, length))
			{
				qDebug("Burning Pass and ready to read !");
				break;
			}
			else
			{
				qDebug("Burning Failed,Retry Times:%d!", k);
				if (k == _burn_Setting->getRetryCnt() - 1)
				{
					qDebug("The last retry choice and failed!");
					qDebug("Quit the Thread!!!");
					emit BurnMsg(1);//burn failed
					delete protocol;
					return;
				}
			}
		}
	}

	//Read to get the data
	if (_flowcontrol->isRead)
	{
		for (int k = 0; k < _burn_Setting->getRetryCnt(); k++)
		{
			if (ReadWholePackage(protocol, nTimes, remainder, Rlength, length))
			{
				qDebug("Burning Pass and ready to read !");
				break;
			}
			else
			{
				qDebug("Burning Failed,Retry Times:%d!", k);
				if (k == _burn_Setting->getRetryCnt() - 1)
				{
					qDebug("The last retry choice and failed!");
					qDebug("Quit the Thread!!!");
					emit BurnMsg(2);//burn failed
					delete protocol;
					return;
				}
			}
		}
	}

	//Ready to verify the burn data and get data.
	if (_flowcontrol->isVerify)
	{
		if (!_burn_data->verifydata())
		{
			emit BurnMsg(3);//burn failed
			delete protocol;
			return;
		}
	}
	emit BurnMsg(4);//burn success and return 
	delete protocol;
}

/************DataManage*************/
Cvt_DataManage::Cvt_DataManage()
{
}

Cvt_DataManage::~Cvt_DataManage()
{
}

Cvt_DataManage::Cvt_DataManage(unsigned char _edidports, Cvt_EDID* ediddata, Cvt_flowControlPar *flow_control, Cvt_BurnSetting* burnsettings, Isp_I2C* I2c) :Cvt_DataThread(flow_control, burnsettings,I2c)
{
	ediddatas = ediddata;
	EdidPorts = _edidports;
	qDebug() << QString::fromLocal8Bit("Create the data manage Thread");
}

void Cvt_DataManage::GetBurnMsg(int num)
{
	num_ = num;
}

void Cvt_DataManage::run()
{
	for (int i = 0; i < EdidPorts_Num; i++)
	{
		if (EdidPorts&(1<<i))
		{
			emit EdidBurnMsg(i);//send the signals to notify the UI display the edid will be burn.

			DataTransfer = new Cvt_DataThread(_flowcontrol, _burn_Setting, (Cvt_BurnData*)&ediddatas[i], _I2cDevice);
			connect(DataTransfer, SIGNAL(BurnMsg(int)), this, SLOT(GetBurnMsg(int)));
			DataTransfer->start();
			DataTransfer->wait();
			while (DataTransfer->isRunning())
			{
				msleep(500);
				qDebug() << QString::fromLocal8Bit("runing");
			}
			emit EdidBurnMsg(num_+10);
			disconnect(DataTransfer);
			qDebug() << QString::fromLocal8Bit("ready to burn next edid");
			delete DataTransfer;
		}
	}
}
