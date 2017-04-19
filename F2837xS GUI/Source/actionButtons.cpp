#pragma once
#include "actionButtons.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//				ActionButtons Methods
//
///////////////////////////////////////////////////////////////////////////////


ActionButtons::ActionButtons(F28377S_Device * new_hDevice, Settings_ui * new_settings)
{
	
	mainLayout = new QGridLayout;
	hDevice = new_hDevice;
	settings = new_settings;
	int row = 0;

	downloadButton = new QPushButton(tr("Download Settings"));
	downloadButton->setToolTip(tr("Download current settings for the controller"));
	mainLayout->addWidget(downloadButton, row, 0);
	if (hDevice->isOnline()) connect(downloadButton, SIGNAL(clicked()), settings, SLOT(update()));

	uploadButton = new QPushButton(tr("Upload Settings"));
	uploadButton->setToolTip(tr("Download current settings for the controller"));
	mainLayout->addWidget(uploadButton, row++, 1);
	if (hDevice->isOnline()) connect(uploadButton, SIGNAL(clicked()), settings, SLOT(upload()));

	flushButton = new QPushButton(tr("Flush"));
	flushButton->setToolTip(tr("Flush all Buffers"));
	mainLayout->addWidget(flushButton, row, 0);
	if(hDevice->isOnline()) connect(flushButton, SIGNAL(clicked()), hDevice, SLOT(fflush()));

	pingButton = new QPushButton(tr("Ping"));
	pingButton->setToolTip(tr("Ping the device to measure latency"));
	mainLayout->addWidget(pingButton, row++, 1);
	if (hDevice->isOnline()) connect(pingButton, SIGNAL(clicked()), hDevice, SLOT(ping()));


	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

ActionButtons::~ActionButtons()
{
}





