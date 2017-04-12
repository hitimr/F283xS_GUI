#pragma once
#include "chart.h"


///////////////////////////////////////////////////////////////////////////////
//
//								Chart Area Methods
//
///////////////////////////////////////////////////////////////////////////////

ChartArea::ChartArea(QGraphicsItem *parent, Qt::WindowFlags wFlags) : 
	QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
	// Seems that QGraphicsView (QChartView) does not grab gestures.
	// They can only be grabbed here in the QGraphicsWidget (QChart).
	grabGesture(Qt::PanGesture);
	grabGesture(Qt::PinchGesture);

	// Display settings
	setTitle("no title");
	legend()->hide();

	// line settings
	QPen pen;	// hold information about (color, width, ...)
	plot_series = new QLineSeries();
	pen.setWidth(2);
	plot_series->setPen(pen);

	addSeries(plot_series);
	createDefaultAxes();

	// axis settings
	setAxisX(abstract_axisX);
	setAxisY(abstract_axisY);
	setAxisToDefaultRange();
}

///////////////////////////////////////////////////////////////////////////////

ChartArea::~ChartArea()
{

}

///////////////////////////////////////////////////////////////////////////////

void ChartArea::clear()
{
	removeSeries(plot_series);
	setAxisToDefaultRange();
	plot_series = new QLineSeries();
}

///////////////////////////////////////////////////////////////////////////////

void ChartArea::setAxisToDefaultRange()
{
	y_min = -1;
	y_max = 1;
	x_min = -1;
	x_max = 1;

	axisX()->setRange(x_min, x_max);
	axisY()->setRange(y_min, y_max);	// todo: check why this line makes troubles

}

///////////////////////////////////////////////////////////////////////////////

void ChartArea::add(qreal new_x, qreal new_y)
{
	plot_series->append(new_x, new_y);


	if (plot_series->count() > max_display_count)
		plot_series->remove(0);	// remove first point to prevent infinite graphs

	//check for new boundaries
	if (new_y > y_max)
		y_max = new_y;

	else if (new_y < y_min)
		y_min = new_y;
}

///////////////////////////////////////////////////////////////////////////////

void ChartArea::updateAxis()
{
	x_min = plot_series->at(0).x();
	x_max = plot_series->at(plot_series->count()-1).x();

	removeSeries(plot_series);	// for some reason its not possible to just append new points
	addSeries(plot_series);		// we have to clear the graph and reassign the series

	createDefaultAxes();
	axisX()->setRange(x_min, x_max);
	axisY()->setRange(y_min, y_max);
}

///////////////////////////////////////////////////////////////////////////////

bool ChartArea::sceneEvent(QEvent *event)
{
	if (event->type() == QEvent::Gesture)
		return gestureEvent(static_cast<QGestureEvent *>(event));

	return QChart::event(event);
}

///////////////////////////////////////////////////////////////////////////////

bool ChartArea::gestureEvent(QGestureEvent *event)
{
	if (QGesture *gesture = event->gesture(Qt::PanGesture)) {
		QPanGesture *pan = static_cast<QPanGesture *>(gesture);
		QChart::scroll(-(pan->delta().x()), pan->delta().y());
	}

	if (QGesture *gesture = event->gesture(Qt::PinchGesture)) {
		QPinchGesture *pinch = static_cast<QPinchGesture *>(gesture);
		if (pinch->changeFlags() & QPinchGesture::ScaleFactorChanged)
			QChart::zoom(pinch->scaleFactor());
	}
	return true;
}

// refresh plot area
void ChartArea::update()
{
	if ((data->size() != 0) && (data->size() > plot_index))
	{
		setAnimationOptions(QChart::NoAnimation);	// no animations for plotting. animations get reactivated when a gesture happens
		while (plot_index < data->size())
		{
			add(data->x(plot_index), data->y(plot_index));
			plot_index++;
		}
		updateAxis();
	}
}




///////////////////////////////////////////////////////////////////////////////
//
//								ChartView Methods
//
///////////////////////////////////////////////////////////////////////////////


ChartView::ChartView(QChart *chart, QWidget *parent) :
	QChartView(chart, parent),
	m_isTouching(false)
{
	setRubberBand(QChartView::RectangleRubberBand);
	setRenderHint(QPainter::Antialiasing);
}

///////////////////////////////////////////////////////////////////////////////

bool ChartView::viewportEvent(QEvent *event)
{
	if (event->type() == QEvent::TouchBegin) {
		// By default touch events are converted to mouse events. So
		// after this event we will get a mouse event also but we want
		// to handle touch events as gestures only. So we need this safeguard
		// to block mouse events that are actually generated from touch.
		m_isTouching = true;

		// Turn off animations when handling gestures they
		// will only slow us down.
		chart()->setAnimationOptions(QChart::NoAnimation);
	}
	return QChartView::viewportEvent(event);
}

///////////////////////////////////////////////////////////////////////////////

void ChartView::mousePressEvent(QMouseEvent *event)
{
	if (m_isTouching)
		return;
	QChartView::mousePressEvent(event);
}

///////////////////////////////////////////////////////////////////////////////

void ChartView::mouseMoveEvent(QMouseEvent *event)
{
	if (m_isTouching)
		return;
	QChartView::mouseMoveEvent(event);
}

///////////////////////////////////////////////////////////////////////////////

void ChartView::mouseReleaseEvent(QMouseEvent *event)
{
	if (m_isTouching)
		m_isTouching = false;

	// Because we disabled animations when touch event was detected
	// we must put them back on.
	chart()->setAnimationOptions(QChart::SeriesAnimations);

	QChartView::mouseReleaseEvent(event);
}

