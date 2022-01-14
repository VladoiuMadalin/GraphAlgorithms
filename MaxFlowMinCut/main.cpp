#include "MaxFlowMinCut.h"
#include <QtWidgets/QApplication>
#include "Graph.h"
#include <iostream>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MaxFlowMinCut w;
	w.show();
	return a.exec();
}
