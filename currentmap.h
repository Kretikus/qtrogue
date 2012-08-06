#pragma once

#include <QChar>
#include <QPoint>
#include <QVector>

class CurrentMap;
class QPainter;

class Character
{
public:
	enum Move {
		Up,
		Down,
		Left,
		Right
	};

	Character(CurrentMap & map);
	virtual ~Character();
	virtual QChar getSymbol() const;

	// retirns false if move was not valid
	bool move(Move mv);

	void draw(QPainter& p) const;

	void setPos(const QPoint & p) { pos_ = p; }

protected:
	CurrentMap & map_;
	QPoint pos_;
};

class Monster : public Character
{
public:
	Monster(CurrentMap & map) : Character(map) {}

public:
	virtual QChar getSymbol() const;
	
};

class CurrentMap
{
public:
	CurrentMap();
	~CurrentMap();

	int getMaxX() const;
	int getMaxY() const;

	QChar getWindowAt(int x, int y) const;
	QChar getWindowAt(QPoint const& pos) const;

	QChar getGlobalAt(QPoint const& pos) const;

	void setData(int x, int y, const char& c);
	void setData(QPoint const& pos, const char& c);

	void draw(QPainter& p) const;

	QVector<QVector<QChar> > data_;
	QPoint offset_;

	Character character_;
	QVector<Monster*> monsters_;
};
