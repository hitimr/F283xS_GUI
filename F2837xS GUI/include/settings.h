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

class labeled_spinBox :
	public QWidget
{
	Q_OBJECT

public:
	labeled_spinBox(QString label_name = "no name", int min = 0, int max = 0);
	~labeled_spinBox();

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

	void generate_topButtons();
	void generate_dynamicLayout();

	void generate_staticLayout();

	QVBoxLayout * mainLayout;


	// top buttons
	QHBoxLayout * buttonyLayout;
	QPushButton * loadFromFileButton;
	QPushButton * downloadButton;
	QPushButton * uploadButton;

	// dynamic settings
	QVBoxLayout * dynamicSettingsLayout;
	labeled_spinBox * sampleRate;
	labeled_spinBox * averageingRate;

	// static settings
	QGridLayout * staticSettingsLayout;
	int	staticColumns = 2;

private:
	QFrame	* separator1;

	QVector<StaticSetting *> dynamic_settings;
	QVector<StaticSetting *> static_settings;
};


