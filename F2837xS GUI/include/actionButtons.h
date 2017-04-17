#pragma once
#include <Qwidget>
#include <QGridLayout>
#include <QPushButton>
#include "F28377S_Device.h"

class ActionButtons :
	public QWidget
{
	Q_OBJECT

public:
	explicit ActionButtons();
	~ActionButtons();

	//void setDevice(F28377S_Device * new_device) { hDevice = new_device; }

private:
	//F28377S_Device * hDevice;
	QGridLayout * mainLayout;

	QPushButton	* flushButton;
};

