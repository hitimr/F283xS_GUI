#pragma once
#include "stdafx.h"
#include "f2837xsgui.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	F2837xSGUI mainWindow;
	mainWindow.show();
	

	return a.exec();	
}