///////////////////////////////////////////////////////////////////////////////

// handle key event for scrolling
void ChartView::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Plus:
		chart()->zoomIn();
		break;
	case Qt::Key_Minus:
		chart()->zoomOut();
		break;
		//![1]
	case Qt::Key_Left:
		chart()->scroll(-10, 0);
		break;
	case Qt::Key_Right:
		chart()->scroll(10, 0);
		break;
	case Qt::Key_Up:
		chart()->scroll(0, 10);
		break;
	case Qt::Key_Down:
		chart()->scroll(0, -10);
		break;
	default:
		QGraphicsView::keyPressEvent(event);
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
//
//								Chart Methods
//
///////////////////////////////////////////////////////////////////////////////

Chart::Chart(QString title, Qt::GlobalColor color)
{
	// fill up widget with its objects
	mainGridLayout = new QGridLayout();
	setLayout(mainGridLayout);

	// create displayed element
	setTitle(title);
	chartView = new ChartView(&chartArea);
	chartView->setRenderHint(QPainter::Antialiasing);
	mainGridLayout->addWidget(chartView, 0,0);

	// add buttons
	buttonLayout = new QGridLayout();	

	saveButton = new QPushButton();
	saveButton->setIcon(QIcon("Icon/chart_saveData.png"));
	saveButton->setToolTip(tr("Save Data"));
	buttonLayout->addWidget(saveButton, 0, 0 ,Qt::AlignTop);
	connect(saveButton, SIGNAL(clicked()), this, SLOT(on_saveButton_clicked()));

	startStopButton = new QPushButton();
	startStopButton->setIcon(QIcon("Icon/chart_playPause.png"));
	startStopButton->setToolTip(tr("Start collecting Data"));
	buttonLayout->addWidget(startStopButton);
	connect(startStopButton, SIGNAL(clicked()), this, SLOT(on_playButton_clicked()));

	clearButton = new QPushButton();
	clearButton->setIcon(QIcon("Icon/chart_clearData.png"));
	clearButton->setToolTip(tr("Clear Data"));
	buttonLayout->addWidget(clearButton);
	connect(clearButton, SIGNAL(clicked()), this, SLOT(on_clearButton_clicked()));	

	resetZoomButton = new QPushButton();
	resetZoomButton->setIcon(QIcon("Icon/chart_resetZoom.png"));
	resetZoomButton->setToolTip(tr("ResetZoom"));
	buttonLayout->addWidget(resetZoomButton);
	connect(resetZoomButton, SIGNAL(clicked()), this, SLOT(on_resetZoomButton_clicked()));

	fftButton = new QPushButton();
	fftButton->setIcon(QIcon("Icon/chart_fft.png"));
	fftButton->setToolTip(tr("Fast Fourier Transform"));
	buttonLayout->addWidget(fftButton);
	connect(fftButton, SIGNAL(clicked()), this, SLOT(on_fftButton_clicked()));


	toggleDisplayButton = new QPushButton(QString(tr("Toggle '%1'").arg(title)));
	mainGridLayout->addLayout(buttonLayout, 0, 1, Qt::AlignTop);	
	connect(toggleDisplayButton, SIGNAL(clicked()), this, SLOT(on_toggleDisplayButton_clicked()));

	// Pointer to Measurement Data
	data = new MeasureData2D();
	chartArea.plot_index = 0;

	bUpdateEnabled = true;
	update_timer.start(100);
	connect(&update_timer, SIGNAL(timeout()), this, SLOT(update()));

}

///////////////////////////////////////////////////////////////////////////////

Chart::~Chart()
{
	delete chartView;
	delete mainGridLayout;
}

///////////////////////////////////////////////////////////////////////////////

void Chart::clear()
{
	chartArea.clear();
	data->clear();
	chartArea.plot_index = 0;
}

///////////////////////////////////////////////////////////////////////////////

void Chart::setData(MeasureData2D * new_data)
{
	data = new_data;
	chartArea.setData(new_data);
}

///////////////////////////////////////////////////////////////////////////////

// Fully redraws a chart
void Chart::redraw()
{
	chartArea.clear();
	chartArea.plot_index = 0;
	chartArea.update();
}

void Chart::update()
{
	if (bUpdateEnabled && isVisible())
	{
		chartArea.update();
	}
}

///////////////////////////////////////////////////////////////////////////////

void Chart::replaceChart(ChartArea * new_chartArea)
{
	chartView = new ChartView(new_chartArea);
}

///////////////////////////////////////////////////////////////////////////////

void Chart::on_playButton_clicked()
{
	if (bUpdateEnabled)
		bUpdateEnabled = false;
	else
		bUpdateEnabled = true;
}

///////////////////////////////////////////////////////////////////////////////

void Chart::on_toggleDisplayButton_clicked()
{
	if (isVisible())
		hide();
	else
		show();
}

///////////////////////////////////////////////////////////////////////////////

void Chart::on_fftButton_clicked()
{
	if (!data->FFT_isenabled())
	{
		data->FFTransform();
		data->FFTenable();
	}
	else
	{
		data->FFTdisable();
	}

	redraw();

}

///////////////////////////////////////////////////////////////////////////////

// opens a dialog to save the data
int Chart::on_saveButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("Text Files (*.txt)"));

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this, tr("Error"), tr("Unable to open File"));
		return -1;
	}
	else
	{
		QTextStream out(&file);
		out << title() << endl << endl;
		for (int i = 0; i < data->size(); i++)
		{
			out << data->x(i) << '\t' << data->y(i) << endl;
		}
		file.close();
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////


