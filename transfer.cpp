#include "transfer.h"

using std::cout;
using std::endl;

namespace ddc {

#define		PERPACK_LEN			16

void Transfer_T::setburnCmd(burnCmd_t *burnmsg)
{
    m_burnmsg = burnmsg;
}

void Transfer_T::setburnCmd(burnCmd_t *burnmsg,quint8 *data, quint32 size)
{
    m_burnmsg = burnmsg;
    m_databody = data;
    m_bodysize = size;
}

void Transfer_T::setburndata(quint8 *data, quint32 size)
{
    m_databody = data;
    m_bodysize = size;
}

bool Transfer_T::transfermultipackage(void)
{
    quint32 Rlength , length = PERPACK_LEN;
    quint32 remainder = m_bodysize % 16;
    quint32 nTimes = (remainder == 0) ? m_bodysize/(length-1) : m_bodysize/length;//how many packs we should send to board.
    quint32 writedelay = m_burnmsg->delay;
    burndata_t tmpdata;
    quint8* feedback = new quint8[m_burnmsg->feedbacklen];

    for (quint32 i = 0; i <= nTimes; i++)
    {
        qDebug("The %d times\n", i);
        if (i == nTimes)
        {
            qDebug("The last times delay longer\n");
            writedelay = m_burnmsg->lastpackdelay;
            if (remainder != 0)
            {
                Rlength = remainder;
            }
        }
        m_burnmsg->burndata[3] = i;
        m_burnmsg->burndata[4] = Rlength;

        for (int j = 0; j < m_spretrycnt; j++)//The single pack retry mechanism.
        {
            if(m_burnmsg->assemblefunc)
            {
                tmpdata = m_burnmsg->assemblefunc(m_burnmsg->burndata,m_burnmsg->datalen,m_databody+i*length,Rlength);
                m_protocol.write(tmpdata.data,tmpdata.size);
                if(tmpdata.data!=m_burnmsg->burndata)
                    delete tmpdata.data;
            }

            Sleep(writedelay);

            m_protocol.read(feedback,m_burnmsg->feedbacklen);

            if(m_burnmsg->verifyfunc(feedback,m_burnmsg->feedbacklen,m_databody,m_bodysize))
            {
                if (i == nTimes)
                {
                   qDebug("The last times and write successfully!\n");
                   delete feedback;
                   return true;
                }
                break;
            }
            else
            {
                if (j = m_spretrycnt - 1)
                {
                    qDebug("the last signel package retry chance!");
                    delete feedback;
                    return false;
                }
                continue;
            }
        }
    }
    delete feedback;
    return true;
}

bool Transfer_T::transferpackage()
{
    if(m_bodysize>PERPACK_LEN)//multi package
    {
        return transfermultipackage();
    }
    else //only send one package
    {
        burndata_t tmpdata;
        quint8* feedback = new quint8[m_burnmsg->feedbacklen];

        if(m_burnmsg->assemblefunc)
        {
            tmpdata = m_burnmsg->assemblefunc(m_burnmsg->burndata,m_burnmsg->datalen,m_databody,m_bodysize);
            m_protocol.write(tmpdata.data,tmpdata.size);
            if(tmpdata.data!=m_burnmsg->burndata)
                delete tmpdata.data;
        }
        else
        {
            m_protocol.write(m_burnmsg->burndata,m_burnmsg->datalen);//such as reset instruction.
            std::cout<<" send data:"<<std::endl;
            for(int i=0;i<m_burnmsg->datalen;++i)
                qDebug(" 0x%x",m_burnmsg->burndata[i]);
        }

        Sleep(m_burnmsg->delay);

        m_protocol.read(feedback,m_burnmsg->feedbacklen);

        cout<<endl;
        cout<<"read data:"<<endl;
        for(int i=0;i<m_burnmsg->feedbacklen;++i)
            cout<<" "<<feedback[i];

        if(m_burnmsg->verifyfunc)
        {
            if(m_burnmsg->verifyfunc(feedback,m_burnmsg->feedbacklen,m_databody,m_bodysize))
            {
                delete feedback;
                return true;
            }
            else
            {
                delete feedback;
                return false;
            }
        }
    }
    return true;
}

void Transfer_T::run()
{
    qDebug()<<"data transfer thread start!";
    for (int k = 0; k < 1/*m_burnmsg->retrycnt*/; k++)
    {
        if (transferpackage())
        {
            qDebug("Burning Pass and ready to read !");
            break;
        }
        else
        {
            qDebug("Burning Failed,Retry Times:%d!", k);
            if (k == m_burnmsg->retrycnt-1)//the last chance
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
