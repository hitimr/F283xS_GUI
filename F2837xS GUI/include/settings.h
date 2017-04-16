#pragma once
#include <QWidget>
#include <QSpinbox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QVector>


///////////////////////////////////////////////////////////////////////////////
//
//	StaticSettings are basically just labels but their name and value can be retrieved seperately
//
///////////////////////////////////////////////////////////////////////////////


class StaticSetting : 
	public QLabel
{
	Q_OBJECT

public:
	StaticSetting(QString new_name = "no name", qreal new_value = 0, bool isChangeable = false);
	~StaticSetting();

	QString name() { return Name; }
	qreal value() { return Value;  }

private:
	QString Name;
	qreal	Value;
	bool	bChangeable;
};


///////////////////////////////////////////////////////////////////////////////
//
//	Labeled_Input: simple Widget that hold a Label and an InputLine
//
///////////////////////////////////////////////////////////////////////////////

class DynamicSetting :
	public QWidget
{
	Q_OBJECT

public:
	DynamicSetting(QString label_name = "no name", int min = 0, int max = 0);
	~DynamicSetting();

	QSpinBox * spinBox;

	void setText(QString new_name) { label->setText(new_name); }

	void downloadAll();

private:
	QHBoxLayout * mainLayout;
	QLabel * label;
};



///////////////////////////////////////////////////////////////////////////////
//
//	Settings_ui: main class that holds a ui to make changes to the  Controller
//	At its core the object consists of a top row of buttons and two vectors that hold all the settings (dynamic, static)
//	The GUI elements are derived by the vectors
//
///////////////////////////////////////////////////////////////////////////////


class Settings_ui :
	public QWidget	
{
	Q_OBJECT

public:
	Settings_ui();
	~Settings_ui();

	void initialize();


	QVBoxLayout * mainLayout;

	void generate_topButtons();
	void generate_dynamicLayout();
	void generate_staticLayout();

	// top buttons
	QHBoxLayout * buttonyLayout;
	QPushButton * loadFromFileButton;
	QPushButton * downloadButton;
	QPushButton * uploadButton;

	// dynamic settings
	QVBoxLayout * dynamicSettingsLayout;
	DynamicSetting * sampleRate;
	DynamicSetting * averageingRate;

	// static settings
	QGridLayout * staticSettingsLayout;

private:
	int	maxColumns = 2;
	QVector<DynamicSetting *> dynamic_settings_vec;
	QVector<StaticSetting *> static_settings_vec;
	void insert_seperator();
};


