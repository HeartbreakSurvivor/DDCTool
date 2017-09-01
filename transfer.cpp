#include "transfer.h"

namespace ddc {

#define		PERPACK_LEN			16

void Transfer_T::setburnCmd(burnCmd_t *burnmsg)
{
    m_databody = burnmsg->burndata;
    m_bodysize = burnmsg->datalen;
    m_assemblefunc = burnmsg->assemblefunc;

    m_protocol.setfeedbacklen(burnmsg->feedbacklen);
    m_protocol.setcbfunc(burnmsg->verifyfunc);

    m_protocol.setwritedelay(burnmsg->delay);
    m_retrycnt = burnmsg->retrycnt;
}

void Transfer_T::setburndata(quint8* data,quint32 size)
{
    m_databody = data;
    m_bodysize = size;
}

bool Transfer_T::transferpackage()
{
    quint32 length = PERPACK_LEN;
    quint32 Rlength = length;
    quint32 remainder = m_bodysize % 16;
    quint32 nTimes = (remainder == 0) ? m_bodysize/(length-1) : m_bodysize/length;//how many packs we should send to board.

    quint8 buf[30] = {0};//temp buffer to store the send data

    for (quint32 i = 0; i <= nTimes; i++)
	{
		qDebug("The %d times\n", i);
        //这里可以被简化掉，上层的逻辑应该由上层来决策，修改成回调来实现,实际上，edid hdcp最后一包延时和普通的包延时，以及擦除hdcp的延时没有区别，transfer只负责传输数据，具体逻辑上层来实现
		if (i == nTimes)
		{
			qDebug("The last times\n");
			if (remainder != 0)
			{
				Rlength = remainder;
			}
            if(m_edidlpdelay!=-1&&(m_hdcplpdelay==-1||m_erasedelay==-1))
            {
                qDebug()<<"this is edid burn process";
                m_protocol.setwritedelay(m_edidlpdelay);
            }
            else if((m_hdcplpdelay!=-1&&m_erasedelay!=-1)&&m_edidlpdelay==-1)
            {
                qDebug()<<"this is hdcp burn process";
                m_protocol.setwritedelay(m_hdcplpdelay);
            }
		}

        //assemble the data
        memcpy((void*)buf, (void *)(m_databody+i*length), Rlength);//get data from the transfer

        if(m_assemblefunc)
        {
            this->m_assemblefunc(i,Rlength);//assemble the data,(append the head and set the offset and package data len)
        }

        m_protocol.setPackdata(buf,Rlength);

        if(m_protocol.burn())
		{
            qDebug("The singale pack burn success and start next pack\n");
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

void Transfer_T::run()
{
    qDebug()<<"data transfer thread start!";
    for (int k = 0; k < m_retrycnt; k++)
    {
        if (transferpackage())
        {
            qDebug("Burning Pass and ready to read !");
            break;
        }
        else
        {
            qDebug("Burning Failed,Retry Times:%d!", k);
            if (k == m_retrycnt-1)//the last chance
            {
                qDebug("The last retry choice and failed!");
                //emit BurnMsg(1);//burn failed
                return;
            }
        }
    }
    //emit BurnMsg(4);//burn success and return

    qDebug()<<"data transfer thread exit!";
}

/*
void Cvt_DataManage::run()
{
	for (int i = 0; i < EdidPorts_Num; i++)
	{
		if (EdidPorts&(1<<i))
		{
			emit EdidBurnMsg(i);//send the signals to notify the UI display the edid will be burn.

            DataTransfer = new Transfer_T(_flowcontrol, _burn_Setting, (Cvt_BurnData*)&ediddatas[i], _I2cDevice);
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
*/
}
