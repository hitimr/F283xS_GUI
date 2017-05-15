#pragma once
#include <Qwidget>
#include <QGridLayout>
#include <QPushButton>
#include <chart.h>
#include "F28377S_Device.h"
#include "settings.h"

///////////////////////////////////////////////////////////////////////////////
//
//		Action Buttons - A collection of various QPushButtons to send commands to the device
//
///////////////////////////////////////////////////////////////////////////////

class ActionButtons :
	public QWidget
{
	Q_OBJECT

public:
	explicit ActionButtons(F28377S_Device *, Settings_ui *, QVector<InteractiveChart *> *);
	~ActionButtons();

public slots:
	void update();

	void setDevice(F28377S_Device * new_device) { hDevice = new_device; }	// we need a reference to the device that holds the actual commands

private:
	F28377S_Device * hDevice;
	Settings_ui * settings;
	QGridLayout * mainLayout;
	QVector<InteractiveChart *> * charts;


	QPushButton * downloadButton;
	QPushButton * uploadButton;
	QPushButton	* flushButton;
	QPushButton	* pingButton;
	QPushButton	* record_hw_button;
	QPushButton * reconnectButton;
	QPushButton * systemStartStopButton;
};

