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
	Connect();
}

///////////////////////////////////////////////////////////////////////////////

F28377S_Device::~F28377S_Device()
{
	TerminateDevice(hUSB);
}

///////////////////////////////////////////////////////////////////////////////

bool F28377S_Device::Connect()
{
	BOOL bDriver_isntalled;
	hUSB = InitializeDevice(BULK_VID, BULK_PID, (LPGUID)&(GUID_DEVINTERFACE_TIVA_BULK), &bDriver_isntalled);
	if (!hUSB)
	{
		bIsOnline = false;
		return false;
	}

	bIsOnline = true;
	return true;	
}

///////////////////////////////////////////////////////////////////////////////

double F28377S_Device::ping()
{
	unsigned char tx_msg[] = { COMMAND_PING };
	unsigned char rx_msg[] = { 0 };
	ULONG ulTransferred = 0;

	new QListWidgetItem( tr("Pinging with single Bytes of Data:"), messageList);

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
			new QListWidgetItem(tr("Replay from Controller : bytes = 1 time = %1ms").arg(delay.count()), messageList);
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
	return (qreal) 100* (qreal)recent_xmit_length / (qreal)buffer_size;
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

BOOL F28377S_Device::fflush()
{
	ULONG ulTransferred = 0;
	unsigned char tx_msg[1] = { COMMAND_FFLUSH };
	BOOL bTx_sucess = WriteUSBPacket(hUSB, tx_msg, 2, &ulTransferred);
	if (!bTx_sucess) return Error_WriteUSBPacket( tr("In function fflush()") );
	new QListWidgetItem(tr("Buffer Flushed"), messageList);

}

///////////////////////////////////////////////////////////////////////////////

BOOL F28377S_Device::get_all()
{
	int32_t i32USBData[512];
	unsigned char header[2];
	ULONG ulTransferred = 0;
	recent_xmit_length = 0;	// we keep this value to determine latest bufferload
	unsigned char tx_msg = REQUEST_ALL_DATA;


	// send reequest
	BOOL bTx_sucess = WriteUSBPacket(hUSB, &tx_msg, 1, &ulTransferred);

	// Read Header
	DWORD dRx_error = ReadUSBPacket(hUSB, header, sizeof(header), &ulTransferred, 50, NULL);
	if (dRx_error) return Error_ReadUSBPacket("While retrieving header information.", dRx_error);

	if ((header[0] >> 7) & 1)
	{
		new QListWidgetItem(tr("Warning Sample Buffer Overflowdetected."), messageList);
		header[0] &= ~(1 << 7); // clear overflowflag so it does not affect data_cnt
	}

	// assemble header value
	recent_xmit_length |= (header[0] << 7);
	recent_xmit_length |=  header[1];

	// read actual data
	Read_USB_MultiByteData(i32USBData, recent_xmit_length);

	xData->add(i32USBData, recent_xmit_length);


	return true;
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
		new QListWidgetItem( tr("Warning: Replymessage-Mismatch for Item 0x%1").arg(type, 0, 16), messageList );
		new QListWidgetItem( tr("Expected: %1 - Recieved: %2").arg(val).arg(rx_msg[1]), messageList);
		return ERROR_REPLY_MESSAGE_MISMATCH;
	}

	new QListWidgetItem(tr("Set Item %1 to %2 ").arg(type).arg(val), messageList);
	return 0;		
}

///////////////////////////////////////////////////////////////////////////////

// read 32bit data array from device and assemble it accordingly
DWORD F28377S_Device::Read_USB_MultiByteData(int32_t * rx_data, int rx_data_cnt)
{
	ULONG ulTransferred = 0;
	static unsigned char buffer[512];

	DWORD dRx_error = ReadUSBPacket(hUSB, buffer, (sizeof(unsigned char)* 4 * rx_data_cnt) , &ulTransferred, 50, NULL);
	if (dRx_error) return Error_ReadUSBPacket(tr("While reading Multibyte-Data."), dRx_error);

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


