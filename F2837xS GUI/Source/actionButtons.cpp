#pragma once
#include "actionButtons.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//				ActionButtons Methods
//
///////////////////////////////////////////////////////////////////////////////


ActionButtons::ActionButtons(F28377S_Device * new_hDevice, Settings_ui * new_settings, QVector<InteractiveChart *> * new_charts)
{

	mainLayout = new QGridLayout;
	hDevice = new_hDevice;
	settings = new_settings;
	charts = new_charts;
	int row = 0;

	downloadButton = new QPushButton(tr("Download Settings"));
	downloadButton->setToolTip(tr("Download current settings for the controller"));
	mainLayout->addWidget(downloadButton, row, 0);
	if (hDevice->isOnline()) connect(downloadButton, SIGNAL(clicked()), settings, SLOT(update()));

	uploadButton = new QPushButton(tr("Upload Settings"));
	uploadButton->setToolTip(tr("Download current settings for the controller"));
	mainLayout->addWidget(uploadButton, row++, 1);
	if (hDevice->isOnline()) connect(uploadButton, SIGNAL(clicked()), settings, SLOT(upload()));

	flushButton = new QPushButton(tr("Flush Buffers"));
	flushButton->setToolTip(tr("Flush all Buffers"));
	mainLayout->addWidget(flushButton, row, 0);
	if (hDevice->isOnline()) connect(flushButton, SIGNAL(clicked()), hDevice, SLOT(fflush()));

	pingButton = new QPushButton(tr("Ping"));
	pingButton->setToolTip(tr("Ping the device to measure latency"));
	mainLayout->addWidget(pingButton, row++, 1);
	if (hDevice->isOnline()) connect(pingButton, SIGNAL(clicked()), hDevice, SLOT(ping()));

	burstButton = new QPushButton(tr("Record on HW"));
	burstButton->setToolTip(tr("Record Data until the Buffer is full"));
	mainLayout->addWidget(burstButton, row, 0);
	if (hDevice->isOnline()) connect(burstButton, SIGNAL(clicked()), hDevice, SLOT(Record_HW()));

	for(int i=0; i<charts->count(); i++)	// charts only update themselves if their size changes. Record_HW creates data batches of identical size. therefore we have to call redraw manually
		if (hDevice->isOnline()) connect(burstButton, SIGNAL(clicked()), charts->at(i), SLOT(redraw()));

	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

ActionButtons::~ActionButtons()
{
}





