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

// add an array. for now we keep out x values as out indices. ToDo: change this
void MeasureData2D::add(int32_t * arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		add((qreal)size(), (qreal)arr[n]);
	}
}

///////////////////////////////////////////////////////////////////////////////

// clear all saved data
void MeasureData2D::clear()
{
	X.clear();
	Y.clear();

	fft_X.clear();
	fft_Y.clear();
}

///////////////////////////////////////////////////////////////////////////////

//generate a distrorted sine signal with a real time axis
void MeasureData2D::generateTestData(int cnt)
{
	int strecht = 150;
	using namespace chrono;

	auto t0 = high_resolution_clock::now();
	int start_index = size();

	for (int i = 0; i < cnt; i++)
	{
		qreal p =((2*sin(2*M_PI / strecht * (i+start_index)) * 100)
			+ (sin(8 * M_PI / strecht * (i + start_index)) * 100)
			//+ (sin(11 * M_PI / strecht * (i+start_index)) * 100)
			);

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
	// this should normally not happen but if we have an odd number of values we throw awaz the last one
	if ((fft_Y.size() % 2) != 0)
	{
		X.pop_back();
		Y.pop_back();
	}
	
	// prepare data containers
	fft_X.clear();
	fft_Y.clear();
	fft_X.resize(X.size());
	fft_Y.resize(Y.size());

	transformer.setSize(X.size());

	double* input  = &Y[0];			// we have to transform from std::vector to a simple array
	double* output = &fft_Y[0];		// apparently this works..

	transformer.forwardTransform(input, output);
	//transformer.rescale(input);

	for (int i = 0; i < (fft_X.size()/2); i++)
	{
		fft_X[2*i] = 2 * i;
		fft_X[2 * i + 1] = 2 * i + 1;

		// calculate absolute value
		fft_Y[i] = sqrt(
			(fft_Y[i] * fft_Y[i]) +
			(fft_Y[i + fft_X.size() / 2] * fft_Y[i + fft_X.size() / 2])
		);
	}



	// second half of the array only contains imaginary values so we throw them away
	fft_X.resize(fft_X.size() / 2);
	fft_Y.resize(fft_Y.size() / 2);	
}

///////////////////////////////////////////////////////////////////////////////

void MeasureData2D::FFTenable()
{
	data_pointer_x = &fft_X;
	data_pointer_y = &fft_Y;

	bFFT_enabled = true;
}

///////////////////////////////////////////////////////////////////////////////

void MeasureData2D::FFTdisable()
{
	data_pointer_x = &X;
	data_pointer_y = &Y;

	bFFT_enabled = false;
}


///////////////////////////////////////////////////////////////////////////////

// since data comes in bulk and without a time signature our only option is linear interpolation between two timestamps
int MeasureData2D::interpolate_time(int start_index, int end_index, std::chrono::microseconds start_time, std::chrono::microseconds end_time)
{
	if ((end_index <= start_index) || (start_time > end_time))
		return -1;

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
	if (index > (int)data_pointer_x->size())
		return data_pointer_x->at(data_pointer_x->size());
	else
		return data_pointer_x->at(index);
}

///////////////////////////////////////////////////////////////////////////////

qreal MeasureData2D::y(int index)
{
	if (index > (int)data_pointer_y->size())
		return data_pointer_y->at(data_pointer_y->size());
	else
		return data_pointer_y->at(index);
}
