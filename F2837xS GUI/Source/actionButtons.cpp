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
	flushButton->setToolTip(tr("Clear any leftover data in the USB Buffers"));
	mainLayout->addWidget(flushButton, row, 0);
	if (hDevice->isOnline()) connect(flushButton, SIGNAL(clicked()), hDevice, SLOT(fflush()));

	pingButton = new QPushButton(tr("Ping"));
	pingButton->setToolTip(tr("Ping the device to measure latency"));
	mainLayout->addWidget(pingButton, row++, 1);
	if (hDevice->isOnline()) connect(pingButton, SIGNAL(clicked()), hDevice, SLOT(ping()));

	record_hw_button = new QPushButton(tr("Record on HW"));
	record_hw_button->setToolTip(tr("Record Data until the Buffer is full and download afterwards"));
	mainLayout->addWidget(record_hw_button, row, 0);
	if (hDevice->isOnline()) connect(record_hw_button, SIGNAL(clicked()), hDevice, SLOT(Record_HW()));

	for(int i=0; i<charts->count(); i++)	// charts only update themselves if their size changes. Record_HW creates data batches of identical size. therefore we have to call redraw manually
		if (hDevice->isOnline()) connect(record_hw_button, SIGNAL(clicked()), charts->at(i), SLOT(redraw()));


	reconnectButton = new QPushButton(tr("Reconnect"));
	record_hw_button->setToolTip(tr("Reconnect to the Device. This is required after a reset"));
	mainLayout->addWidget(reconnectButton, row++, 1);
	connect(reconnectButton, SIGNAL(clicked()), hDevice, SLOT(Reconnect()));

	systemStartStopButton = new QPushButton();
	systemStartStopButton->setToolTip(tr("Pause the System and put it in idle mode. The connection will stay online but not measurements will be taken"));
	mainLayout->addWidget(systemStartStopButton, row, 0);
	connect(systemStartStopButton, SIGNAL(clicked()), hDevice, SLOT(toggle_StartStop()));
	connect(systemStartStopButton, SIGNAL(clicked()), this, SLOT(update()));

	//streamButton = new QPushButton(tr("Stream Data");
	//systemStartStopButton->setToolTip(tr("Download all Data continously. Warning: this has a high perfomance impact!"));
	//mainLayout->addWidget(systemStartStopButton, row++, 1);
	//connect(systemStartStopButton, SIGNAL(clicked()), hDevice, SLOT(toggle_StartStop()));

	update();

	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

ActionButtons::~ActionButtons()
{
}

///////////////////////////////////////////////////////////////////////////////

void ActionButtons::update()
{
	if (hDevice->isRunning())
	{
		systemStartStopButton->setText(tr("Pause System"));
		record_hw_button->setEnabled(true);
	}
	else
	{
		systemStartStopButton->setText(tr("Start System"));
		record_hw_button->setEnabled(false);
	}
}





