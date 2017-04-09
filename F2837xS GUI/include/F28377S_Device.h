#pragma once
#include <iostream>
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

	bool Get_USB_Device();

	bool IsOnline() { return isOnline; }
	double ping();

	int Error_WriteUSBPacket(QString msg = "Unknown error encoutnered while trying to send data to the device");

	DWORD Error_ReadUSBPacket(QString msg, DWORD err = -1);

private:
	LMUSB_HANDLE hUSB;
	bool isOnline = false;
	QListWidget * messageList = new QListWidget();
};

