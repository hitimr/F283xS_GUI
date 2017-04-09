#pragma once
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QLineEdit>


///////////////////////////////////////////////////////////////////////////////
//
//	Labeled_Input: simple Widget that hold a Label and an InputLine
//	
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

	void generateTopButtons();
	void generateForm();

	QVBoxLayout * mainLayout;
	QHBoxLayout * buttonyLayout;
	QVBoxLayout * formLayout;

	QPushButton * loadFromFileButton;
	QPushButton * downloadButton;
	QPushButton * uploadButton;

	labeled_spinBox * sampleRate;
	labeled_spinBox * averageingRate;

private:
	QFrame	* button_form_border;
};


