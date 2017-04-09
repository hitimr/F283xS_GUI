#pragma once
#include "stdafx.h"
#include "f2837xsgui.h"
#include "chart.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	F2837xSGUI mainWindow;
	mainWindow.show();

	//.mainWindow.grabGesture(Qt::PanGesture);
	//mainWindow.grabGesture(Qt::PinchGesture);
	

	return a.exec();
	
}
