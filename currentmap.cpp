#include "currentmap.h"
#include "perlinnoise.h"

#include <QDateTime>
#include <QPainter>
#include <QDebug>

namespace {
	const int maxYGlobal = 150;
	const int maxXGlobal = 240;

	const int middleY = 75;
	const int middleX = 120;

	const int maxYWindow = 50;
	const int maxXWindow = 80;

	const int charYSz = 12;
	const int charXSz = 12;

	const QPoint middlePos(maxXGlobal/2, maxYGlobal/2);

	const int maxXOffset = maxXGlobal-maxXWindow;
	const int maxYOffset = maxYGlobal-maxYWindow;

	void capCharacterPos(QPoint & p)
	{
		if(p.x() < 1) p.setX(1);
		if(p.y() < 1) p.setY(1);
		if(p.x() > maxXGlobal-2) p.setX(maxXGlobal-2);
		if(p.y() > maxYGlobal-2) p.setY(maxYGlobal-2);
	}
	
	void capMapOffset(QPoint & offset)
	{
		if(offset.x() < 0) offset.setX(0);
		if(offset.y() < 0) offset.setY(0);
		if(offset.x() > maxXOffset) offset.setX(maxXOffset);
		if(offset.y() > maxYOffset) offset.setY(maxYOffset);
	}
}

Character::Character(CurrentMap & map)
: map_(map), pos_(middlePos)
{
}

Character::~Character()
{
}

QChar Character::getSymbol() const
{
	return '@';
}

bool Character::move(Character::Move mv)
{
	bool xChanged = false;
	QPoint diff;
	switch(mv) {
		case Left:
			xChanged = true;
			diff = QPoint(-1,0);
			break;
		case Right:
			xChanged = true;
			diff = QPoint(1,0);
			break;
		case Up:
			diff = QPoint(0,-1);
			break;
		case Down:
			diff = QPoint(0,1);
			break;
	};
	QPoint newPos = pos_ + diff;

	if(map_.getGlobalAt(newPos) != '.') return false;

	if (newPos.x() >= 0 && newPos.y() >= 0 
		&& newPos.x() <= maxXGlobal && newPos.y() < maxYGlobal)
	{
		pos_ = newPos;
		capCharacterPos(pos_);

		if ( ( xChanged && (pos_.x() >= (maxXWindow/2) && (pos_.x() <= (maxXOffset + (maxXWindow/2))))) ||
		     (!xChanged && (pos_.y() >= (maxYWindow/2) && (pos_.y() <= (maxYOffset + (maxYWindow/2))))) )
		{
			map_.offset_ += diff;
			capMapOffset(map_.offset_);
		}
		return true;
	}
	return false;
}

void Character::draw(QPainter & p) const
{
	if (pos_.x() > map_.offset_.x() && pos_.y() > map_.offset_.y()) {
		QPoint pos = pos_ - map_.offset_;
		p.drawText(charXSz*(pos.x()+1), charYSz*(pos.y()+2), getSymbol());
	}
}

QChar Monster::getSymbol() const
{
	return 'M';
}

CurrentMap::CurrentMap()
: offset_(80, 50), character_(*this)
{
	monsters_.push_back(new Monster(*this));
	monsters_[0]->setPos(QPoint(81,51));

	const long seed = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000;
	PerlinNoise pn(seed); //237

	data_.resize(maxYGlobal);
	for(int i = 0; i < maxYGlobal; ++i) {
		data_[i].resize(maxXGlobal);
		for (int j = 0; j < maxXGlobal; ++j) {

			double x = (double)j/((double) maxXGlobal);
			double y = (double)i/((double) maxYGlobal);

			double n = pn.noise(10 * x, 10 * y, 0.8) + 0.7;

			// Watter (or a Lakes)
			if(n < 0.31) {
				data_[i][j] = '~';
			}
			// Floors (or Planes)
			else if (n >= 0.31 && n < 0.98) {
				data_[i][j] = '.';
			}
			// Walls (or Mountains)
			else if (n >= 0.98 && n < 1.2) {
				data_[i][j] = '#';
			}
			// Ice (or Snow)
			else {
				data_[i][j] = 'S';
			}
		}
	}

	for(int i = 0; i < maxXGlobal; ++i) {
		data_[0][i] = '|';
		data_[maxYGlobal-1][i] = '|';
	}

	for(int i = 0; i < maxYGlobal; ++i) {
		data_[i][0] = '-';
		data_[i][maxXGlobal-1] = '-';
	}
	
}

CurrentMap::~CurrentMap()
{
	for(int i = 0; i < monsters_.size(); ++i) {
		delete monsters_[i];
	}
}

int CurrentMap::getMaxX() const
{
	return maxXWindow;
}


int CurrentMap::getMaxY() const
{
	return maxYWindow;
}

void CurrentMap::setData(int x, int y, const char & c)
{
	data_[offset_.y() + y][offset_.x() + x] = c;
}

void CurrentMap::setData(const QPoint & pos, const char & c)
{
	setData(pos.x(), pos.y(), c);
}

void CurrentMap::draw(QPainter & p) const
{
	for (int y = 0; y < maxYWindow; ++y) {
		for (int x = 0; x < maxXWindow; ++x) {
			p.drawText(12*(x+1), 12*(y+2), QString(getWindowAt(x,y)));
		}
	}

	Q_FOREACH(Monster* m, monsters_) {
		m->draw(p);
	}
	
	character_.draw(p);
}

QChar CurrentMap::getWindowAt(int x, int y) const
{
	return data_[offset_.y()+y][offset_.x() + x];
}

QChar CurrentMap::getWindowAt(const QPoint & pos) const
{
	return getWindowAt(pos.x(), pos.y());
}

QChar CurrentMap::getGlobalAt(const QPoint & pos) const
{
	return data_[pos.y()][pos.x()];
}
