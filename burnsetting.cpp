#include "burnsetting.h"
#include <qdebug.h>

BurnSetting_T::BurnSetting_T()
{
    m_slaveaddr = 0x6E;
    m_i2cspeed = 100;
    m_writedelay = 200;
    m_readdelay = 200;
    m_retrycnt = 3;
    m_perpackretrycnt = 3;
    m_edidlastdelay = 200;
    m_hdcplastdelay = 200;
    m_erasehdcpkeydelay = 200;
    m_iscreatelogs = 0;
}

BurnSetting_T::BurnSetting_T(unsigned char slaveaddr,int speed, int writedelay, int readdelay, int retrycnt, int spretrycnt,
	int edidlastdelay, int hdcplastdelay, int erasehdcpdelay, bool iscreatlog)
{
    m_slaveaddr = slaveaddr;
    m_i2cspeed = speed;
    m_writedelay = writedelay;
    m_readdelay = readdelay;
    m_retrycnt = retrycnt;
    m_perpackretrycnt = spretrycnt;
    m_edidlastdelay = edidlastdelay;
    m_hdcplastdelay = hdcplastdelay;
    m_erasehdcpkeydelay = erasehdcpdelay;
    m_iscreatelogs = iscreatlog;
}

BurnSetting_T::~BurnSetting_T()
{
}

void BurnSetting_T::setI2cSpeed(int speed)
{
    m_i2cspeed = speed;
}

void BurnSetting_T::setwriteDelay(int delay)
{
    m_writedelay = delay;
}

void BurnSetting_T::setreadDelay(int delay)
{
    m_readdelay = delay;
}
void BurnSetting_T::setRetryCnt(int cnt)
{
    m_retrycnt = cnt;
}

void BurnSetting_T::setPerpackRetryCnt(int pcnt)
{
    m_perpackretrycnt = pcnt;
}

int BurnSetting_T::getI2cSpeed(void) const
{
    return m_i2cspeed;
}

int BurnSetting_T::getwriteDelay(void) const
{
    return m_writedelay;
}
void BurnSetting_T::setEdidlastDelay(int delay)
{
    m_edidlastdelay = delay;
}
void BurnSetting_T::setHdcplastDelay(int delay)
{
    m_hdcplastdelay = delay;
}
void BurnSetting_T::setEraseHdcpDelay(int delay)
{
    m_erasehdcpkeydelay = delay;
}
void BurnSetting_T::setCreatlogs(bool iscreate)
{
    m_iscreatelogs = iscreate;
}

void BurnSetting_T::setSlaveaddr(unsigned char slaveaddr)
{
    m_slaveaddr = slaveaddr;
}

unsigned char BurnSetting_T::getSlaveaddr(void)
{
    return m_slaveaddr;
}

int BurnSetting_T::getreadDelay(void) const
{
    return m_readdelay;
}

int BurnSetting_T::getRetryCnt(void) const
{
    return m_retrycnt;
}

int BurnSetting_T::getPerpackRetryCnt(void) const
{
    return m_perpackretrycnt;
}
int BurnSetting_T::getEdidlastDelay(void)
{
    return m_edidlastdelay;
}
int BurnSetting_T::getHdcplastDelay(void)
{
    return m_hdcplastdelay;
}

int BurnSetting_T::getEraseHdcpDelay(void)
{
    return m_erasehdcpkeydelay;
}

bool BurnSetting_T::getCreatlogs(void)
{
    return m_iscreatelogs;
}
