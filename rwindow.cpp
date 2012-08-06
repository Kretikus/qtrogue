#include "rwindow.h"
#include "perlinnoise.h"

#include <QApplication>
#include <QDateTime>
#include <QPainter>
#include <QKeyEvent>

#include <QDebug>

#include <time.h>

namespace {
	class ScopedUpdate {
		public:
			ScopedUpdate(QWidget* w) : w_(w) {}
			~ScopedUpdate() { w_->update(); }
		private:
			QWidget* w_;
	};
}

RWidget::RWidget(QWidget *parent)
: QWidget(parent)
{
	resize((map_.getMaxX()+1)*12, (map_.getMaxY()+1)*12);
}

void RWidget::paintEvent(QPaintEvent * event)
{
	QPainter p(this);

	p.save();
	p.setBrush(Qt::white);
	p.drawRect(event->rect());

	p.restore();
	map_.draw(p);

	if (!currentText_.isEmpty()) {
		QFont f = font();
		f.setBold(true);
		p.setFont(f);
		p.drawText(12, 12, currentText_);
	}
}

void RWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
	bool validMove = true;
	ScopedUpdate scopedU(this); Q_UNUSED(scopedU);
	switch(keyEvent->key()) {
		case Qt::Key_Left:
			validMove = map_.character_.move(Character::Left);
			break;
		case Qt::Key_Right:
			validMove = map_.character_.move(Character::Right);
			break;
		case Qt::Key_Up:
			validMove = map_.character_.move(Character::Up);
			break;
		case Qt::Key_Down:
			validMove = map_.character_.move(Character::Down);
			break;
		case Qt::Key_Escape:
			qApp->exit();
			break;
	}
	currentText_ = !validMove ? "BOINK!" : "";
}
