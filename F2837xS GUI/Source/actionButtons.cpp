#pragma once
#include "actionButtons.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//				ActionButtons Methods
//
///////////////////////////////////////////////////////////////////////////////


ActionButtons::ActionButtons(F28377S_Device * new_hDevice)
{
	
	mainLayout = new QGridLayout;
	hDevice = new_hDevice;

	// generate buttons and connect them with the device
	flushButton = new QPushButton(tr("Flush"));
	flushButton->setToolTip(tr("Flush all Buffers"));
	mainLayout->addWidget(flushButton, 0, 0);
	if(hDevice->isOnline()) connect(flushButton, SIGNAL(clicked()), hDevice, SLOT(fflush()));

	pingButton = new QPushButton(tr("Ping"));
	pingButton->setToolTip(tr("Ping the device to measure latency"));
	mainLayout->addWidget(pingButton, 0, 1);
	if (hDevice->isOnline()) connect(pingButton, SIGNAL(clicked()), hDevice, SLOT(ping()));


	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

ActionButtons::~ActionButtons()
{
}





