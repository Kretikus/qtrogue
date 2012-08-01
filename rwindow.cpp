#include "rwindow.h"
#include "perlinnoise.h"

#include <QApplication>
#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>

#include <QDebug>

#include <time.h>

namespace {
	const int maxYGlobal = 150;
	const int maxXGlobal = 240;

	const int middleY = 75;
	const int middleX = 120;

	const int maxYWindow = 50;
	const int maxXWindow = 80;

	const int charYSz = 12;
	const int charXSz = 12;

	const QPoint middlePos(maxXWindow/2, maxYWindow/2);
}


CurrentMap::CurrentMap()
: offset_(80, 50)
{
	const long seed = QDateTime::currentDateTime().toMSecsSinceEpoch() / 1000;
	PerlinNoise pn(seed); //237

	data_.resize(maxYGlobal);
	for(int i = 0; i < maxYGlobal; ++i) {
		data_[i].resize(maxXGlobal);
		for (int j = 0; j < maxXGlobal; ++j) {
//			if (x < middleX) data_[i][x] = i < middleY ? '0' : '1';
//			else data_[i][x] = i < middleY ? '2' : '3';

			double x = (double)j/((double) maxXGlobal);
			double y = (double)i/((double) maxYGlobal);

			double n = pn.noise(10 * x, 10 * y, 0.8) + 0.7;

			// Watter (or a Lakes)
			if(n < 0.35) {
				data_[i][j] = '~';
			}
			// Floors (or Planes)
			else if (n >= 0.35 && n < 0.9) {
				data_[i][j] = '.';
			}
			// Walls (or Mountains)
			else if (n >= 0.9 && n < 1.2) {
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

QChar CurrentMap::getAt(int x, int y) const
{
	return data_[offset_.y()+y][offset_.x() + x];
}

QChar CurrentMap::getAt(const QPoint & pos) const
{
	return getAt(pos.x(), pos.y());
}


Character::Character()
: pos_(middlePos)
{
}

QChar Character::getSymbol() const
{
	return '@';
}

void Character::draw(QPainter & p) const
{
	p.drawText(charXSz*pos_.x(), charYSz*pos_.y(), getSymbol());
}


RWidget::RWidget(QWidget *parent)
: QWidget(parent)
{
	resize((maxXWindow+1)*12, (maxYWindow+1)*12);
}

void RWidget::paintEvent(QPaintEvent * /*event*/)
{
	QPainter p(this);
	for (int y = 0; y < maxYWindow; ++y) {
		for (int x = 0; x < maxXWindow; ++x) {
			p.drawText(12*(x+1), 12*(y+1), QString(map_.getAt(x,y)));
		}
	}

	player_.draw(p);
	//qDebug() << player_.pos_ << map_.getAt(player_.pos_-QPoint(1,1));
}

void RWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
	switch(keyEvent->key()) {
		case Qt::Key_Left:
			if(!isMoveValid(player_.pos_ + QPoint(-1,0))) return;

			if (map_.offset_.x() && player_.pos_.x() <= middlePos.x()) {
				map_.offset_ -= QPoint(1,0);
			} else {
				if (player_.pos_.x() > 2) player_.pos_ -= QPoint(1,0);
			}
			break;
		case Qt::Key_Right:
			if(!isMoveValid(player_.pos_ + QPoint(1,0))) return;

			if (map_.offset_.x() < maxXGlobal-maxXWindow && player_.pos_.x() >= middlePos.x()) {
				map_.offset_ += QPoint(1,0);
			} else {
				if (player_.pos_.x() < maxXWindow-1) player_.pos_ += QPoint(1,0);
			}
			break;
		case Qt::Key_Up:
			if(!isMoveValid(player_.pos_ + QPoint(0,-1))) return;

			if (map_.offset_.y() && player_.pos_.y() <= middlePos.y()) {
				map_.offset_ -= QPoint(0,1);
			} else {
				if (player_.pos_.y() > 2) player_.pos_ -= QPoint(0,1);
			}
			break;
		case Qt::Key_Down:
			if(!isMoveValid(player_.pos_ + QPoint(0,1))) return;

			if (map_.offset_.y() < maxYGlobal-maxYWindow && player_.pos_.y() >= middlePos.y()) {
				map_.offset_ += QPoint(0,1);
			} else {
				if (player_.pos_.y() < maxYWindow-1) player_.pos_ += QPoint(0,1);
			}
			break;
		case Qt::Key_Escape:
			qApp->exit();
			break;
	}
	update();
}

bool RWidget::isMoveValid(const QPoint & p)
{
	return map_.getAt(p-QPoint(1,1)) == '.';
}





