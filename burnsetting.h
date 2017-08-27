#pragma once

struct BurnSetting_T
{
public:
    BurnSetting_T();
    BurnSetting_T(unsigned char slaveaddr,int speed, int writedelay, int readdelay, int retrycnt, int spretrycnt,
		int edidlastdelay, int hdcplastdelay, int erasehdcpdelay,bool iscreatlog);

    ~BurnSetting_T();
	void setI2cSpeed(int speed);
	void setwriteDelay(int delay);
	void setreadDelay (int delay);
	void setRetryCnt(int cnt);
	void setPerpackRetryCnt(int pcnt);
	void setEdidlastDelay(int delay);
	void setHdcplastDelay(int delay);
	void setEraseHdcpDelay(int delay);
	void setSlaveaddr(unsigned char slaveaddr);
	void setCreatlogs(bool iscreatlog);

    int getI2cSpeed(void) const;//const type function
	int getwriteDelay(void) const;
	int getreadDelay(void) const;
	int getRetryCnt(void) const;
	int getPerpackRetryCnt(void) const;
	int getEdidlastDelay(void);
	int getHdcplastDelay(void);
	int getEraseHdcpDelay(void);
	bool getCreatlogs(void);	
	unsigned char getSlaveaddr(void);

private:
    unsigned char m_slaveaddr=0x6E;
    bool m_iscreatelogs;
    int m_i2cspeed;
    int m_writedelay;//ms
    int m_readdelay;//ms
    int m_retrycnt;
    int m_perpackretrycnt;
    int m_edidlastdelay;//ms
    int m_hdcplastdelay;//ms
    int m_erasehdcpkeydelay;
};

