#include "QtOpenGL.h"
#include "MD_Graph.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	QtOpenGL w;
	w.show();
	return a.exec();
}
