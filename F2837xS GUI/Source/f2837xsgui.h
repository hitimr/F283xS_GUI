#pragma once
#include "stdafx.h"


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

	Chart * xChart;
	Chart * yChart;
	Chart * zChart;

	Settings_ui * settings;

	MeasureData2D * xData;
	MeasureData2D * yData;
	MeasureData2D * zData;

	QListWidget * messageList;	

private slots:
	void on_testButton_clicked();

private:
	Ui::F2837xSGUIClass ui;
};

