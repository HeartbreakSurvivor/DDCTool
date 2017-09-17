#pragma once
//QT
#include <QtCore>
#include <qstring.h>
#include <qfile.h>
#include <qglobal.h>
#include <qthread.h>
#include <QDataStream>

#include <Windows.h>
#include <map>
#include <vector>
#include <algorithm>

#include <iostream>
#include <iomanip>

#include "burn_data.h"


#define		EdidPorts_Num           6

#define		BIT(_bit_)              (1<<(_bit_))
#define		EDID_VGA				BIT(0)
#define 	EDID_DVI				BIT(1)
#define 	EDID_HDMI1				BIT(2)
#define 	EDID_HDMI2				BIT(3)
#define 	EDID_HDMI3				BIT(4)
#define 	EDID_DP					BIT(5)


enum Cvt_DataType
{
	DATATYPE_EDID_VGA = 0,
	DATATYPE_EDID_DVI,
	DATATYPE_EDID_HDMI1,
	DATATYPE_EDID_HDMI2,
	DATATYPE_EDID_HDMI3,
	DATATYPE_EDID_DP,
	DATATYPE_HDCPKEY,
};

typedef struct
{
	bool isErasehdcp;
	bool isBurn;
	bool isRead;
	bool isVerify;
}Cvt_flowControlPar;

typedef struct
{
    quint8* data;
    quint8 size;
}burndata_t;
