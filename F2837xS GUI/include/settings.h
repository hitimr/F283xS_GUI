#pragma once
#include <QWidget>
#include <QSpinbox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QVector>
#include "F28377S_Device.h"
#include "usb_commands.h"

#define NUMERICAL 1
#define BOOLEAN	  2


///////////////////////////////////////////////////////////////////////////////
//
//	StaticSettings are basically just labels but their name and value can be retrieved seperately
//
///////////////////////////////////////////////////////////////////////////////



class StaticIntSetting : 
	public QLabel
{
	Q_OBJECT

public:
	explicit StaticIntSetting(F28377S_Device *, QString new_name = "no name", int command = 0xFF, int type = NUMERICAL);
	~StaticIntSetting();

	void update();
	void setLabelText();
	int value() { return Value; }

private:
	F28377S_Device * hDevice;
	QString Name;
	int		Value;
	int		usb_command;
	bool	isBoolean = false;
};

///////////////////////////////////////////////////////////////////////////////
//
//	DynamicSetting: simple Widget that hold a Label and a spin-box
//	I am well aware of the fact that this would be a prime example for multiple inheritance
//	from an abstract-settings class but for such a simple object its just not worth the effort	
//
///////////////////////////////////////////////////////////////////////////////

class DynamicSetting :
	public QWidget
{
	Q_OBJECT

public:
	DynamicSetting(F28377S_Device *, QString new_name = "no name", int new_command = 0xFF, int min = 0, int max = 0, qreal new_multiplier = 1);
	~DynamicSetting();

	QDoubleSpinBox * spinBox;

	void setText(QString new_name) { label->setText(new_name); }

	void downloadAll();
	void update();
	void upload();

private:
	QHBoxLayout * mainLayout;
	QLabel * label;
	F28377S_Device * hDevice;
	int i32Value = 0;
	qreal multiplier = 1;	// some settings are different from their actual value (i.e. SPICLK != SPI BRR)
	int usb_command;
};



///////////////////////////////////////////////////////////////////////////////
//
//	Settings_ui: main class that holds a ui to make changes to the  Controller
//	At its core the object consists of a top row of buttons and two vectors that hold all the settings (dynamic, static)
//	The GUI elements are derived by their respective vectors
//
///////////////////////////////////////////////////////////////////////////////


class Settings_ui :
	public QWidget	
{
	Q_OBJECT

public:
	Settings_ui(F28377S_Device *);
	~Settings_ui();

	void initialize();

	QVBoxLayout * mainLayout;

	void generate_dynamicLayout();
	void generate_staticLayout();

public slots:
	void update();
	void upload();

private:
	QGridLayout * dynamicSettingsLayout;
	QGridLayout * staticSettingsLayout;

	int	maxColumns = 2;
	QVector<DynamicSetting *> dynamic_settings_vec;
	QVector<StaticIntSetting *> static_settings_vec;
	F28377S_Device * hDevice;

	void insert_seperator();
};


