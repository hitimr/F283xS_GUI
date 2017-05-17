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

	bool connect_device();
	void setData(QVector<MeasureData2D *> * new_data) { data = new_data; }
	void setMessageInterface(QListWidget * new_messageInterface) { messageInterface = new_messageInterface;  }
	void setBuffersize(int new_size) { buffer_size = new_size; }

	int Error_WriteUSBPacket(QString msg = "Unknown error encoutnered while trying to send data to the device");
	DWORD Error_ReadUSBPacket(QString msg, DWORD err = -1);

	bool isOnline() { return bIsOnline; }
	bool isRunning() { return bIsRunning;  }
	qreal bufferLoad();

	int get_setting(int type, int * val);
	int set_setting(int type, int val);

public slots:
	double	ping();									// ping the device and emasure latency
	int		Debug_Data(int on_off);					// [ON/OFF] generate artificial data and ignore measured data
	int		Debug_Data(bool state);
	int		Save_Raw_Data(int on_off);				// [ON/OFF] save every measured or generated data point
	BOOL	fflush();								// Tell the device to flush USB TX and RX Buffer and get rid of any residual data
	int		download_channel_data(int channel_num, MeasureData2D * dest);
	int		Record_HW(bool bUseDebugData = false);	// Store all Data in RAM and transmit via USB when full
	int		send_command(int command);
	void	Reconnect();
	int		toggle_StartStop();
	int		system_pause();
	int		system_start();
	void	update_systemStatus();



private:
	LMUSB_HANDLE hUSB;
	bool bIsOnline = false;
	bool bIsRunning = false;
	QListWidget * messageInterface = new QListWidget();
	QVector<MeasureData2D *> * data;

	DWORD Read_USB_MultiByteData(int32_t *, int = 1);

	int packet_size = 0;	 // neede to calculate buffer load
	int buffer_size = 0;
};

