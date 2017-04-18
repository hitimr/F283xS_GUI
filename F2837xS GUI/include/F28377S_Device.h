#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <QObject>
#include <QListWidget>
#include <QString>
#include <QTranslator>
#include <QMessagebox>
#include <windows.h>
#include <initguid.h>
#include "tiva_guids.h"
#include "lmusbdll.h"
#include "usb_commands.h"
#include "data.h"
//#include "settings.h"



///////////////////////////////////////////////////////////////////////////////
//
//	F28377S_Device provides an interface for communication with a F28377S device
//
///////////////////////////////////////////////////////////////////////////////


class F28377S_Device : public QWidget
{
	Q_OBJECT
public:
	F28377S_Device();
	~F28377S_Device();

	bool Connect();
	void setXData(MeasureData2D * new_data) { xData = new_data; }
	void setMessageList(QListWidget * new_messageList) { messageList = new_messageList;  }

	int Error_WriteUSBPacket(QString msg = "Unknown error encoutnered while trying to send data to the device");
	DWORD Error_ReadUSBPacket(QString msg, DWORD err = -1);

	bool isOnline() { return bIsOnline; }




public slots:
	double ping();
	int Debug_Data(int option);
	int Save_Raw_Data(int mode);
	void fflush();	
	BOOL get_all();
	int get_setting(int type, int * val);


private:
	LMUSB_HANDLE hUSB;
	bool bIsOnline = false;
	QListWidget * messageList = new QListWidget();
	MeasureData2D * xData;

	DWORD Read_USB_MultiByteData(int32_t *, int);
};

