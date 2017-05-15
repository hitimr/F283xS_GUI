#pragma once
#include "f2837xsgui.h"

///////////////////////////////////////////////////////////////////////////////
//
//				Main GUI Methods
//
///////////////////////////////////////////////////////////////////////////////


F2837xSGUI::F2837xSGUI(QWidget *parent)	: 
	QMainWindow(parent)
{
	hDevice = new F28377S_Device();

	launch_GUI();

	hDevice->setMessageInterface(messageList);

	createGuiElements();

	if (bGuiOfflineMode)
	{
		new QListWidgetItem(tr("Warning: no device connected"), messageList);
	}
	else
	{		
		hDevice->fflush();
		settings->update();
		hDevice->setData(data_vec);
		hDevice->Debug_Data(ui.debugDataCheckBox->isChecked());
	}

	connect(&test_routine_timer,		SIGNAL(timeout()),		this,		SLOT(test_routine()));	// ToDo: Remove before release
	connect(ui.debugDataCheckBox,		SIGNAL(toggled(bool)),	hDevice,	SLOT(Debug_Data(bool)));
	connect(ui.clearMessabeBoxButton,	SIGNAL(clicked()),		this,		SLOT(on_clearButton_clicked()));

	messageList->clear();
	new QListWidgetItem(tr("Init complete"), messageList);
}		

///////////////////////////////////////////////////////////////////////////////

F2837xSGUI::~F2837xSGUI()
{

}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::createGuiElements()
{
	// create charts, add them to its respective area and vector
	chart_vec = new QVector<InteractiveChart *>();
	data_vec = new QVector<MeasureData2D *>();
	QHBoxLayout * toggleChartButtonLayout = new QHBoxLayout();

	chart_vec->push_back(new InteractiveChart(hDevice, "X-Axis", Qt::blue));
	chart_vec->push_back(new InteractiveChart(hDevice, "Y-Axis", Qt::red));
	chart_vec->push_back(new InteractiveChart(hDevice, "Z-Axis", Qt::green));

	ui.chartArea->addLayout(toggleChartButtonLayout, Qt::AlignTop);

	for (int i = 0; i < chart_vec->size(); i++)
	{
		toggleChartButtonLayout->addWidget(chart_vec->at(i)->toggleDisplayButton);

		data_vec->push_back(new MeasureData2D());
		chart_vec->at(i)->setData(data_vec->at(i));
		data_vec->at(i)->setMessageInterface(messageList);

		ui.chartArea->addWidget(chart_vec->at(i));
	}

	settings = new Settings_ui(hDevice);
	actionButtons = new ActionButtons(hDevice, settings, chart_vec);
	cli = new Cli(hDevice);

	ui.inputLayout->addWidget(settings);
	ui.inputLayout->addWidget(actionButtons);
	ui.inputLayout->addWidget(messageList);
	ui.inputLayout->addWidget(cli, Qt::AlignBottom);
	ui.debugDataCheckBox->setCheckState(Qt::Unchecked);
	ui.clearMessabeBoxButton->setText(tr("Clear Messages"));
}

///////////////////////////////////////////////////////////////////////////////

//  button to quickly test various functions
void F2837xSGUI::on_testButton_clicked()
{	
	if (!test_routine_timer.isActive())
	{
		test_routine_timer.start(1000);
	}
	else
	{
		test_routine_timer.stop();
	}
}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::test_routine()
{
	hDevice->Record_HW();

	for (int i = 0; i < chart_vec->count(); i++)	// charts only update themselves if their size changes. Record_HW creates data batches of identical size. therefore we have to call redraw manually
	{
		chart_vec->at(i)->redraw();
	}
}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::launch_GUI()
{
	messageList = new QListWidget();

	if (!hDevice->isOnline())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Device not found"));
		msgBox.setInformativeText("Please check the connection and if the device has been flashed properly");

		msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Ignore | QMessageBox::Close);
		msgBox.setDefaultButton(QMessageBox::Retry);

		switch (msgBox.exec())
		{
		case QMessageBox::Retry:
			launch_GUI();
			break;
		case QMessageBox::Ignore:
			bGuiOfflineMode = true;
			break;
		case QMessageBox::Close:
			on_exitButton_clicked();
			break;
		defaul:
			break;
		}
	}
	else
	{
		bGuiOfflineMode = false;
	}

	ui.setupUi(this);	// creates GUI elemements defined by the .ui file
}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::on_exitButton_clicked()
{
	if (hDevice->isOnline())
	{
		hDevice->Debug_Data(OFF);
		hDevice->Save_Raw_Data(OFF);
	}

	QCoreApplication::quit();
	exit(0);
}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::on_clearButton_clicked()
{
	messageList->clear();
}

///////////////////////////////////////////////////////////////////////////////

