#include "infoBox.h"

///////////////////////////////////////////////////////////////////////////////
//
//		AbstractInfoBox Methods
//
///////////////////////////////////////////////////////////////////////////////

AbstractInfoBox::AbstractInfoBox()
{
	mainLayout = new QVBoxLayout();
	label_vec = new QVector<QWidget *>();
	setLayout(mainLayout);

	scrollAreaWidget = new QWidget();
	scrollArea = new QScrollArea();	
	scrollAreaLayout = new QVBoxLayout(scrollAreaWidget);

	//scrollAreaWidget->setLayout(scrollAreaLayout);
	scrollArea->setWidget(scrollAreaWidget);
	scrollArea->setWidgetResizable(true);
	mainLayout->addWidget(scrollArea);

	closeButton = new QPushButton();
	connect(closeButton, SIGNAL(clicked()), this, SLOT(on_closeButton_clicked()));
	mainLayout->addWidget(closeButton);
}

///////////////////////////////////////////////////////////////////////////////

AbstractInfoBox::~AbstractInfoBox()
{
}

///////////////////////////////////////////////////////////////////////////////

// derive displayed elements from label_vec
void AbstractInfoBox::show()
{
	for (int i = 0; i < label_vec->size(); i++)
	{
		scrollAreaLayout->addWidget(label_vec->at(i));
	}
}

///////////////////////////////////////////////////////////////////////////////

void AbstractInfoBox::on_closeButton_clicked()
{

}

///////////////////////////////////////////////////////////////////////////////
//
//		GpioLabel Methods
//
///////////////////////////////////////////////////////////////////////////////

GpioLabel::GpioLabel(QString name, QString gpio_number, QString pin_number, QString comment)
{
	mainLayout = new QHBoxLayout();
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum));

	mainLayout->addWidget(new QLabel(name));
	mainLayout->addWidget(new QLabel(gpio_number));
	mainLayout->addWidget(new QLabel(pin_number));
	mainLayout->addWidget(new QLabel(comment));

	setLayout(mainLayout);
}

///////////////////////////////////////////////////////////////////////////////

GpioLabel::~GpioLabel()
{

}

///////////////////////////////////////////////////////////////////////////////
//
//		GpioInfoBox Methods
//
///////////////////////////////////////////////////////////////////////////////

GpioInfoBox::GpioInfoBox()
{
	setWindowTitle(tr("GPIOs"));

	label_vec->push_back(new GpioLabel(tr("Name"), tr("GPIO Num"), tr("Pin Num"), tr("Comment")));
	label_vec->push_back(new InfoSeperator(""));

	label_vec->push_back(new InfoSeperator("EPWM "));
	label_vec->push_back(new GpioLabel("EPWM6A", "10", "-"));
	label_vec->push_back(new GpioLabel("EPWM6B", "11", "-"));
	label_vec->push_back(new GpioLabel("EPWM7A", "12", "40", "CNV_C_POS"));
	label_vec->push_back(new GpioLabel("EPWM7B", "13", "39", "CNV_C_NEG"));
	label_vec->push_back(new GpioLabel("EPWM8A", "14", "38", "CNV_B_NEG"));
	label_vec->push_back(new GpioLabel("EPWM8B", "15", "37", "CNV_B_NEG"));
	label_vec->push_back(new GpioLabel("EPWM9A", "16", "36", "CNV_A_NEG"));
	label_vec->push_back(new GpioLabel("EPWM9B", "17", "35", "CNV_A_NEG"));
	
	label_vec->push_back(new InfoSeperator("GPIO "));
	label_vec->push_back(new GpioLabel("BUSY_A", "41", "5", tr("Busy Indicator")));
	label_vec->push_back(new GpioLabel("BUSY_B", "99", "53", tr("Busy Indicator")));
	label_vec->push_back(new GpioLabel("BUSY_C", "20", "34", tr("Busy Indicator")));
	label_vec->push_back(new GpioLabel("BUFF_OE_A", "17", "35", tr("CNV_A_NEG")));
	// ToDo: fill up thew rest

	label_vec->push_back(new InfoSeperator("I2C "));
	label_vec->push_back(new GpioLabel("SCL_A", "92", "59", ""));
	label_vec->push_back(new GpioLabel("SDA_A", "91", "52", ""));

	label_vec->push_back(new InfoSeperator("SCI/UART "));
	label_vec->push_back(new GpioLabel("SCIRXD_B", "19", "75", tr("Hardwired to USB")));
	label_vec->push_back(new GpioLabel("SCITXD_B", "18", "76", tr("Hardwired to USB")));

	label_vec->push_back(new InfoSeperator(tr("OTHER ")));
	label_vec->push_back(new GpioLabel("XCLKOUT", "73", "12", tr("CPU Clock indicator")));
	label_vec->push_back(new GpioLabel("DEBUG_PIN", "10", "78", tr("Can be toggled for debugging purposes")));


	show();
}

///////////////////////////////////////////////////////////////////////////////

GpioInfoBox::~GpioInfoBox()
{

}

///////////////////////////////////////////////////////////////////////////////
//
//	Info Seperator Methods
//
///////////////////////////////////////////////////////////////////////////////

InfoSeperator::InfoSeperator(QString new_name)
{
	QLabel * label = new QLabel(new_name);
	label->setSizePolicy( QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred) );

	QFrame * line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);
	line->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));

	QHBoxLayout * layout = new QHBoxLayout();
	layout->addWidget(label);
	layout->addWidget(line);
	layout->setSpacing(0);

	setLayout(layout);
}

///////////////////////////////////////////////////////////////////////////////

InfoSeperator::~InfoSeperator()
{
}

///////////////////////////////////////////////////////////////////////////////
//
//		AboutInfoBox Methods
//
///////////////////////////////////////////////////////////////////////////////

AboutInfoBox::AboutInfoBox()
{
	mainLayout->addWidget(new QLabel( tr("F2837xS GUI")));
	mainLayout->addWidget(new QLabel( tr("V1.0")));
	mainLayout->addWidget(new QLabel(""));
	mainLayout->addWidget(new QLabel(tr("Author: Mario Hiti")));
}

///////////////////////////////////////////////////////////////////////////////

AboutInfoBox::~AboutInfoBox()
{
}
