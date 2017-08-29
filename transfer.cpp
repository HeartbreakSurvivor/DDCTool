#include "transfer.h"

namespace ddc {

#define		PERPACK_LEN			16

Transfer_T::Transfer_T(DDCProtocol_T& protocol,burnMsg_t* burnmsg,quint8 retrycnt):m_protocol(protocol)
{
    //m_protocol = protocol;
    m_data2send = burnmsg->burndata;
    m_datasize = burnmsg->datalen;
    m_assemblefunc = burnmsg->assemblefunc;
    m_protocol.setfeedbacklen(burnmsg->feedbacklen);
    m_protocol.setcbfunc(burnmsg->verifyfunc);
    m_retrycnt = retrycnt;
}

Transfer_T::Transfer_T(DDCProtocol_T& protocol,burnMsg_t* burnmsg,quint8 retrycnt,int edidlpdelay):m_protocol(protocol)
{
   // m_protocol = protocol;
    m_data2send = burnmsg->burndata;
    m_datasize = burnmsg->datalen;
    m_assemblefunc = burnmsg->assemblefunc;
    m_protocol.setfeedbacklen(burnmsg->feedbacklen);
    m_protocol.setcbfunc(burnmsg->verifyfunc);
    m_edidlpdelay = edidlpdelay;
    m_retrycnt = retrycnt;
}

Transfer_T::Transfer_T(DDCProtocol_T& protocol,burnMsg_t* burnmsg,quint8 retrycnt,int erasehdcpdelay,int hdcplpdelay):m_protocol(protocol)
{
    //m_protocol = protocol;
    m_data2send = burnmsg->burndata;
    m_datasize = burnmsg->datalen;
    m_assemblefunc = burnmsg->assemblefunc;
    m_protocol.setfeedbacklen(burnmsg->feedbacklen);
    m_protocol.setcbfunc(burnmsg->verifyfunc);
    m_erasedelay = erasehdcpdelay;
    m_hdcplpdelay = hdcplpdelay;
    m_retrycnt = retrycnt;
}

bool Transfer_T::transferpackage()
{
    quint32 length = PERPACK_LEN;
    quint32 Rlength = length;
    quint32 remainder = m_datasize % 16;
    quint32 nTimes = (remainder == 0) ? m_datasize/(length-1) : m_datasize/length;//how many packs we should send to board.

    quint8 buf[30] = {0};//temp buffer to store the send data

    for (quint32 i = 0; i <= nTimes; i++)
	{
		qDebug("The %d times\n", i);
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
        memcpy((void*)buf, (void *)(m_data2send + i*length), Rlength);//get data from the transfer

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
