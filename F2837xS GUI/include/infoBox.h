#pragma once
#include <QDialog>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QScrollArea>

///////////////////////////////////////////////////////////////////////////////
//
//		AbstractInfoBox provides a basic Dialog for providing Information.
//		All items are stored in label_vec
//
///////////////////////////////////////////////////////////////////////////////

class AbstractInfoBox : public QDialog
{
	Q_OBJECT;

public:
	AbstractInfoBox();
	~AbstractInfoBox();

	void show();

public slots:
	void on_closeButton_clicked();

protected:
	QVBoxLayout * mainLayout;

	QScrollArea * scrollArea;
	QWidget * scrollAreaWidget;
	QVBoxLayout * scrollAreaLayout;

	QPushButton * closeButton;
	QVector<QWidget *> * label_vec;
};

///////////////////////////////////////////////////////////////////////////////
//
//	InfoSeperator holds a label and a line to seperate information
//
///////////////////////////////////////////////////////////////////////////////

class InfoSeperator : public QWidget
{
	Q_OBJECT;
public:
	InfoSeperator(QString new_name);
	~InfoSeperator();
};

///////////////////////////////////////////////////////////////////////////////
//
//	GpioLabel is aconvenienceclass that holds some information about a gpio
//
///////////////////////////////////////////////////////////////////////////////

class GpioLabel : public QWidget
{
	Q_OBJECT;
public:
	GpioLabel(QString new_name, QString new_gpio_number, QString new_pin_number, QString comment = QString(""));
	~GpioLabel();

private:
	QHBoxLayout * mainLayout;
};

///////////////////////////////////////////////////////////////////////////////
//
//		GpioInfoBox provides a list for all connected Gpios
//
///////////////////////////////////////////////////////////////////////////////

class GpioInfoBox : public AbstractInfoBox
{
	Q_OBJECT;

public:
	GpioInfoBox();
	~GpioInfoBox();

};

///////////////////////////////////////////////////////////////////////////////
//
//		AboutInfoBox provides a list for all connected Gpios
//
///////////////////////////////////////////////////////////////////////////////

class AboutInfoBox : public AbstractInfoBox
{
	Q_OBJECT;
public:
	AboutInfoBox();
	~AboutInfoBox();
};