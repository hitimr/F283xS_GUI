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

	settings = new Settings_ui();

	messageList = new QListWidget();
	hUSB->setMessageList(messageList);

	ui.inputLayout->addWidget(settings);
	ui.inputLayout->addWidget(messageList, Qt::AlignBottom);	


	new QListWidgetItem(tr("Init complete"), messageList);
	if (bGuiOfflineMode) new QListWidgetItem(tr("Warning: no device connected"), messageList);
}
		

///////////////////////////////////////////////////////////////////////////////

F2837xSGUI::~F2837xSGUI()
{

}

void F2837xSGUI::createCharts()
{
	// create charts and add them to its respective area
	xChart = new Chart("X-Axis", Qt::blue);
	yChart = new Chart("Y-Axis", Qt::red);
	zChart = new Chart("Z-Axis", Qt::green);

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

	//connect data to theri respective charts
	xChart->setData(xData);
	yChart->setData(yData);
	zChart->setData(zData);

	hUSB->setXData(xData);

}

///////////////////////////////////////////////////////////////////////////////

//  button to quickly test various functions
void F2837xSGUI::on_testButton_clicked()
{
	//new QListWidgetItem(tr("Generating Data"), messageList);
	//xData->generateTestData(256);
	//yData->generateTestData(128);

	//hUSB->ping();
	
	for (int i = 0; i < 10; i++)
	{
		//hUSB->get_all();
		//xChart->update();
	}
}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::findDevice()
{
	hUSB = new F28377S_Device();

	if (!hUSB->IsOnline())
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

void F2837xSGUI::on_exitButton_clicked()
{
	QCoreApplication::quit();
	exit(0);
}

