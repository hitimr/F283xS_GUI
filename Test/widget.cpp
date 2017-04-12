/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "widget.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QValueAxis>
#include <QTimer>

QT_CHARTS_USE_NAMESPACE

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
	m_chart = new QChart;
	QChartView *chartView = new QChartView(m_chart);
	chartView->setMinimumSize(800, 600);
	m_series = new QLineSeries;
	m_chart->addSeries(m_series);
	QValueAxis *axisX = new QValueAxis;
	axisX->setRange(0, 2000);
	axisX->setLabelFormat("%g");
	axisX->setTitleText("Samples");
	QValueAxis *axisY = new QValueAxis;
	axisY->setRange(-1, 1);
	axisY->setTitleText("Audio level");
	m_chart->setAxisX(axisX, m_series);
	m_chart->setAxisY(axisY, m_series);
	m_chart->legend()->hide();
	m_chart->setTitle("Data from the microphone");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(chartView);
	setLayout(mainLayout);

	connect(&update_timer, SIGNAL(timeout()), this, SLOT(handle_timeout()));
	update_timer.start(30);

}

Widget::~Widget()
{

}

void Widget::handle_timeout()
{
	static qreal data[101];
	for (int i = 0; i < 101; i++)
	{
		data[i] = i;
	}
	qint64 range = 2000;
	QVector<QPointF> oldPoints = m_series->pointsVector();
	QVector<QPointF> points;
	int resolution = 4;

	if (oldPoints.count() < range) {
		points = m_series->pointsVector();
	}
	else {
		for (int i = maxSize / resolution; i < oldPoints.count(); i++)
			points.append(QPointF(i - maxSize / resolution, oldPoints.at(i).y()));
	}

	qint64 size = points.count();
	for (int k = 0; k < maxSize / resolution; k++)
		points.append(QPointF(k + size, (qint32)data[resolution * k]));

	m_series->replace(points);
}
