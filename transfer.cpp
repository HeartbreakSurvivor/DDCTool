#include "transfer.h"

using namespace std;
using std::cout;
using std::endl;

namespace ddc {

#define		PERPACK_LEN			16

void Transfer_T::setburnCmd(burnCmd_t *burnmsg,quint8 source)
{
    m_burnmsg = burnmsg;
    m_source = source;
}

void Transfer_T::setburnCmd(burnCmd_t *burnmsg,quint8 *data, quint32 size,quint8 source)
{
    m_burnmsg = burnmsg;
    m_databody = data;
    m_bodysize = size;
    m_source = source;
}

void Transfer_T::setburndata(quint8 *data, quint32 size)
{
    m_databody = data;
    m_bodysize = size;
}

bool Transfer_T::transfermultipackage(void)
{
    quint32 Rlength = PERPACK_LEN , length = PERPACK_LEN;
    quint32 remainder = m_bodysize % 16;
    quint32 nTimes = (remainder == 0) ? m_bodysize/(length)-1 : m_bodysize/length;//how many packs we should send to board.
    quint32 writedelay = m_burnmsg->delay;
    burndata_t tmpdata;
    quint8* feedback = new quint8[m_burnmsg->feedbacklen];

    for (quint32 i = 0; i <= nTimes; i++)
    {
        qDebug()<<"";
        qDebug("The %d time", i);
        if (i == nTimes)
        {
            qDebug("The last time and sleep longer");
            writedelay = m_burnmsg->lastpackdelay;
            if (remainder != 0)
            {
                Rlength = remainder;
            }
        }
        m_burnmsg->burndata[3] = i;//update the package offset dynamically.
        m_burnmsg->burndata[4] = Rlength;

        for (int j = 0; j < m_spretrycnt; j++)//The single pack retry mechanism.
        {
            if(m_burnmsg->assemblefunc)
            {
                tmpdata = m_burnmsg->assemblefunc(m_burnmsg->burndata,m_burnmsg->datalen,m_databody+i*length,Rlength);
                //qDebug("assembly data size:%d",tmpdata.size);
                m_protocol.write(tmpdata.data,tmpdata.size,m_source);
                if(tmpdata.data!=m_burnmsg->burndata)
                {
                    //qDebug("delete the burndata");
                    delete[] tmpdata.data;//for the object array,recall the destructor one by one.
                }
            }

            qDebug("Sleep %d millisecond",writedelay);
            Sleep(writedelay);

            m_protocol.read(feedback,m_burnmsg->feedbacklen,m_source);

            //if(m_burnmsg->verifyfunc(feedback,m_burnmsg->feedbacklen,m_databody,m_bodysize))
            if(1)
            {
                if (i == nTimes)
                {
                   qDebug("The last time and burn successfully!");
                   emit transfer_res(m_burnmsg->name,Rlength);//burn failed
                   delete feedback;
                   return true;
                }
                break;
            }
            else
            {
                if (j = m_spretrycnt - 1)
                {
                    qDebug("the last retry chance!");
                    delete feedback;
                    return false;
                }
                continue;
            }
        }
        //qDebug("Rlength: %d",Rlength);
        emit transfer_res(m_burnmsg->name,Rlength);//burn failed
    }
    delete feedback;
    return true;
}

bool Transfer_T::transferpackage()
{
    //qDebug()<<"bodysize:"<<m_bodysize;
    //qDebug()<<"source:"<<m_source;
    if(m_bodysize>PERPACK_LEN)//multi package
    {
        return transfermultipackage();
    }
    else //only send one package
    {
        qDebug()<<"single package";
        burndata_t tmpdata;
        quint8* feedback = new quint8[m_burnmsg->feedbacklen];

        if(m_burnmsg->assemblefunc)
        {
            tmpdata = m_burnmsg->assemblefunc(m_burnmsg->burndata,m_burnmsg->datalen,m_databody,m_bodysize);
            m_protocol.write(tmpdata.data,tmpdata.size,m_source);
            if(tmpdata.data!=m_burnmsg->burndata)
                delete tmpdata.data;
        }
        else
        {
            m_protocol.write(m_burnmsg->burndata,m_burnmsg->datalen,m_source);//such as reset instruction.
        }

        Sleep(m_burnmsg->delay);

        m_protocol.read(feedback,m_burnmsg->feedbacklen,m_source);

        if(0)
        //if(m_burnmsg->verifyfunc)
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
    for (int k = 0; k < m_burnmsg->retrycnt; k++)
    {
        if (transferpackage())
        {
            qDebug("Data Write Successfully!");
            emit transfer_res(m_burnmsg->name,-1);//burn success and return
            break;
        }
        else
        {
            qDebug("Write Failed,Retry Times:%d!", k);
            if (k == m_burnmsg->retrycnt-1)//the last chance
            {
                qDebug("The last retry chance but failed, -----Burning Failed!");
                emit transfer_res(m_burnmsg->name,-2);//burn failed
                return;
            }
        }
    }
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
