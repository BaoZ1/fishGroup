#pragma once

#include <QtWidgets/QWidget>
#include <vector>
#include <complex>
#include <QTimer>

using std::vector;
using std::complex;

struct fish
{
	complex<double>v, a;
	complex<int>pos;
	vector<complex<int> >path;

	double maxV, maxA;

	bool isEater;
};

class fishGroup : public QWidget
{
	Q_OBJECT

public:
	fishGroup(QWidget* parent = Q_NULLPTR);
	void paintEvent(QPaintEvent*);
	void refresh();

	vector<fish*>fishes;
	int eaterNum;
	QTimer timer;
};
