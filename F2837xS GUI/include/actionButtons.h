#pragma once
#include <Qwidget>
#include <QGridLayout>
#include <QPushButton>
#include "F28377S_Device.h"

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
	explicit ActionButtons(F28377S_Device *);
	~ActionButtons();

	void setDevice(F28377S_Device * new_device) { hDevice = new_device; }	// we need a reference to the device that holds the actual commands

private:
	F28377S_Device * hDevice;
	QGridLayout * mainLayout;

	QPushButton	* flushButton;
	QPushButton	* pingButton;
};

