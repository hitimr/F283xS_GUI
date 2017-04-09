#pragma once
#include "stdafx.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	F2837xSGUI mainWindow;
	mainWindow.show();
	

	return a.exec();	
}
