#pragma once
#include "chart.h"


///////////////////////////////////////////////////////////////////////////////
//
//								Chart Area Methods
//
///////////////////////////////////////////////////////////////////////////////

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags) : 
	QChart(QChart::ChartTypeCartesian, parent, wFlags)
{
	// Seems that QGraphicsView (QChartView) does not grab gestures.
	// They can only be grabbed here in the QGraphicsWidget (QChart).
	grabGesture(Qt::PanGesture);
	grabGesture(Qt::PinchGesture);

	// Display settings
	setTitle(name);
	legend()->hide();

	// line settings
	QPen pen;	// hold information about (color, width, ...)
	plot_series = new QLineSeries();
	pen.setWidth(2);
	plot_series->setPen(pen);

	addSeries(plot_series);
	createDefaultAxes();

	setAxisToDefaultRange();
}

///////////////////////////////////////////////////////////////////////////////

Chart::~Chart()
{

}

///////////////////////////////////////////////////////////////////////////////

void Chart::clear()
{
	removeSeries(plot_series);
	setAxisToDefaultRange();
	plot_series = new QLineSeries();
}

///////////////////////////////////////////////////////////////////////////////

void Chart::setAxisToDefaultRange()
{
	y_min = -1;
	y_max = 1;
	x_min = -1;
	x_max = 1;

	axisX()->setRange(x_min, x_max);
	axisY()->setRange(y_min, y_max);	// todo: check why this line makes troubles

}

///////////////////////////////////////////////////////////////////////////////

void Chart::add(qreal new_x, qreal new_y)
{
}

///////////////////////////////////////////////////////////////////////////////

void Chart::update_axis()
{
	x_min = plot_series->at(0).x();
	x_max = plot_series->at(plot_series->count()-1).x();

	removeSeries(plot_series);
	addSeries(plot_series);	

	createDefaultAxes();
	axisX()->setRange(x_min, x_max);
	axisY()->setRange(y_min, y_max);
}

///////////////////////////////////////////////////////////////////////////////

void Chart::update_title()
{
	setTitle(QString("%1 - %2 Samples/s").arg(name).arg(data->sampleRate()));
}

///////////////////////////////////////////////////////////////////////////////

bool Chart::sceneEvent(QEvent *event)
{
	if (event->type() == QEvent::Gesture)
		return gestureEvent(static_cast<QGestureEvent *>(event));

	return QChart::event(event);
}

///////////////////////////////////////////////////////////////////////////////

bool Chart::gestureEvent(QGestureEvent *event)
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

// refresh plot area
void Chart::update()
{
	if ((data->size() != 0) && (data->size() > i32Plot_index)) // update only if data has changed
	{
		setAnimationOptions(QChart::NoAnimation);	// no animations for plotting. animations get reactivated when a gesture happens
		int start_index;
		QVector<QPointF> points;

		if (data->size()/i32Resolution < i32Range)
			start_index = 0;

		else 	// chart is too bog to be fully displayed
			start_index = (data->size() - (i32Range*i32Resolution));

		int i = start_index;
		while(i < data->size())
		{
			//check for new boundaries
			if (data->y(i) > y_max)
				y_max = data->y(i);

			else if (data->y(i) < y_min)
				y_min = data->y(i);

			points.append(QPointF(data->x(i), data->y(i)));
			i += i32Resolution;
		}
		i32Plot_index = i;

		plot_series->replace(points);
		update_axis();
		update_title();

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

InteractiveChart::InteractiveChart(QString name, Qt::GlobalColor color)
{
	// fill up widget with its objects
	mainGridLayout = new QGridLayout();
	setLayout(mainGridLayout);

	// create displayed element
	setName(name);
	chartView = new ChartView(&chart);
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

	resolutionSpinBox = new QSpinBox();
	resolutionSpinBox->setMinimum(1);
	resolutionSpinBox->setToolTip(tr("Resolution: number of points to be skipped until another point is drawn"));
	resolutionSpinBox->setValue(1);
	connect(resolutionSpinBox, SIGNAL(finishedEditing()), this, SLOT(on_resolutionSpinBox_changed()));
	buttonLayout->addWidget(resolutionSpinBox);

	toggleDisplayButton = new QPushButton(QString(tr("Toggle '%1'").arg(name)));
	mainGridLayout->addLayout(buttonLayout, 0, 1, Qt::AlignTop);	
	connect(toggleDisplayButton, SIGNAL(clicked()), this, SLOT(on_toggleDisplayButton_clicked()));

	// Pointer to Measurement Data
	data = new MeasureData2D();
	chart.i32Plot_index = 0;

	bUpdateEnabled = true;
	update_timer.start(30);
	connect(&update_timer, SIGNAL(timeout()), this, SLOT(update()));

}

///////////////////////////////////////////////////////////////////////////////

InteractiveChart::~InteractiveChart()
{
	delete chartView;
	delete mainGridLayout;
}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::clear()
{
	chart.clear();
	data->clear();
	chart.i32Plot_index = 0;
}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::setData(MeasureData2D * new_data)
{
	data = new_data;
	chart.setData(new_data);
}

///////////////////////////////////////////////////////////////////////////////

// Fully redraws a chart
void InteractiveChart::redraw()
{
	chart.clear();
	chart.i32Plot_index = 0;
	chart.update();
}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::update()
{
	// update only if necessary
	if (bUpdateEnabled && isVisible())
	{
		if (resolutionSpinBox->value() != chart.i32Resolution)
		{
			// resolution changed
			chart.i32Resolution = resolutionSpinBox->value();
			redraw();
		}
		else
			chart.update();
	}
}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::replaceChart(Chart * new_chartArea)
{
	chartView = new ChartView(new_chartArea);
}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::on_playButton_clicked()
{
	if (bUpdateEnabled)
		bUpdateEnabled = false;
	else
		bUpdateEnabled = true;
}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::on_toggleDisplayButton_clicked()
{
	if (isVisible())
		hide();
	else
		show();
}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::on_fftButton_clicked()
{
	if (!data->FFT_isenabled())
	{
		int start_index = 0;
		if (data->size() > chart.range())
			start_index = data->size() - chart.range();

		data->FFTransform(start_index);
		data->FFTenable();
	}
	else
	{
		data->FFTdisable();
	}

	redraw();

}

///////////////////////////////////////////////////////////////////////////////

void InteractiveChart::on_resolutionSpinBox_changed()
{
	chart.i32Resolution = resolutionSpinBox->value();
	redraw();
}

///////////////////////////////////////////////////////////////////////////////

// opens a dialog to save the data
int InteractiveChart::on_saveButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("Text Files (*.txt)"));

	if (fileName == NULL)
		return 0; // cancel button clicked

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
		return data->size();
	}
}

///////////////////////////////////////////////////////////////////////////////


