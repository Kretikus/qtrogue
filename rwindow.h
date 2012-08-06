#include "currentmap.h"

#include <QVector>
#include <QWidget>

class RWidget : public QWidget
{
	Q_OBJECT
public:
	RWidget(QWidget* parent);

protected:
	void paintEvent(QPaintEvent * event);
	void keyReleaseEvent(QKeyEvent *);

	QString    currentText_;
	CurrentMap map_;
};
