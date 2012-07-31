#include <QVector>
#include <QWidget>

class CurrentMap
{
public:
	CurrentMap();

	int getMaxX() const;
	int getMaxY() const;
	QChar getAt(int x, int y) const;
	QChar getAt(QPoint const& pos) const;
	
	void setData(int x, int y, const char& c);
	void setData(QPoint const& pos, const char& c);

	QVector<QVector<QChar> > data_;
	QPoint offset_;
};

class Character
{
public:
	Character();
	QChar getSymbol() const;

	void draw(QPainter& p) const;

	QPoint pos_;
};

class RWidget : public QWidget
{
	Q_OBJECT
public:
	RWidget(QWidget* parent);

protected:
	void paintEvent(QPaintEvent * event);
	void keyReleaseEvent(QKeyEvent *);

	Character player_;
	CurrentMap map_;
};
