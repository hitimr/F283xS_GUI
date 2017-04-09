#pragma once
#include "stdafx.h"

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
	formLayout = new QVBoxLayout;

	generateTopButtons();
	generateForm();


	//diosplay items
	mainLayout->addLayout(buttonyLayout);
	mainLayout->addLayout(formLayout);
	setLayout(mainLayout);
}


Settings_ui::~Settings_ui()
{
}

///////////////////////////////////////////////////////////////////////////////

// convenience function to generate Buttons
void Settings_ui::generateTopButtons()
{
	loadFromFileButton = new QPushButton(tr("From File"));
	downloadButton = new QPushButton(tr("Download"));
	uploadButton = new QPushButton(tr("Upload"));

	buttonyLayout->addWidget(loadFromFileButton);
	buttonyLayout->addWidget(downloadButton);
	buttonyLayout->addWidget(uploadButton);
}

///////////////////////////////////////////////////////////////////////////////

void Settings_ui::generateForm()
{
	// insert a line on top
	button_form_border = new QFrame();
	button_form_border->setFrameShape(QFrame::HLine);
	button_form_border->setFrameShadow(QFrame::Sunken);
	formLayout->addWidget(button_form_border);

	// create form
	sampleRate = new labeled_spinBox(tr("Sample Rate [Hz]"), 0, 5000);
	averageingRate = new labeled_spinBox(tr("Averaging Results"), 0 , 1000);

	formLayout->addWidget(sampleRate);
	formLayout->addWidget(averageingRate);
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

