#pragma once
#include "stdafx.h"
#include "data.h"


QT_CHARTS_BEGIN_NAMESPACE
class QLineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE
QT_CHARTS_USE_NAMESPACE

class QGestureEvent;

QT_BEGIN_NAMESPACE
class QGestureEvent;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


///////////////////////////////////////////////////////////////////////////////
//
//				ChartArea Class - holds the actual chart
//
///////////////////////////////////////////////////////////////////////////////


class ChartArea : public QChart
{
	Q_OBJECT
friend class Chart;
public:
	explicit ChartArea(Qt::GlobalColor color = Qt::black, QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
	~ChartArea();

	void clear();
	void add(qreal new_x, qreal new_y);
	void updateAxis();

protected:
	bool sceneEvent(QEvent *event);

private:
	bool gestureEvent(QGestureEvent *event);

	// min and max scales
	qreal			y_max =  1;
	qreal			y_min = -1;
	qreal			x_max =  1;
	qreal			x_min = -1;

	
	QLineSeries *	plot_series		= new QLineSeries();		// defaul series that gets displayed
	QValueAxis *	abstract_axisX	= new QValueAxis();
	QValueAxis *	abstract_axisY	= new QValueAxis();

	// after reaching that amount of samples  we start scrolling
	int max_display_count = 2000;
};

///////////////////////////////////////////////////////////////////////////////
//
//	ChartView Class - basic QChartView plus the added functionality of mouce gesture events
//
///////////////////////////////////////////////////////////////////////////////


class ChartView : public QChartView
{
	Q_OBJECT
public:
	explicit ChartView(QChart *chart, QWidget *parent = 0);
	void clear() { };

protected:
	bool viewportEvent(QEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	bool m_isTouching;
};

///////////////////////////////////////////////////////////////////////////////
//
//	Chart Class - used to create instances of an interactive chart
//
///////////////////////////////////////////////////////////////////////////////


class Chart : public QWidget
{
	Q_OBJECT
public:
	Chart(QString title, Qt::GlobalColor color);
	~Chart();

	// layouts
	QGridLayout *		mainGridLayout;
	QGridLayout *		buttonLayout;

	// in order for mouse gestures to work neither the actual chart nor the chartView can be a sibling of of the other
	// thus both are combined in this class
	ChartArea *			chartArea;
	ChartView *			chartView;

	QPushButton *		saveButton;
	QPushButton *		startStopButton;
	QPushButton *		clearButton;	
	QPushButton *		resetZoomButton;

	void	clear();
	void	setData(MeasureData2D * new_data);
	void	setTitle(QString new_title) { chartArea->setTitle(new_title); }

	QString title() { return chartArea->title(); }	

public slots:
	void	update();
	void	on_clearButton_clicked() { clear(); }
	void	on_resetZoomButton_clicked() { chartArea->zoomReset(); }
	int		on_saveButton_clicked();

private:
	MeasureData2D *		data;			// a chart has its own data container. after a fixed interverall update() gets called and the missing parts of the graph get drawn
	int					plot_index;		// gets increased by 1 whenever a point is drawn. even when the graph has reached its maximum
	QTimer				update_timer;
};

