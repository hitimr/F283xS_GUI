#pragma once
#include "stdafx.h"
#include "chart.h"
#include "settings.h"
#include "data.h"
#include "ui_f2837xsgui.h"
#include "F28377S_Device.h"


#define OFFLINE_MODE	0xA
#define ONLINE_MODE		0xB


QT_CHARTS_USE_NAMESPACE

///////////////////////////////////////////////////////////////////////////////
//
//	Main Window is divided into 2 halfes and includes some actionbar stuff
//	All of that is defined in the .ui form
//
///////////////////////////////////////////////////////////////////////////////

class F2837xSGUI : public QMainWindow
{
	Q_OBJECT

public:
	F2837xSGUI(QWidget *parent = 0);
	~F2837xSGUI();

	void findDevice();

	F28377S_Device * hUSB;

	Chart * xChart;
	Chart * yChart;
	Chart * zChart;

	Settings_ui * settings;

	MeasureData2D * xData;
	MeasureData2D * yData;
	MeasureData2D * zData;

	QListWidget * messageList;	
	QGridLayout * mainLayout;

private slots:
	void on_testButton_clicked();
	void on_exitButton_clicked();

private:
	Ui::F2837xSGUIClass ui;
	bool bGuiOfflineMode = false;
	void createCharts();
};

