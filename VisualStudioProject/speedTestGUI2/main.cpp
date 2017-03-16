#include "stdafx.h"
#include "speedtestgui.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	speedTestGUI w;
	w.show();
	return a.exec();
}
