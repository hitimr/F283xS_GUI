#pragma once
#include <QLineEdit>
#include "F28377S_Device.h"

///////////////////////////////////////////////////////////////////////////////
//
//	Provides a simple command line interface for direct input into USB
//	Mainly for debugging purposes
//
///////////////////////////////////////////////////////////////////////////////


class Cli : public QLineEdit
{
	Q_OBJECT;

public:
	Cli(F28377S_Device * new_hDevice);
	~Cli();

public slots:
	void parse();

private:
	F28377S_Device * hDevice;
};

