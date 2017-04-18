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
	dynamicSettingsLayout = new QGridLayout;
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
	dynamic_settings_vec.push_back(new DynamicSetting(hDevice, tr("Global Clock Divider"),	SETTING_CLK_DIV,		1, 125, 1));
	dynamic_settings_vec.push_back(new DynamicSetting(hDevice, tr("SPI fast baud rate"),	SETTING_SPI_FAST_BRR,	1, 125, 1));
	dynamic_settings_vec.push_back(new DynamicSetting(hDevice, tr("Conversion Time [ns]"),	SETTING_CNV_PERIOD,		80,500, 10));
	dynamic_settings_vec.push_back(new DynamicSetting(hDevice, tr("Conversion Multiplier"),	SETTING_CNV_MULT,		1, 3,	1));
	dynamic_settings_vec.push_back(new DynamicSetting(hDevice, tr("Number of Conversions"),	SETTING_CNV_NUM,		10, 200, 1));

	
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("Sample Buffer Size"),	SETTING_SMPL_BUF_SIZE,	NUMERICAL));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("USB Buffer Size"),		SETTING_USB_BUF_SIZE,	NUMERICAL));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("Flash-Mode"),			SETTING_FLASH_MODE,		BOOLEAN));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("Debug-Mode"),			SETTING_DEBUG_MODE,		BOOLEAN));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("SPI slow baud rate"),	SETTING_SPI_SLOW_BRR,	NUMERICAL));
	static_settings_vec.push_back(new StaticIntSetting(hDevice, tr("Transmission Period"),	SETTING_XMIT_PERIOD,	NUMERICAL));
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
	dynamicSettingsLayout = new QGridLayout();

	int row = 0;
	int column = 0;
	for (int i = 0; i < dynamic_settings_vec.size(); i++)
	{
		dynamicSettingsLayout->addWidget(dynamic_settings_vec[i], row, column++);

		if (column >= maxColumns)
		{
			column = 0;
			row++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

void Settings_ui::update()
{
	for (int i = 0; i < static_settings_vec.size(); i++)
	{
		static_settings_vec[i]->update();
	}
	for (int i = 0; i < dynamic_settings_vec.size(); i++)
	{
		dynamic_settings_vec[i]->update();
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
		staticSettingsLayout->addWidget(static_settings_vec[i], row, column++);

		if (column >= maxColumns)
		{
			column = 0;
			row++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//				DynamicSetting Methods
//
///////////////////////////////////////////////////////////////////////////////

DynamicSetting::DynamicSetting(F28377S_Device * new_hDevice, QString new_name, int command, int min, int max, qreal new_multiplier)
{
	mainLayout = new QHBoxLayout();
	label = new QLabel();
	spinBox = new QDoubleSpinBox();

	hDevice = new_hDevice;
	label->setText(new_name);
	usb_command = command;
	spinBox->setMinimum(min);
	spinBox->setMaximum(max);
	multiplier = new_multiplier;

	spinBox->setValue(i32Value*multiplier);

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

void DynamicSetting::update()
{
	hDevice->get_setting(usb_command, &i32Value);
	spinBox->setValue(i32Value*multiplier);
}

///////////////////////////////////////////////////////////////////////////////

StaticIntSetting::StaticIntSetting(F28377S_Device * new_hDevice, QString new_name, int command, int type)
{
	Name = new_name;
	usb_command = command;
	Value = 0;
	hDevice = new_hDevice;

	if (type == BOOLEAN)
		isBoolean = true;
	else
		isBoolean = false;

	setLabelText();
}

///////////////////////////////////////////////////////////////////////////////

StaticIntSetting::~StaticIntSetting()
{
}

///////////////////////////////////////////////////////////////////////////////

void StaticIntSetting::update()
{
	hDevice->get_setting(usb_command, &Value);
	setLabelText();
}

///////////////////////////////////////////////////////////////////////////////

void StaticIntSetting::setLabelText()
{
	if (isBoolean == true)
	{
		if (Value == OFF)
			setText(QString("%1: OFF").arg(Name));
		else
			setText(QString("%1: ON").arg(Name));
	}
	else
	{
		setText(QString("%1: %2").arg(Name).arg(Value));
	}
}
