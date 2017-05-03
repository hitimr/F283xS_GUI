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

	if(!bGuiOfflineMode)
		hUSB->fflush();

	createCharts();	

	settings = new Settings_ui(hUSB);
	actionButtons = new ActionButtons(hUSB, settings, chart_vec);
	messageList = new QListWidget();

	hUSB->setMessageList(messageList);

	ui.inputLayout->addWidget(settings);
	ui.inputLayout->addWidget(actionButtons);
	ui.inputLayout->addWidget(messageList, Qt::AlignBottom);	



	if (bGuiOfflineMode)
	{
		new QListWidgetItem(tr("Warning: no device connected"), messageList);
	}
	else
	{
		settings->update();
	}

	new QListWidgetItem(tr("Init complete"), messageList);
	connect(&test_routine_timer, SIGNAL(timeout()), this, SLOT(test_routine()));
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
	chart_vec->push_back( xChart = new InteractiveChart(hUSB, "X-Axis", Qt::blue));
	chart_vec->push_back( yChart = new InteractiveChart(hUSB, "Y-Axis", Qt::red));
	chart_vec->push_back( zChart = new InteractiveChart(hUSB, "Z-Axis", Qt::green));

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
	hUSB->Record_HW();

	for (int i = 0; i < chart_vec->count(); i++)	// charts only update themselves if their size changes. Record_HW creates data batches of identical size. therefore we have to call redraw manually
	{
		chart_vec->at(i)->redraw();
	}
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

