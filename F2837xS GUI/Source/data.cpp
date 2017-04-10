#pragma once
#include "data.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//				MeasureData2D
//
///////////////////////////////////////////////////////////////////////////////


MeasureData2D::MeasureData2D()
{
	data_pointer_x = &X;
	data_pointer_y = &Y;

	bFFT_enabled = false;

}

///////////////////////////////////////////////////////////////////////////////


MeasureData2D::~MeasureData2D()
{

}

///////////////////////////////////////////////////////////////////////////////

void MeasureData2D::add(qreal new_x, qreal new_y)
{ 
	X.push_back(new_x);
	Y.push_back(new_y);
}

///////////////////////////////////////////////////////////////////////////////

// clear all saved data
void MeasureData2D::clear()
{
	X.clear();
	Y.clear();
}

///////////////////////////////////////////////////////////////////////////////

//generate a distrorted sine signal with a real time axis
void MeasureData2D::generateTestData(int cnt)
{
	using namespace chrono;

	auto t0 = high_resolution_clock::now();
	int start_index = size();
	for (int i = 0; i < cnt; i++)
	{
		qreal p = (sin(M_PI / 50 * i) * 100);
		p += qrand() % 20;
		add(i + start_index, p);
	}
	auto t1 = high_resolution_clock::now();

	interpolate_time(0, cnt, microseconds(0), duration_cast<microseconds>(t1 - t0) );
}

///////////////////////////////////////////////////////////////////////////////

// calculates the FFT of the data that is being held
// the original data is being kept. only the display pointers are being switched
void MeasureData2D::FFTransform()
{
	fft_X.clear();
	fft_Y.clear();

	for (int i = 0; i < X.size(); i++)
	{
		fft_X.push_back(X[i]);
		fft_Y.push_back(Y[i]+50);
	}

	data_pointer_x = &fft_X;
	data_pointer_y = &fft_Y;
}

///////////////////////////////////////////////////////////////////////////////

// since data comes in bulk and without a time signature our only option is linear interpolation between two timestamps
int MeasureData2D::interpolate_time(int start_index, int end_index, std::chrono::microseconds start_time, std::chrono::microseconds end_time)
{
	if ((end_index <= start_index) || (start_time > end_time))
		return ERROR_BAD_ARGUMENTS;

	if ((start_index + 1) == end_index)
	{
		//this extra case is necessary since the algorithm below migth produce NaN
		X[start_index] = (qreal)start_time.count();
		X[end_index] = (qreal)end_time.count();
		return 0;
	}

	qreal step = (end_time - start_time).count() / (qreal)(end_index - start_index - 1);


	for (int i = 0; i < (end_index - start_index); i++)
	{
		X[start_index + i] = (qreal)start_time.count() + i*step;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

qreal MeasureData2D::x(int index)
{
	if (index > (int)X.size())
		return X[X.size()];
	else
		return X[index];
}

///////////////////////////////////////////////////////////////////////////////

qreal MeasureData2D::y(int index)
{
	if (index > (int)Y.size())
		return Y[Y.size()];
	else
		return Y[index];
}
