#pragma once
#include "stdafx.h"
#include "chart.h"
#include "settings.h"
#include "data.h"
#include "ui_f2837xsgui.h"
#include "F28377S_Device.h"
#include "actionButtons.h"
#include "cli.h"


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

	void launch_GUI();

	F28377S_Device * hDevice;	// handle for the USB device

	// data and charts have unique names for better readability
	InteractiveChart * xChart;
	InteractiveChart * yChart;
	InteractiveChart * zChart;

	MeasureData2D * xData;
	MeasureData2D * yData;
	MeasureData2D * zData;

	// an extra vector for iteration
	QVector<MeasureData2D * > * data_vec;
	QVector<InteractiveChart *> * chart_vec;

	// ui-elements
	QGridLayout * mainLayout;
	Settings_ui * settings;
	ActionButtons * actionButtons;
	QListWidget * messageList;	
	Cli * cli;


private slots:
	void on_testButton_clicked();
	void on_exitButton_clicked();
	void on_clearButton_clicked();
	void test_routine();

private:
	Ui::F2837xSGUIClass ui;
	bool bGuiOfflineMode = false;
	void createCharts();

	QTimer test_routine_timer;
};

