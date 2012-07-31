#include <QApplication>

#include <rwindow.h>

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	RWidget w(0);
	
	w.show();

	app.exec();
}
