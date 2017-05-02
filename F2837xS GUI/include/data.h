#pragma once
#include <vector>
#include <chrono>
#include <Qtimer>
#include <QListwidget> 
#include "qfouriertransformer.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Basic Container for holding 2D Data
//
///////////////////////////////////////////////////////////////////////////////


class MeasureData2D : public QObject
{
	Q_OBJECT;
public:
	MeasureData2D();
	~MeasureData2D();

	void add(qreal new_x, qreal new_y);
	void add(int32_t * arr, int n);
	void clear();
	void generateTestData(int cnt);
	void setMessageList(QListWidget * new_messageList) { messageList = new_messageList;  }

	void FFTransform(int start_index);
	int ClosestPowerOf2(int n);
	void FFTenable();
	void FFTdisable();
	qreal Average();
	void Remove_Offset();


	int interpolate_time(int from_index, int to_index, std::chrono::microseconds start_time, std::chrono::microseconds end_time);


	// Getter-Functions
	qreal x(int index);	// return data at index. returns NULL for empty sets
	qreal y(int index);	// if index is too big the last value gets returned
	int size() { return (int)data_pointer_x->size(); }
	qreal sampleRate() { return sample_rate; }
	bool FFT_isEnabled() { return bFFT_enabled; }
	QString xAxisTitle();
	QString yAxisTitle();

public slots:
	qreal update_sampleRate();


private: 
	// when we ask for a point on x/y we actually retrieve the values pointers are pointing to
	// this allows for seamless transistion bewtween different display types
	std::vector<qreal> * data_pointer_x;	
	std::vector<qreal> * data_pointer_y;

	std::vector<qreal>  X;
	std::vector<qreal>  Y;

	std::vector<qreal>  fft_X;
	std::vector<qreal>  fft_Y;
	bool bFFT_enabled = false;
	qreal adc_multiplier = 5.96E-6;

	QFourierTransformer transformer;
	QListWidget *		messageList;

	QTimer	sampleRate_update_timer;
	int added_samples;
	std::chrono::high_resolution_clock::time_point t0 = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	qreal sample_rate = 0;

	QString xAxis_title =		"Time [ms]";
	QString yAxis_title =		"Amplitude [mV]";
	QString FFT_xAxis_title =	"Frequncy [?]";
	QString FFT_yAxis_title =	"Amplitude [?]";
};

