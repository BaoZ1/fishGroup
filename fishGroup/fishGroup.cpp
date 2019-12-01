#include "fishGroup.h"
#include <stdlib.h>
#include <QPainter>

fishGroup::fishGroup(QWidget* parent)
	: QWidget(parent)
{
	setFixedSize(800, 800);
	fishes.resize(50);

	eaterNum = 1;

	QPalette palette(this->palette());
	palette.setColor(QPalette::Background, Qt::black);
	this->setPalette(palette);

	srand(time(NULL));

	for (int i = 0; i < fishes.size(); i++)
	{
		fishes[i] = new fish;
		fishes[i]->pos = complex<int>(rand() % 500, rand() % 500);
		fishes[i]->v = complex<int>((rand() - RAND_MAX / 2) % 20, (rand() - RAND_MAX / 2) % 20);
		fishes[i]->a = 0;
		fishes[i]->maxV = ((double)(rand() % 5500)) / 1000.0 + 3.5;
		fishes[i]->maxA = ((double)(rand() % 2500)) / 1000.0 + 2.0;
		fishes[i]->isEater = false;

		if (i < eaterNum)
		{
			fishes[i]->maxV += 2;
			fishes[i]->maxA += 1;
			fishes[i]->isEater = true;
		}
	}

	timer.setParent(this);
	connect(&timer, &QTimer::timeout, [&] {refresh(); });
	timer.start(30);
}

void fishGroup::paintEvent(QPaintEvent*)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing, true);

	p.setPen(QColor(Qt::red));
	p.setBrush(QColor(Qt::red));

	for (int i = 0; i < eaterNum; i++)
	{
		p.drawEllipse(QPoint(fishes[i]->pos.real(), fishes[i]->pos.imag()), 7, 7);
	}

	p.setPen(QColor(Qt::white));
	p.setBrush(QColor(Qt::white));

	for (int i = eaterNum; i < fishes.size(); i++)
	{
		p.drawEllipse(QPoint(fishes[i]->pos.real(), fishes[i]->pos.imag()), 5, 5);
	}
}

void fishGroup::refresh()
{
	for (int i = 0; i < fishes.size(); i++)
	{
		fish* thisFish = fishes[i];
		thisFish->pos += thisFish->v;
		thisFish->v += thisFish->a;
		//thisFish->a = 0;
		for (int j = 0; j < 50; j++)
		{
			if (j == i)
			{
				continue;
			}

			fish* thatFish = fishes[j];

			complex<int> distance = thatFish->pos - thisFish->pos;
			int distanceSquare = pow(distance.real(), 2) + pow(distance.imag(), 2);

			if (thisFish->isEater == false)//普通鱼
			{
				if (distanceSquare <= 40000)//普通鱼感知距离
				{
					if (thatFish->isEater == false)//对方也是普通鱼
					{
						thisFish->a += {distance.real() / 3000.0, distance.imag() / 3000.0};
					}
					else//看见个大鱼
					{
						thisFish->a -= {distance.real() * 100.0, distance.imag() * 100.0};
					}
				}
				if (distanceSquare <= 400)//离得太近
				{
					thisFish->a -= {distance.real() / 300.0, distance.imag() / 300.0};
				}
			}
			else//是大鱼
			{
				if (distanceSquare <= 100000)//超大视野，看谁逮谁
				{
					thisFish->a += {distance.real() / 200.0, distance.imag() / 200.0};
				}
			}
		}

		thisFish->pos.real(thisFish->pos.real() > this->width() ? 0 : thisFish->pos.real());
		thisFish->pos.real(thisFish->pos.real() < 0 ? this->width() : thisFish->pos.real());
		thisFish->pos.imag(thisFish->pos.imag() > this->height() ? 0 : thisFish->pos.imag());
		thisFish->pos.imag(thisFish->pos.imag() < 0 ? this->height() : thisFish->pos.imag());

		thisFish->v.real(thisFish->v.real() > thisFish->maxV ? thisFish->maxV : thisFish->v.real());
		thisFish->v.real(thisFish->v.real() < -thisFish->maxV ? -thisFish->maxV : thisFish->v.real());
		thisFish->v.imag(thisFish->v.imag() > thisFish->maxV ? thisFish->maxV : thisFish->v.imag());
		thisFish->v.imag(thisFish->v.imag() < -thisFish->maxV ? -thisFish->maxV : thisFish->v.imag());

		thisFish->a.real(thisFish->a.real() > thisFish->maxA ? thisFish->maxA : thisFish->a.real());
		thisFish->a.real(thisFish->a.real() < -thisFish->maxA ? -thisFish->maxA : thisFish->a.real());
		thisFish->a.imag(thisFish->a.imag() > thisFish->maxA ? thisFish->maxA : thisFish->a.imag());
		thisFish->a.imag(thisFish->a.imag() < -thisFish->maxA ? -thisFish->maxA : thisFish->a.imag());
	}

	repaint();
}