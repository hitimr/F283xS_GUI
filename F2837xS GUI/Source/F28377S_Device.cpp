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
		isOnline = false;
		return false;
	}

	isOnline = true;
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
	QMessageBox::critical(this, tr("USB Write Error"), tr("Unable to open File"));
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

	err_msg.append("/n/n");
	err_msg.append(msg);

	QMessageBox::critical(this, tr("USB Read Error"), err_msg);

	return err;
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

void F28377S_Device::fflush()
{

}

///////////////////////////////////////////////////////////////////////////////

BOOL F28377S_Device::get_all()
{
	int32_t i32USBData[512];
	ULONG ulTransferred = 0;
	unsigned char tx_msg = REQUEST_ALL_DATA;

	Save_Raw_Data(ON);

	BOOL bTx_sucess = WriteUSBPacket(hUSB, &tx_msg, 1, &ulTransferred);
	std::this_thread::sleep_for(5ms);

	Read_USB_MultiByteData(i32USBData);

	xData->add(i32USBData, ulTransferred);


	Save_Raw_Data(OFF);
	Save_Raw_Data(OFF);
	return true;
}

///////////////////////////////////////////////////////////////////////////////

ULONG F28377S_Device::Read_USB_MultiByteData(int32_t * rx_data)
{
	ULONG ulTransferred = 0;
	DWORD dRx_error;
	static unsigned char buffer[512];

	dRx_error = ReadUSBPacket(hUSB, buffer, sizeof(buffer), &ulTransferred, 50, NULL);
	if (dRx_error == WAIT_TIMEOUT)
	{
		return 0;
	}
	else if (dRx_error != 0) return Error_ReadUSBPacket("While trying to communicate with the device.", dRx_error);

	// assemble data
	ulTransferred /= 4;
	for (ULONG i = 0; i < ulTransferred; i++)
	{
		rx_data[i] = 0;
		for (ULONG j = 0; j < 4; j++)
		{
			rx_data[i] |= (uint32_t)(buffer[4 * i + (3 - j)] << (8 * j));
		}
	}
	return ulTransferred;
}


