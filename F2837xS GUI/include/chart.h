#pragma once
#include <iostream>
#include <QChart>
#include <QLineSeries>
#include <QValueAxis>
#include <QChartView>
#include <QTimer>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QTextStream>
#include <QFileDialog>
#include <QGesture>
#include <QPushButton>
#include <QSpinBox>
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
//				self-updating QChart
//
///////////////////////////////////////////////////////////////////////////////


class Chart : public QChart
{
	Q_OBJECT
friend class InteractiveChart;
public:
	explicit Chart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0);
	~Chart();

	void clear();
	void setAxisToDefaultRange();
	void add(qreal new_x, qreal new_y);
	void update_axis();
	void update_title();
	void setData(MeasureData2D * new_data) { data = new_data; }
	void setName(QString new_name) { name = new_name; }
	int  range() { return i32Range; }

public slots:
	void update();

protected:
	bool sceneEvent(QEvent *event);

private:
	bool gestureEvent(QGestureEvent *event);

	// min and max scales
	qreal			y_max =  1;
	qreal			y_min = -1;
	qreal			x_max =  1;
	qreal			x_min = -1;

	MeasureData2D * data;

	
	QLineSeries *	plot_series		= new QLineSeries();		// defaul series that gets displayed

	// after reaching that amount of samples  we start scrolling
	int32_t			i32Range = 4096;
	int32_t			i32Plot_index;		// gets increased by 1 whenever a point is drawn. even when the graph has reached its maximum	
	int32_t			i32Resolution = 1;
	QString			name = "no name";
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
//	A QChart with additional features
//
///////////////////////////////////////////////////////////////////////////////


class InteractiveChart : public QWidget
{
	Q_OBJECT
public:
	InteractiveChart(QString title, Qt::GlobalColor color);
	~InteractiveChart();

	// layouts
	QGridLayout *		mainGridLayout;
	QGridLayout *		buttonLayout;

	// in order for mouse gestures to work neither the actual chart nor the chartView can be a sibling of of the other
	// thus both are combined in this class
	Chart				chart;
	ChartView *			chartView;

	QPushButton *		toggleDisplayButton;
	QPushButton *		saveButton;
	QPushButton *		startStopButton;
	QPushButton *		clearButton;	
	QPushButton *		resetZoomButton;
	QPushButton *		fftButton;
	QSpinBox *			resolutionSpinBox;


	void	clear();
	void	setData(MeasureData2D * new_data);
	void	setName(QString new_title) { chart.setName(new_title); chart.setTitle(new_title); }
	void	setColour(Qt::GlobalColor color, int width = 2);

	QString title() { return chart.title(); }	

public slots:
	void	redraw();
	void	update();
	void	replaceChart(Chart * new_chartArea);
	void	on_playButton_clicked();
	void	on_clearButton_clicked() { clear(); }
	void	on_resetZoomButton_clicked() { chart.zoomReset(); }
	void	on_toggleDisplayButton_clicked();
	void	on_fftButton_clicked();
	void	on_resolutionSpinBox_changed();
	int		on_saveButton_clicked();

private:
	MeasureData2D *		data;			// a chart has its own data container. after a fixed interverall update() gets called and the missing parts of the graph get drawn	
	QTimer				update_timer;
	bool				bUpdateEnabled;
};



