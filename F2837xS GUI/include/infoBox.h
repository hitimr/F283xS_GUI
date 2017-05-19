#pragma once
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>

///////////////////////////////////////////////////////////////////////////////
//
//		AbstractInfoBox provides a basic Dialog for providing Information
//
///////////////////////////////////////////////////////////////////////////////

class AbstractInfoBox : public QDialog
{
	Q_OBJECT;
public:

	AbstractInfoBox();
	~AbstractInfoBox();

public slots:
	void on_closeButton_clicked();

protected:
	QVBoxLayout * mainLayout;
	QGridLayout * infoLayout;
	QPushButton * closeButton;
};

///////////////////////////////////////////////////////////////////////////////
//
//	GpioLabel is aconvenienceclass that holds some information about a gpio
//
///////////////////////////////////////////////////////////////////////////////

class GpioLabel : QWidget
{
	Q_OBJECT;
public:
	GpioLabel(QString new_name, QString new_gpio_number, QString new_pin_number);
	~GpioLabel();

private:
	QHBoxLayout * mainLayout;
	QLabel * gpio_name;
	QLabel * gpio_number;
	QLabel * pin_number;
};

///////////////////////////////////////////////////////////////////////////////
//
//		GpioInfoBox provides a list for all connected Gpios
//
///////////////////////////////////////////////////////////////////////////////

class GpioInfoBox : AbstractInfoBox
{
	Q_OBJECT;
public:

	GpioInfoBox();
	~GpioInfoBox();
};

