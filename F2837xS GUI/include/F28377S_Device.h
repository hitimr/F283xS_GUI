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

	bool IsOnline() { return isOnline; }
	double ping();
	int Save_Raw_Data(int mode);

	int Debug_Data(int option);

	void fflush();

	BOOL get_all();

	

private:
	LMUSB_HANDLE hUSB;
	bool isOnline = false;
	QListWidget * messageList = new QListWidget();
	MeasureData2D * xData;

	ULONG Read_USB_MultiByteData(int32_t * rx_data);
};

