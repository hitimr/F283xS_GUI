#pragma once
#include <windows.h>
#include <initguid.h>
#include "tiva_guids.h"
#include "lmusbdll.h"



///////////////////////////////////////////////////////////////////////////////
//
//	F28377S_Device provides an interface for communication with a F28377S device
//
///////////////////////////////////////////////////////////////////////////////


class F28377S_Device 
{
public:
	F28377S_Device();
	~F28377S_Device();

	bool Get_USB_Device();

	bool IsOnline() { return isOnline; }

private:
	LMUSB_HANDLE hUSB;
	bool isOnline = false;
};

