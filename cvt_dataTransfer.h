#pragma once
#include "qthread.h"
#include "cvt_burnsetting.h"
#include "cvt_global.h"
#include "cvt_protocol.h"
#include "cvt_isp_iic.h"
#include "burn_data.h"

class Cvt_DataThread : public QThread
{
	Q_OBJECT//if you don't add this ,the signal ans slots will generate link error.
public:
	Cvt_DataThread();
	bool TransferWholePackage(Cvt_protocol* protocol, quint32 nTimes, quint8 remainder, quint8 Rlength, quint8 length);
	bool ReadWholePackage(Cvt_protocol* protocol,quint32 nTimes, quint8 remainder, quint8 Rlength, quint8 length);
    Cvt_DataThread(Cvt_flowControlPar *flow_control, Cvt_BurnSetting* burnsettings, ddc::BurnData_T* burndata, Cvt_Isp_I2C* I2c)
	{
		_flowcontrol = flow_control;
		_burn_Setting = burnsettings;
		_burn_data = burndata;
		_I2cDevice = I2c;
	}

	Cvt_DataThread(Cvt_flowControlPar *flow_control, Cvt_BurnSetting* burnsettings, Cvt_Isp_I2C* I2c)
	{
		_flowcontrol = flow_control;
		_burn_Setting = burnsettings;
		_I2cDevice = I2c;
	}

	~Cvt_DataThread();
	void run();

signals:
	void BurnMsg(int num);

public:
	Cvt_flowControlPar *_flowcontrol;
	Cvt_BurnSetting *_burn_Setting;
    ddc::BurnData_T *_burn_data;
	Cvt_Isp_I2C *_I2cDevice;
};


class Cvt_DataManage:public Cvt_DataThread
{
	Q_OBJECT
public:
	Cvt_DataManage();
    Cvt_DataManage(unsigned char _edidports,Cvt_BurnSetting* ediddata,  Cvt_flowControlPar *flow_control, Cvt_BurnSetting* burnsettings, Cvt_Isp_I2C* I2c);
	~Cvt_DataManage();
	int num_;
	void run();

public slots:
	void GetBurnMsg(int num);
	
signals:
	void EdidBurnMsg(int num);
private:

    unsigned char *ediddatas = NULL;
	unsigned char EdidPorts;//store the edid ports the user has selected.
	Cvt_DataThread *DataTransfer;//data transfer thread.
};


