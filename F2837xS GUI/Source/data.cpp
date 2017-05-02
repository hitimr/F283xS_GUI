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
	// connect output to time-spectrum
	data_pointer_x = &X;
	data_pointer_y = &Y;

	bFFT_enabled = false;

	connect(&sampleRate_update_timer, SIGNAL(timeout()), this, SLOT(update_sampleRate()));
	sampleRate_update_timer.start(500);

}

///////////////////////////////////////////////////////////////////////////////


MeasureData2D::~MeasureData2D()
{

}

///////////////////////////////////////////////////////////////////////////////

void MeasureData2D::add(qreal new_x, qreal new_y)
{ 
	X.push_back(new_x);
	Y.push_back(new_y*adc_multiplier);
}

///////////////////////////////////////////////////////////////////////////////

// add an array. for now we keep out x values as out indices. ToDo: change this
void MeasureData2D::add(int32_t * arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		add((qreal)size(), (qreal)arr[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////

// clear all saved data
void MeasureData2D::clear()
{
	FFTdisable();

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

	//interpolate_time(0, cnt, microseconds(0), duration_cast<microseconds>(t1 - t0) );
}

///////////////////////////////////////////////////////////////////////////////

// calculates the FFT of the data that is being held
// the original data is being kept. only the display pointers are being switched
void MeasureData2D::FFTransform(int start_index)
{
	if (X.size() > 0)
	{
		// this should normally not happen but if we have an odd number of values we throw awaz the last one
		if (X.size() % 2 != 0)
		{
			X.pop_back();
			Y.pop_back();
		}

		int fft_size = ClosestPowerOf2(X.size()-start_index);

		// prepare data containers
		fft_X.clear();
		fft_Y.clear();
		fft_X.resize(fft_size);
		fft_Y.resize(fft_size);
		transformer.setSize(fft_size);

		double* input = &Y[Y.size()- fft_size];			// we have to transform from std::vector to a simple array
		double* output = &fft_Y[0];						// this works since the c++ 11 stores the vector values as sequntially

		// actual transformation
		transformer.forwardTransform(input, output);

		// calculate absolute value
		for (int i = 0; i < (fft_X.size() / 2); i++)
		{
			fft_X[2 * i] = 2 * i;
			fft_X[2 * i + 1] = 2 * i + 1;

			
			fft_Y[i] = sqrt(
				(fft_Y[i] * fft_Y[i]) +
				(fft_Y[i + fft_X.size() / 2] * fft_Y[i + fft_X.size() / 2])
			);
		}

		fft_Y[0] = 0; // sometimes the transformation glitches out and the first value is way too big
		

		// second half of the array only contains imaginary values so we throw them away
		fft_X.resize(fft_X.size() / 2);
		fft_Y.resize(fft_Y.size() / 2);
	}
}

///////////////////////////////////////////////////////////////////////////////

// find the next smalles power of two by looking for the most significant high bit
int MeasureData2D::ClosestPowerOf2(int n)
{
	if (n < 0)
		return 0;

	for (int i = sizeof(n) * 8 - 1; i>0; i--)
	{
		if ((n >> i) & 1) // look for highes binary 1
		{
			n = (1 << i);	// clear out everything but that bit
			break;
		}
	}
	return n;

}

///////////////////////////////////////////////////////////////////////////////

// swap output so they point to the fft data. added data will still be saved in the background
void MeasureData2D::FFTenable()
{
	data_pointer_x = &fft_X;
	data_pointer_y = &fft_Y;

	bFFT_enabled = true;
}

///////////////////////////////////////////////////////////////////////////////

// swap output data back to time-based data
void MeasureData2D::FFTdisable()
{
	data_pointer_x = &X;
	data_pointer_y = &Y;

	bFFT_enabled = false;
}

// calculates linear average of all Y-datapoints
qreal MeasureData2D::Average()
{
	if (Y.size() == 0)
		return 0;

	qreal avg = 0;
	for (int i = 0; i < Y.size(); i++)
	{
		avg += Y[i];
	}

	return (avg /= (qreal)Y.size());
}

///////////////////////////////////////////////////////////////////////////////

// removes offset by substracting the linear average
void MeasureData2D::Remove_Offset()
{
	qreal avg = Average();
	for (int i = 0; i < Y.size(); i++)
	{
		Y[i] -= avg;
	}
}

///////////////////////////////////////////////////////////////////////////////

// since data comes in bulk and without a time signature our only option is linear interpolation between two timestamps
int MeasureData2D::interpolate_time(int start_index, int end_index, std::chrono::microseconds start_time, std::chrono::microseconds end_time)
{
	// precision is µs but for easier readability we transform to ms
	start_time /= 1000;
	end_time /= 1000;


	if ((end_index <= start_index) || (start_time > end_time))	// check for bad input
		return -1;

	if ((start_index + 1) == end_index)
	{
		//an extra case for only 2 samples is necessary since the algorithm below migth produce NaN otherwise
		X[start_index] = (qreal)start_time.count();
		X[end_index] = (qreal)end_time.count();
		return 0;
	}

	// fill X-axis with new values
	qreal step = (end_time - start_time).count() / (qreal)(end_index - start_index - 1);	
	for (int i = 0; i < (end_index - start_index); i++)
	{
		X[start_index + i] = (qreal)start_time.count() + i*step;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

// return data at a certain index. note that data_pointer may poit to any arbitrary vector (in our case time or frequencz spectrum)
qreal MeasureData2D::x(int index)
{
	if (index > (int)data_pointer_x->size())
		return data_pointer_x->at(data_pointer_x->size());
	else
		return data_pointer_x->at(index);
}

///////////////////////////////////////////////////////////////////////////////

// see above
qreal MeasureData2D::y(int index)
{
	if (index > (int)data_pointer_y->size())
		return data_pointer_y->at(data_pointer_y->size());
	else
		return data_pointer_y->at(index);
}

///////////////////////////////////////////////////////////////////////////////

QString MeasureData2D::xAxisTitle()
{
	if (FFT_isEnabled())
		return FFT_xAxis_title;
	else
		return xAxis_title;
}

///////////////////////////////////////////////////////////////////////////////

QString MeasureData2D::yAxisTitle()
{
	if (FFT_isEnabled())
		return FFT_yAxis_title;
	else
		return yAxis_title;
}

///////////////////////////////////////////////////////////////////////////////

// calculate sample rate by lineary interpolating the amo-unt of added samples between a timespan
qreal MeasureData2D::update_sampleRate()
{
	using namespace chrono;

	if (X.size() == 0)
	{
		added_samples = 0;
		t0 = std::chrono::high_resolution_clock::now();
		return 0;
	}
	else
	{
		added_samples = X.size() - added_samples;

		t1 = std::chrono::high_resolution_clock::now();

		sample_rate = (qreal)(added_samples) / (duration<qreal>(t1 - t0).count());

		//new QListWidgetItem( (QString)("%1").arg(sample_rate) , messageList);
		return sample_rate;
	}
}
