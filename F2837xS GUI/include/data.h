#pragma once
#include <vector>
#include <chrono>
#include "qfouriertransformer.h"


///////////////////////////////////////////////////////////////////////////////
//
//	Basic Container for holding 2D Data
//
///////////////////////////////////////////////////////////////////////////////


class MeasureData2D
{
public:
	MeasureData2D();
	~MeasureData2D();

	void add(qreal new_x, qreal new_y);
	void add(int32_t * arr, int n);
	void clear();
	void generateTestData(int cnt);

	void FFTransform();
	void FFTenable();
	void FFTdisable();
	bool FFT_isenabled() { return bFFT_enabled; }

	int interpolate_time(int from_index, int to_index, std::chrono::microseconds start_time, std::chrono::microseconds end_time);
	int size() { return (int)data_pointer_x->size(); }
	
	qreal x(int index);	// return data at index. returns NULL for empty sets
	qreal y(int index);	// if index is too big the last value gets returned


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

	QFourierTransformer transformer;
};

