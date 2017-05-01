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
	void setBuffersize(int new_size) { buffer_size = new_size; }

	int Error_WriteUSBPacket(QString msg = "Unknown error encoutnered while trying to send data to the device");
	DWORD Error_ReadUSBPacket(QString msg, DWORD err = -1);

	bool isOnline() { return bIsOnline; }
	qreal bufferLoad();

	int get_setting(int type, int * val);
	int set_setting(int type, int val);

public slots:
	double ping();					// ping the device and emasure latency
	int Debug_Data(int on_off);		// [ON/OFF] generate artificial data and ignore measured data
	int Save_Raw_Data(int on_off);	// [ON/OFF] save every measured or generated data point
	BOOL fflush();					// Tell the device to flush USB TX and RX Buffer and get rid of any residual data
	int get_all();					// download the entire data buffer to the host. Warning: this can have a very high performance impact on fast sample rates
	int Record_HW();				// Store all Data in RAM and transmit via USB when full



private:
	LMUSB_HANDLE hUSB;
	bool bIsOnline = false;
	QListWidget * messageList = new QListWidget();
	MeasureData2D * xData;

	DWORD Read_USB_MultiByteData(int32_t *, int = 1);

	int packet_size = 0;	 // neede to calculate buffer load
	int buffer_size = 0;
};

