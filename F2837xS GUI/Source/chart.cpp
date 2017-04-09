#pragma once
#include "stdafx.h"
#include "chart.h"
#include "data.h"

///////////////////////////////////////////////////////////////////////////////
//
//								Chart Area Methods
//
///////////////////////////////////////////////////////////////////////////////

ChartArea::ChartArea(Qt::GlobalColor color, QGraphicsItem *parent, Qt::WindowFlags wFlags)
	: QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
	// Seems that QGraphicsView (QChartView) does not grab gestures.
	// They can only be grabbed here in the QGraphicsWidget (QChart).
	grabGesture(Qt::PanGesture);
	grabGesture(Qt::PinchGesture);

	// Display settings
	setTitle("no title");
	legend()->hide();

	// line settings
	QPen pen(color);	// hold information about (color, width, ...)
	plot_series = new QLineSeries();
	pen.setWidth(2);
	plot_series->setPen(pen);

	addSeries(plot_series);
	createDefaultAxes();

	// axs settings
	setAxisX(abstract_axisX);
	setAxisY(abstract_axisY);
	axisX()->setRange(x_min, x_max);
	axisY()->setRange(y_min*1.1, y_max*1.1);	// todo: check why this line makes troubles
}

///////////////////////////////////////////////////////////////////////////////

ChartArea::~ChartArea()
{

}

///////////////////////////////////////////////////////////////////////////////

void ChartArea::clear()
{
	plot_series = new QLineSeries();
	abstract_axisX = new QValueAxis();
	abstract_axisY = new QValueAxis();
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
	chartArea = new ChartArea(color);
	setTitle(title);
	chartView = new ChartView(chartArea);
	chartView->setRenderHint(QPainter::Antialiasing);
	mainGridLayout->addWidget(chartView, 0,0);

	// add buttons
	buttonLayout = new QGridLayout();	

	QIcon icon("Icon/chart_saveData.png");
	saveButton = new QPushButton();	
	saveButton->setIcon(icon);
	saveButton->setToolTip(tr("Save Data"));
	buttonLayout->addWidget(saveButton, 0, 0 ,Qt::AlignTop);
	connect(saveButton, SIGNAL(clicked()), this, SLOT(on_saveButton_clicked()));

	icon.addFile("Icon/chart_playPause.png");
	startStopButton = new QPushButton();
	startStopButton->setIcon(icon);
	startStopButton->setToolTip(tr("Start collecting Data"));
	buttonLayout->addWidget(startStopButton);

	icon.addFile("Icon/chart_clearData.png");
	clearButton = new QPushButton();
	clearButton->setIcon(icon);
	clearButton->setToolTip(tr("Clear Data"));
	buttonLayout->addWidget(clearButton);
	connect(clearButton, SIGNAL(clicked()), this, SLOT(on_clearButton_clicked()));	

	icon.addFile("Icon/chart_resetZoom.png");
	resetZoomButton = new QPushButton();
	resetZoomButton->setIcon(icon);
	resetZoomButton->setToolTip(tr("ResetZoom"));
	buttonLayout->addWidget(resetZoomButton);
	connect(resetZoomButton, SIGNAL(clicked()), this, SLOT(on_resetZoomButton_clicked()));

	mainGridLayout->addLayout(buttonLayout, 0, 1, Qt::AlignTop);	

	// Pointer to Measurement Data
	data = new MeasureData2D();
	plot_index = 0;

	// update-timer seetings
	QObject::connect(&update_timer, SIGNAL(timeout()), this, SLOT(update()));
	update_timer.setInterval(100);	
	update_timer.start();
}

///////////////////////////////////////////////////////////////////////////////

Chart::~Chart()
{
	delete chartArea;
	delete chartView;
	delete mainGridLayout;
}

///////////////////////////////////////////////////////////////////////////////

void Chart::clear()
{
	chartArea->clear();
	chartView->clear();
	data->clear();
	plot_index = 0;

	//ToDo: chartArea clear, chartView clear
}

///////////////////////////////////////////////////////////////////////////////

void Chart::setData(MeasureData2D * new_data)
{
	clear();
	data = new_data;
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

// Timeout and refresh plot area
void Chart::update()
{
	if ((data->size() != 0) && (data->size() > plot_index))
	{
		chartArea->setAnimationOptions(QChart::NoAnimation);	// no animations for plotting. animations get reactivated when a gesture happens
		while(plot_index < data->size())
		{
			qreal new_x = data->x(plot_index);
			qreal new_y = data->y(plot_index);

			chartArea->add(new_x, new_y);
			plot_index++;			
		}
		chartArea->updateAxis();
	}	
}