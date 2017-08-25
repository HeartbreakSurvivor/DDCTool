#include "cvt_burnsetting.h"

Cvt_BurnSetting::Cvt_BurnSetting()
{
	Burn_SlaveAddr = 0x6E;
	Burn_I2cSpeed = 5;
	Burn_writeDelay = 200;
	Burn_readDelay = 200;
	Burn_RetryCnt = 0;
	Burn_PerPackRetryCnt = 0;
	Burn_EdidlastDelay = 200;
	Burn_HdcplastDelay = 200;
	Burn_eraseHdcpkeyDelay = 200;
	Burn_isCreatlogs = 0;
}

Cvt_BurnSetting::Cvt_BurnSetting(int speed, int writedelay, int readdelay, int retrycnt, int spretrycnt,
	int edidlastdelay, int hdcplastdelay, int erasehdcpdelay, bool iscreatlog)
{
	Burn_I2cSpeed = speed;
	Burn_writeDelay = writedelay;
	Burn_readDelay = readdelay;
	Burn_RetryCnt = retrycnt;
	Burn_PerPackRetryCnt = spretrycnt;
	Burn_EdidlastDelay = edidlastdelay;
	Burn_HdcplastDelay = hdcplastdelay;
	Burn_eraseHdcpkeyDelay = erasehdcpdelay;
	Burn_isCreatlogs = iscreatlog;
}

Cvt_BurnSetting::~Cvt_BurnSetting()
{
}

void Cvt_BurnSetting::setI2cSpeed(int speed)
{
	Burn_I2cSpeed = speed;
}

void Cvt_BurnSetting::setwriteDelay(int delay)
{
	Burn_writeDelay = delay;
}

void Cvt_BurnSetting::setreadDelay(int delay)
{
	Burn_readDelay = delay;
}
void Cvt_BurnSetting::setRetryCnt(int cnt)
{
	Burn_RetryCnt = cnt;
}

void Cvt_BurnSetting::setPerpackRetryCnt(int pcnt)
{
	Burn_PerPackRetryCnt = pcnt;
}

int Cvt_BurnSetting::getI2cSpeed(void) const
{
	return Burn_I2cSpeed;
}

int Cvt_BurnSetting::getwriteDelay(void) const
{
	return Burn_writeDelay;
}
void Cvt_BurnSetting::setEdidlastDelay(int delay)
{
	Burn_EdidlastDelay = delay;
}
void Cvt_BurnSetting::setHdcplastDelay(int delay)
{
	Burn_HdcplastDelay = delay;
}
void Cvt_BurnSetting::setEraseHdcpDelay(int delay)
{
	Burn_eraseHdcpkeyDelay = delay;
}
void Cvt_BurnSetting::setCreatlogs(bool iscreate)
{
	Burn_isCreatlogs = iscreate;
}

void Cvt_BurnSetting::setSlaveaddr(unsigned char slaveaddr)
{
	Burn_SlaveAddr = slaveaddr;
}

unsigned char Cvt_BurnSetting::getSlaveaddr(void)
{
	return Burn_SlaveAddr;
}

int Cvt_BurnSetting::getreadDelay(void) const
{
	return Burn_readDelay;
}

int Cvt_BurnSetting::getRetryCnt(void) const
{
	return Burn_RetryCnt;
}

int Cvt_BurnSetting::getPerpackRetryCnt(void) const
{
	return Burn_PerPackRetryCnt;
}
int Cvt_BurnSetting::getEdidlastDelay(void)
{
	return Burn_EdidlastDelay;
}
int Cvt_BurnSetting::getHdcplastDelay(void)
{
	return Burn_HdcplastDelay;
}

int Cvt_BurnSetting::getEraseHdcpDelay(void)
{
	return Burn_eraseHdcpkeyDelay;
}

bool Cvt_BurnSetting::getCreatlogs(void)
{
	return Burn_isCreatlogs;
}
