#pragma once
#include "settings.h"

///////////////////////////////////////////////////////////////////////////////
//
//				Settings Methods
//
///////////////////////////////////////////////////////////////////////////////

Settings_ui::Settings_ui()
{
	// generate object containers
	mainLayout = new QVBoxLayout;
	buttonyLayout = new QHBoxLayout;
	dynamicSettingsLayout = new QVBoxLayout;

	initialize();
	generate_topButtons();
	generate_dynamicLayout();
	generate_staticLayout();


	//diosplay items
	mainLayout->addLayout(buttonyLayout);
	mainLayout->addLayout(staticSettingsLayout);
	mainLayout->addLayout(dynamicSettingsLayout);
	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

Settings_ui::~Settings_ui()
{

}

///////////////////////////////////////////////////////////////////////////////

void Settings_ui::initialize()
{
	//dynamic_settings.push_back(StaticSetting(tr("Clock-Scale"), 1, true));
	
	static_settings.push_back(new StaticSetting(tr("Sample Buffer Size"), 1, false));
	static_settings.push_back(new StaticSetting(tr("USB Buffer Size"),	1, false));
	static_settings.push_back(new StaticSetting(tr("USB Buffer Size"), 1, false));
}


///////////////////////////////////////////////////////////////////////////////

// convenience function to generate Buttons
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
	// insert a line on top
	separator1 = new QFrame();
	separator1->setFrameShape(QFrame::HLine);
	separator1->setFrameShadow(QFrame::Sunken);
	dynamicSettingsLayout->addWidget(separator1);

	// create form
	sampleRate = new labeled_spinBox(tr("Sample Rate [Hz]"), 0, 5000);
	averageingRate = new labeled_spinBox(tr("Averaging Results"), 0 , 1000);

	dynamicSettingsLayout->addWidget(sampleRate);
	dynamicSettingsLayout->addWidget(averageingRate);
}

void Settings_ui::generate_staticLayout()
{
	staticSettingsLayout = new QGridLayout();

	for (int j = 0; j < static_settings.size()/ staticColumns; j++)
	{
		for (int i = 0; i < staticColumns; i++)
		{
			staticSettingsLayout->addWidget(static_settings[j], i, j);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//				Labeled Input Methods
//
///////////////////////////////////////////////////////////////////////////////

labeled_spinBox::labeled_spinBox(QString label_name, int minimum, int maximum )
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

labeled_spinBox::~labeled_spinBox()
{

}

///////////////////////////////////////////////////////////////////////////////

void labeled_spinBox::downloadAll()
{

}

///////////////////////////////////////////////////////////////////////////////

StaticSetting::StaticSetting(QString new_name, qreal new_value, bool isChangeable)
{
	Name = new_name;
	Value = new_value;
	bChangeable = isChangeable;

	setText(QString("%1: %2").arg(new_name).arg(new_value));

}

///////////////////////////////////////////////////////////////////////////////

StaticSetting::~StaticSetting()
{
}
