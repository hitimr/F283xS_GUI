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
	ui.setupUi(this);

	findDevice();
	createCharts();	

	settings = new Settings_ui(hUSB);
	actionButtons = new ActionButtons(hUSB);
	messageList = new QListWidget();

	hUSB->setMessageList(messageList);

	ui.inputLayout->addWidget(settings);
	ui.inputLayout->addWidget(actionButtons);
	ui.inputLayout->addWidget(messageList, Qt::AlignBottom);	


	new QListWidgetItem(tr("Init complete"), messageList);
	if (bGuiOfflineMode) new QListWidgetItem(tr("Warning: no device connected"), messageList);

	connect(&test_routine_timer, SIGNAL(timeout()), this, SLOT(test_routine()));
}		

///////////////////////////////////////////////////////////////////////////////

F2837xSGUI::~F2837xSGUI()
{

}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::createCharts()
{
	// create charts and add them to its respective area
	xChart = new InteractiveChart("X-Axis", Qt::blue);
	yChart = new InteractiveChart("Y-Axis", Qt::red);
	zChart = new InteractiveChart("Z-Axis", Qt::green);

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
	xData = new MeasureData2D();
	yData = new MeasureData2D();
	zData = new MeasureData2D();

	//connect data to their respective charts
	xChart->setData(xData);
	yChart->setData(yData);
	zChart->setData(zData);

	xData->setMessageList(messageList);

	hUSB->setXData(xData);

}

///////////////////////////////////////////////////////////////////////////////

//  button to quickly test various functions
void F2837xSGUI::on_testButton_clicked()
{
	//xData->generateTestData(512);
	//yData->generateTestData(128);

	//test_routine_timer.start(15);

	//hUSB->Debug_Data(ON);
	//hUSB->Save_Raw_Data(ON);

		//hUSB->get_all();
		//xChart->update();
	
	settings->update();



}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::test_routine()
{
	xData->generateTestData(50);
	//hUSB->get_all();
}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::findDevice()
{
	hUSB = new F28377S_Device();

	if (!hUSB->isOnline())
	{
		QMessageBox msgBox;
		msgBox.setText(tr("Device not found"));
		msgBox.setInformativeText("Please check the connection and if the device has been flashed properly");

		msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Ignore | QMessageBox::Close);
		msgBox.setDefaultButton(QMessageBox::Retry);

		switch (msgBox.exec())
		{
		case QMessageBox::Retry:
			findDevice();
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

}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::on_exitButton_clicked()
{
	if (hUSB->isOnline())
	{
		hUSB->Debug_Data(OFF);
		hUSB->Save_Raw_Data(OFF);
	}

	QCoreApplication::quit();
	exit(0);
}

