#pragma once
#include "f28377s_device.h"

using namespace std;
using namespace chrono;


///////////////////////////////////////////////////////////////////////////////
//
//	F28377S_Device methods
//
///////////////////////////////////////////////////////////////////////////////


F28377S_Device::F28377S_Device()
{
	connect_device();
}

///////////////////////////////////////////////////////////////////////////////

F28377S_Device::~F28377S_Device()
{
	TerminateDevice(hUSB);
}

///////////////////////////////////////////////////////////////////////////////

bool F28377S_Device::connect_device()
{
	BOOL bDriver_isntalled;
	hUSB = InitializeDevice(BULK_VID, BULK_PID, (LPGUID)&(GUID_DEVINTERFACE_TIVA_BULK), &bDriver_isntalled);
	if (!hUSB)
	{
		bIsOnline = false;
		return false;
	}

	bIsOnline = true;

	update_systemStatus();

	return true;	
}

///////////////////////////////////////////////////////////////////////////////

double F28377S_Device::ping()
{
	unsigned char tx_msg[] = { COMMAND_PING };
	unsigned char rx_msg[] = { 0 };
	ULONG ulTransferred = 0;

	new QListWidgetItem( tr("Pinging with single Bytes of Data:"), messageInterface);

	for (int i = 0; i < 4; i++)
	{
		high_resolution_clock::time_point start_time = high_resolution_clock::now();

		BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, 1, &ulTransferred);
		if (!bTx_sucess) return Error_WriteUSBPacket(tr("While trying to ping the device."));

		DWORD dRx_error = ReadUSBPacket(hUSB, rx_msg, 1, &ulTransferred, 500, NULL);
		if (dRx_error != 0) return Error_ReadUSBPacket("While trying to ping the device.", dRx_error);

		if (tx_msg[0] == rx_msg[0])
		{
			high_resolution_clock::time_point end_time = high_resolution_clock::now();
			duration<double, milli> delay = duration_cast<duration<double, milli>>(end_time - start_time);
			new QListWidgetItem(tr("Replay from Controller : bytes = 1 time = %1ms").arg(delay.count()), messageInterface);
		}
		else
		{
			cout << "Unexpected reply message" << endl;
			cout << "Expected	Recieved" << endl;
			cout << "0x" << hex << COMMAND_PING << "	0x" << hex << rx_msg << dec << endl;
			return ERROR_REPLY_MESSAGE_MISMATCH;
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::Error_WriteUSBPacket(QString msg)
{
	QMessageBox::critical(this, tr("USB Write Error"), tr("Unable to write the Command to USB\n%1").arg(msg));
	return ERROR_DDE_FAIL;
}

///////////////////////////////////////////////////////////////////////////////

DWORD F28377S_Device::Error_ReadUSBPacket(QString msg, DWORD err)
{
	// format we want to construct for the error messagebox
	// Error [err] <explanation>
	// <msg>

	QString err_msg = QString("Error %1").arg(err);
	switch (err)
	{
	case WAIT_TIMEOUT:
		err_msg.append(": Operation Timeout ");
		break;

	case WAIT_OBJECT_0:
		err_msg.append(": break event was signalled ");
		break;

	default:
		err_msg.append(": An unknown Error Occured ");
	}

	err_msg.append("\n\n");
	err_msg.append(msg);

	QMessageBox::critical(this, tr("USB Read Error"), err_msg);

	return err;
}

///////////////////////////////////////////////////////////////////////////////

// returns bufferload in percent
qreal F28377S_Device::bufferLoad()
{
	return (qreal) 100* (qreal)packet_size / (qreal)buffer_size;
}

///////////////////////////////////////////////////////////////////////////////

// tell the controller to save its raw data.
int F28377S_Device::Save_Raw_Data(int option)
{
	unsigned char tx_msg[2];
	tx_msg[0] = COMMAND_SAVE_DATA;
	tx_msg[1] = option;
	ULONG ulTransferred;

	if (!(option == ON) && !(option == OFF))
		return ERROR_BAD_COMMAND;

	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, 2, &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket(tr("While trying to send a command to the device."));

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::Debug_Data(int option)
{
	unsigned char tx_msg[2];
	tx_msg[0] = COMMAND_DEBUG_DATA;
	tx_msg[1] = option;
	ULONG ulTransferred;

	if (!(option == ON) && !(option == OFF))
		return ERROR_BAD_COMMAND;

	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, 2, &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket(tr("While trying to send a command to the device."));

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::Debug_Data(bool state)
{
	if (state == true)
		return Debug_Data((int)ON);
	else
		return Debug_Data((int)OFF);
}

///////////////////////////////////////////////////////////////////////////////

BOOL F28377S_Device::fflush()
{
	ULONG ulTransferred = 0;
	unsigned char buffer[64];
	unsigned char tx_msg[1] = { COMMAND_FFLUSH };

	// send command to flush buffers
	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, 2, &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket(tr("In function fflush()"));

	// read out any rem,aining bytes
	ulTransferred = 1;
	while (ulTransferred != 0)
	{
		ReadUSBPacket(hUSB, buffer, sizeof(buffer), &ulTransferred, 50, NULL);
	}
	

	new QListWidgetItem(tr("Buffer Flushed"), messageInterface);
	return bTx_sucess;

}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::download_channel_data(int channel_num, MeasureData2D * dest)
{
	if ((channel_num != REQUEST_DATA_X) && (channel_num != REQUEST_DATA_Y) && (channel_num != REQUEST_DATA_Z))
		return -1;

	unsigned char tx_msg = channel_num;
	unsigned char header[2];
	int data_cnt = 0;
	ULONG ulTransferred = 0;
	int packet_size = 1;	// we keep this value to determine latest bufferload

	bool bLastPacket = false;
	while (bLastPacket == false)
	{
		// send reequest
		BOOL bTx_sucess = WriteUSBPacket(hUSB, &tx_msg, 1, &ulTransferred);

		// Read Header
		DWORD dRx_error = ReadUSBPacket(hUSB, header, sizeof(header), &ulTransferred, 50, NULL);
		if (dRx_error) return Error_ReadUSBPacket("While retrieving header information.", dRx_error);

		// acheck for overflow
		if ((header[0] >> 7) & 1)
		{
			//new QListWidgetItem(tr("Warning Sample Buffer Overflowdetected."), messageList);
			header[0] &= ~(1 << 7); // clear overflowflag so it does not affect data_cnt
		}

		// check for split packet
		if ((header[0] >> 6) & 1)
		{
			header[0] &= ~(1 << 6); 
		}
		else
		{
			bLastPacket = true;
		}

		// assemble packet_size
		packet_size = (header[0] << 8);
		packet_size |= header[1];

		if (packet_size == 0)
			return data_cnt;

		// read data
		int32_t * i32USBData = new int32_t[packet_size];

		Read_USB_MultiByteData(i32USBData, packet_size);
		dest->add(i32USBData, packet_size);
		data_cnt += packet_size;

		delete[] i32USBData;
	}


	return data_cnt;
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::Record_HW(bool bUseDebugData)
{
	using namespace chrono;

	ULONG ulTransferred = 0;
	unsigned char header[2];
	unsigned char tx_msg = COMMAND_RECORD_HW;
	unsigned char rx_msg = 0;

	// send command and wait for echo
	if(bUseDebugData) Debug_Data(ON);

	BOOL bTx_sucess = WriteUSBPacket(hUSB, &tx_msg, 1, &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket(tr(""));
	auto t0 = high_resolution_clock::now();

	DWORD dRx_error = ReadUSBPacket(hUSB, &rx_msg, sizeof(rx_msg), &ulTransferred, 5000, NULL);
	if (dRx_error) return Error_ReadUSBPacket("While retrieving header information.", dRx_error);
	auto t1 = high_resolution_clock::now();
	microseconds duration_us = duration_cast<microseconds>(t1 - t0);

	if (tx_msg != rx_msg)
	{
		QMessageBox::critical(this, tr("USB Read Error"), tr("Unexpected reply message '%1'").arg(tx_msg));
		return ERROR_REPLY_MESSAGE_MISMATCH;
	}

	// Download Data
	int data_cnt = 0;
	int channels[] = { REQUEST_DATA_X, REQUEST_DATA_Y, REQUEST_DATA_Z };
	for (int i = 0; i < 3; i++)
	{
		data->at(i)->clear();
		data_cnt += download_channel_data(channels[i], data->at(i));

		data->at(i)->interpolate_time(0, data->at(i)->size(), 0, (qreal)duration_us.count() / 1000);
		//data->at(i)->Remove_Offset();
	}	

	new QListWidgetItem(tr("Download complete. %1 values in %2ms recorded at %3kHz").arg(data_cnt).arg(duration_us.count()/1000).arg((double)(1000*(double)data_cnt/ (double)duration_us.count()), 0, 'f', 2), messageInterface);
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// send a single command mainly for debugging
int F28377S_Device::send_command(int command)
{
	ULONG ulTransferred = 0;
	unsigned char tx_msg[] = { (unsigned char)command };

	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, sizeof(tx_msg), &ulTransferred);

	new QListWidgetItem(tr("Sent Command 0x%1").arg(command, 0, 16), messageInterface);

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void F28377S_Device::Reconnect()
{
	TerminateDevice(hUSB);
	connect_device();

	new QListWidgetItem(tr("Device reconnected"), messageInterface);

	fflush();
	update_systemStatus();
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::toggle_StartStop()
{
	if (bIsRunning == true)
		system_pause();
	else
		system_start();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::system_pause()
{
	ULONG ulTransferred = 0;
	unsigned char tx_msg[1] = { COMMAND_SYSTEM_PAUSE };

	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, sizeof(tx_msg), &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket();

	update_systemStatus();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::system_start()
{
	ULONG ulTransferred = 0;
	unsigned char tx_msg[1] = { COMMAND_SYSTEM_CONTINUE };

	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, sizeof(tx_msg), &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket();

	update_systemStatus();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void F28377S_Device::update_systemStatus()
{
	int new_systemStatus;
	get_setting(SETTING_SYSTEM_STATUS, &new_systemStatus);

	if (new_systemStatus == 0)
		bIsRunning = true;
	else
		bIsRunning = false;
}

///////////////////////////////////////////////////////////////////////////////

int F28377S_Device::get_setting(int type, int * val)
{
	ULONG ulTransferred = 0;
	unsigned char tx_msg[2] = { SETTING_GET, (unsigned char) type };

	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, 2, &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket(tr("While requesting setting 0x%1.").arg(type, 0, 16));

	return Read_USB_MultiByteData(val, 1);
}

///////////////////////////////////////////////////////////////////////////////

// Tell the device to change the register for <type> to <val>
// after transmission the device will echo back the contents of the changed register so we can check against an integer overflow
int F28377S_Device::set_setting(int type, int val)
{
	ULONG ulTransferred = 0;
	unsigned char tx_msg[4] = { SETTING_SET, (unsigned char)type, (uint8_t)(val >> 8), (uint8_t)val };	// since we are uploadint up to 16bit data we need 2 additional messages-slots
	int32_t rx_msg[1] = { 0 };

	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, 4, &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket(tr("While requesting setting 0x%1.").arg(type, 0, 16));

	Read_USB_MultiByteData(rx_msg);
	if ((int32_t)val != rx_msg[0])
	{
		new QListWidgetItem( tr("Warning: Replymessage-Mismatch for Item 0x%1").arg(type, 0, 16), messageInterface );
		new QListWidgetItem( tr("Expected: %1 - Recieved: %2").arg(val).arg(rx_msg[1]), messageInterface);
		return ERROR_REPLY_MESSAGE_MISMATCH;
	}

	new QListWidgetItem(tr("Set Item %1 to %2 ").arg(type, 0 ,16).arg(val), messageInterface);
	return 0;		
}

///////////////////////////////////////////////////////////////////////////////

// read 32bit data array from device and assemble it accordingly
DWORD F28377S_Device::Read_USB_MultiByteData(int32_t * rx_data, int rx_data_cnt)
{
	ULONG ulTransferred = 0;
	static unsigned char buffer[512];

	DWORD dRx_error = ReadUSBPacket(hUSB, buffer, (sizeof(unsigned char)* 4 * rx_data_cnt) , &ulTransferred, 500, NULL);
	//if (dRx_error) return Error_ReadUSBPacket(tr("While reading Multibyte-Data."), dRx_error);

	// assemble data
	for (ULONG i = 0; i < rx_data_cnt; i++)
	{
		rx_data[i] = 0;
		for (ULONG j = 0; j < 4; j++)
		{
			rx_data[i] |= (uint32_t)(buffer[4 * i + (3 - j)] << (8 * j));
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////


