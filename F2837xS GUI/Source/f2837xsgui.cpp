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

	messageList = new QListWidget();
	hDevice->setMessageInterface(messageList);


	if(!bGuiOfflineMode)
		hDevice->fflush();

	createCharts();	

	settings = new Settings_ui(hDevice);
	actionButtons = new ActionButtons(hDevice, settings, chart_vec);
	cli = new Cli(hDevice);

	ui.inputLayout->addWidget(settings);
	ui.inputLayout->addWidget(actionButtons);
	ui.inputLayout->addWidget(messageList);	
	ui.inputLayout->addWidget(cli, Qt::AlignBottom);
	ui.debugDataCheckBox->setCheckState(Qt::Checked);



	if (bGuiOfflineMode)
	{
		new QListWidgetItem(tr("Warning: no device connected"), messageList);
	}
	else
	{
		settings->update();
		hDevice->Debug_Data(ui.debugDataCheckBox->isChecked());
	}

	new QListWidgetItem(tr("Init complete"), messageList);
	connect(&test_routine_timer, SIGNAL(timeout()), this, SLOT(test_routine()));	// ToDo: Remove before release
	connect(ui.debugDataCheckBox, SIGNAL(toggled(bool)), this->hDevice, SLOT(Debug_Data(bool)));
}		

///////////////////////////////////////////////////////////////////////////////

F2837xSGUI::~F2837xSGUI()
{

}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::createCharts()
{
	// create charts, add them to its respective area and vector
	chart_vec = new QVector<InteractiveChart *>();
	chart_vec->push_back(xChart = new InteractiveChart(hDevice, "X-Axis", Qt::blue));
	chart_vec->push_back(yChart = new InteractiveChart(hDevice, "Y-Axis", Qt::red));
	chart_vec->push_back(zChart = new InteractiveChart(hDevice, "Z-Axis", Qt::green));

	// buttons for toddling chart display
	QHBoxLayout * toggleChartButtonLayout = new QHBoxLayout();
	toggleChartButtonLayout->addWidget(xChart->toggleDisplayButton);
	toggleChartButtonLayout->addWidget(yChart->toggleDisplayButton);
	toggleChartButtonLayout->addWidget(zChart->toggleDisplayButton);
	ui.chartArea->addLayout(toggleChartButtonLayout, Qt::AlignTop);


	ui.chartArea->addWidget(xChart);
	ui.chartArea->addWidget(yChart);
	ui.chartArea->addWidget(zChart);
	ui.chartArea->addWidget(&QWidget());

	// create data containers
	data_vec = new QVector<MeasureData2D *>();
	data_vec->push_back(xData = new MeasureData2D());
	data_vec->push_back(yData = new MeasureData2D());
	data_vec->push_back(zData = new MeasureData2D());

	//connect data to their respective charts
	xChart->setData(xData);
	yChart->setData(yData);
	zChart->setData(zData);

	for (int i = 0; i < chart_vec->size(); i++)
	{
		chart_vec->at(i)->setData(data_vec->at(i));
		data_vec->at(i)->setMessageInterface(messageList);
	}

	hDevice->setXData(xData);
}

///////////////////////////////////////////////////////////////////////////////

//  button to quickly test various functions
void F2837xSGUI::on_testButton_clicked()
{	
	if (!test_routine_timer.isActive())
	{
		test_routine_timer.start(500);
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

	ui.setupUi(this);
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

