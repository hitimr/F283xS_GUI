#pragma once
#include "f28377s_device.h"



F28377S_Device::F28377S_Device()
{
	//hUSB = Get_USB_Device();
}


F28377S_Device::~F28377S_Device()
{
}


bool F28377S_Device::Get_USB_Device()
{
	BOOL bDriver_isntalled;

	hUSB = InitializeDevice(BULK_VID, BULK_PID, (LPGUID)&(GUID_DEVINTERFACE_TIVA_BULK), &bDriver_isntalled);
	if (1)
	{
		//QMessageBox::critical(this, tr("Error"), tr("Error. Device could not be located. Please check the connection"));
		return false;
	}

	isOnline = true;
	return true;	
}
