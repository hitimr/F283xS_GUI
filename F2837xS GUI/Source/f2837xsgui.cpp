#pragma once
#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
//
//				Main GUI Methods
//
///////////////////////////////////////////////////////////////////////////////


F2837xSGUI::F2837xSGUI(QWidget *parent)	: 
	QMainWindow(parent)
{
	ui.setupUi(this);

	// create charts and add them to its respective area
	xChart = new Chart("X-Axis", Qt::blue);
	yChart = new Chart("Y-Axis", Qt::red);
	zChart = new Chart("Z-Axis", Qt::green);
	ui.chartArea->addWidget(xChart);
	ui.chartArea->addWidget(yChart);
	ui.chartArea->addWidget(zChart);

	// create data containers
	xData = new MeasureData2D();
	yData = new MeasureData2D();
	zData = new MeasureData2D();

	//connect data to theri respective charts
	xChart->setData(xData);
	yChart->setData(yData);
	zChart->setData(zData);
	

	settings = new Settings_ui();
	messageList = new QListWidget();
	ui.inputLayout->addWidget(settings);
	ui.inputLayout->addWidget(messageList, Qt::AlignBottom);

	new QListWidgetItem(tr("Init complete"), messageList);

}

///////////////////////////////////////////////////////////////////////////////

F2837xSGUI::~F2837xSGUI()
{

}

///////////////////////////////////////////////////////////////////////////////

void F2837xSGUI::on_testButton_clicked()
{
	new QListWidgetItem(tr("Generating Data"), messageList);
	xData->generateTestData(300);
	yData->generateTestData(800);
}