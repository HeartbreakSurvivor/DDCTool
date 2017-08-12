#pragma once

class Cvt_BurnSetting
{
public:
	Cvt_BurnSetting();
	Cvt_BurnSetting(int speed, int writedelay, int readdelay, int retrycnt, int spretrycnt,
		int edidlastdelay, int hdcplastdelay, int erasehdcpdelay,bool iscreatlog);

	~Cvt_BurnSetting();
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

	int getI2cSpeed(void) const;
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
	unsigned char Burn_SlaveAddr=0x6E;
	bool Burn_isCreatlogs;
	int Burn_I2cSpeed;
	int Burn_writeDelay;//ms
	int Burn_readDelay;//ms
	int Burn_RetryCnt;
	int Burn_PerPackRetryCnt;
	int Burn_EdidlastDelay;//ms
	int Burn_HdcplastDelay;//ms
	int Burn_eraseHdcpkeyDelay;
};

