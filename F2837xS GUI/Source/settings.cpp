#pragma once
#include "settings.h"

///////////////////////////////////////////////////////////////////////////////
//
//				Settings Methods
//
///////////////////////////////////////////////////////////////////////////////

Settings_ui::Settings_ui(F28377S_Device * new_hDevice)
{
	// generate object containers
	mainLayout = new QVBoxLayout;
	hDevice = new_hDevice;
	buttonyLayout = new QHBoxLayout;
	dynamicSettingsLayout = new QVBoxLayout;
	staticSettingsLayout = new QGridLayout;

	initialize();	// populate vectos

	generate_topButtons();	// derive GUI elements
	generate_dynamicLayout();
	generate_staticLayout();

	//display items
	mainLayout->addLayout(buttonyLayout);
	insert_seperator();
	mainLayout->addLayout(dynamicSettingsLayout);
	insert_seperator();
	mainLayout->addLayout(staticSettingsLayout);
	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

Settings_ui::~Settings_ui()
{

}

///////////////////////////////////////////////////////////////////////////////

// populate both vectors
void Settings_ui::initialize()
{
	//dynamic_settings.push_back(StaticSetting(tr("Clock-Scale"), 1, true));

	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("USB Buffer Size"),		SETTING_USB_BUF_SIZE));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("Sample Buffer Size"),	SETTING_SMPL_BUF_SIZE));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("Flash-Mode"),			SETTING_FLASH_MODE));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("Debug-Mode"),			SETTING_DEBUG_MODE));
}


///////////////////////////////////////////////////////////////////////////////

void Settings_ui::generate_topButtons()
{
	loadFromFileButton = new QPushButton(tr("From File"));
	downloadButton = new QPushButton(tr("Download"));
	uploadButton = new QPushButton(tr("Upload"));

	buttonyLayout->addWidget(loadFromFileButton);
	buttonyLayout->addWidget(downloadButton);
	buttonyLayout->addWidget(uploadButton);
}

///////////////////////////////////////////////////////////////////////////////

void Settings_ui::generate_dynamicLayout()
{

	// create form
	sampleRate = new DynamicSetting(tr("Sample Rate [Hz]"), 0, 5000);
	averageingRate = new DynamicSetting(tr("Averaging Results"), 0 , 1000);

	dynamicSettingsLayout->addWidget(sampleRate);
	dynamicSettingsLayout->addWidget(averageingRate);
}

///////////////////////////////////////////////////////////////////////////////

void Settings_ui::update()
{
	for (int i = 0; i < static_settings_vec.size(); i++)
	{
		static_settings_vec[i]->update();
	}
}

///////////////////////////////////////////////////////////////////////////////

// convenience function that just generates a line and adds it to the main layout
void Settings_ui::insert_seperator()
{
	// insert a line on top
	QFrame * new_line = new QFrame;
	new_line->setFrameShape(QFrame::HLine);
	new_line->setFrameShadow(QFrame::Sunken);
	mainLayout->addWidget(new_line);
}

///////////////////////////////////////////////////////////////////////////////

void Settings_ui::generate_staticLayout()
{
	staticSettingsLayout = new QGridLayout();

	int row = 0;
	int column = 0;
	for (int i = 0; i < static_settings_vec.size(); i++)
	{
		staticSettingsLayout->addWidget(static_settings_vec[i], column++, row);

		if (column >= maxColumns)
		{
			column = 0;
			row++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//				Labeled Input Methods
//
///////////////////////////////////////////////////////////////////////////////

DynamicSetting::DynamicSetting(QString label_name, int minimum, int maximum )
{
	mainLayout = new QHBoxLayout();

	spinBox = new QSpinBox();
	label = new QLabel(label_name);

	spinBox->setMinimum(minimum);
	spinBox->setMaximum(maximum);

	mainLayout->addWidget(label);
	mainLayout->addWidget(spinBox);

	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

DynamicSetting::~DynamicSetting()
{

}

///////////////////////////////////////////////////////////////////////////////

void DynamicSetting::downloadAll()
{

}

///////////////////////////////////////////////////////////////////////////////

StaticIntSetting::StaticIntSetting(F28377S_Device * new_hDevice, QString new_name, int command, int new_value)
{
	Name = new_name;
	usb_command = command;
	Value = new_value;
	hDevice = new_hDevice;

	setText(QString("%1: %2").arg(new_name).arg(Value));
}

///////////////////////////////////////////////////////////////////////////////

StaticIntSetting::~StaticIntSetting()
{
}

///////////////////////////////////////////////////////////////////////////////

void StaticIntSetting::update()
{
	hDevice->get_setting(usb_command, &Value);
	setText(QString("%1: %2").arg(Name).arg(Value));
}
