#pragma once
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////////////
//
//				Basic Container for holding 2D Data
//
///////////////////////////////////////////////////////////////////////////////


class MeasureData2D
{
public:
	MeasureData2D();
	~MeasureData2D();

	void add(qreal new_x, qreal new_y);
	void clear();
	void generateTestData(int cnt);

	int interpolate_time(int from_index, int to_index, std::chrono::microseconds start_time, std::chrono::microseconds end_time);
	int size() { return (int) X.size(); }
	
	qreal x(int index);	// return data at index. returns NULL for empty sets
	qreal y(int index);	// if index is too big the last value gets returned


private: 
	std::vector<qreal>  X;
	std::vector<qreal>  Y;
};

