#include <QMessageBox>
#include <QtWidgets/QMainWindow>
#include <QDebug>
#include "painter.h"



Painter::Painter (QWidget* parent) : QWidget (parent)
{
	const int top = 0;
	const int bottom = 200;
	const int column1 = 50;
	const int column2 = 290;
	const int column3 = 315;
	const int column4 = 560;
	const int TBcolumn1 = 130;
	const int TBcolumn2 = 210;
	const int TBcolumn3 = 390;
	const int TBcolumn4 = 470;
	const int TBrow1 = 90;
	const int TBrow2 = 110;

	using LinkProtocal::PCv;
	using LinkProtocal::messagePoll;
	using LinkProtocal::hasMessageBetweenPorts;

	using PortPoint = QPair<int, QPoint>;
	PortPoint pp1{ 1001,{column1,top } };
	PortPoint pp2{ 1002,{column1,bottom } };
	PortPoint pp3{ 1003,{column2,top } };
	PortPoint pp4{ 1004,{column3,bottom } };
	PortPoint pp5{ 1005,{column4,top } };
	PortPoint pp6{ 1006,{column4,bottom } };
	PortPoint ppTB1_1{ 1,{TBcolumn1,TBrow1 } };
	PortPoint ppTB1_2{ 2,{TBcolumn1,TBrow2 } };
	PortPoint ppTB1_3{ 3,{TBcolumn2,TBrow1 } };
	PortPoint ppTB1_4{ 4,{TBcolumn2,TBrow2 } };
	PortPoint ppTB2_1{ 5,{TBcolumn3,TBrow1 } };
	PortPoint ppTB2_2{ 6,{TBcolumn3,TBrow2 } };
	PortPoint ppTB2_3{ 7,{TBcolumn4,TBrow1 } };
	PortPoint ppTB2_4{ 8,{TBcolumn4,TBrow2 } };

	pointMap.insert (pp1.first, pp1.second);
	pointMap.insert (pp2.first, pp2.second);
	pointMap.insert (pp3.first, pp3.second);
	pointMap.insert (pp4.first, pp4.second);
	pointMap.insert (pp5.first, pp5.second);
	pointMap.insert (pp6.first, pp6.second);
	pointMap.insert (ppTB1_1.first, ppTB1_1.second);
	pointMap.insert (ppTB1_2.first, ppTB1_2.second);
	pointMap.insert (ppTB1_3.first, ppTB1_3.second);
	pointMap.insert (ppTB1_4.first, ppTB1_4.second);
	pointMap.insert (ppTB2_1.first, ppTB2_1.second);
	pointMap.insert (ppTB2_2.first, ppTB2_2.second);
	pointMap.insert (ppTB2_3.first, ppTB2_3.second);
	pointMap.insert (ppTB2_4.first, ppTB2_4.second);
	lineStateMap.insert ({ 1001,1 }, 0);
	lineStateMap.insert ({ 1002,2 }, 0);
	lineStateMap.insert ({ 1003,3 }, 0);
	lineStateMap.insert ({ 1004,6 }, 0);
	lineStateMap.insert ({ 1005,7 }, 0);
	lineStateMap.insert ({ 1006,8 }, 0);
	lineStateMap.insert ({ 1004,5 }, 0);

	resize (600, 400);
	stateCnt = 0;
}

void Painter::draw (QPainter* p)
{

	//lineStateMap2 = lineStateMap;
	//updateLineStateMap ();
	////bool newPolyline = stateCnt == 0;
	//QMap<QPair<int, int>, bool> changedLine;
	//int cnt = lineStateMap.size ();


	//for (auto iter = lineStateMap.keyBegin (); cnt>0; ++iter,--cnt) {
	//	if (lineStateMap[{iter->first, iter->second}] != lineStateMap2[{iter->first, iter->second}]) {
	//		changedLine[{iter->first, iter->second}] = true;
	//	}
	//}

	//DrawALineBetween2Point (pointMap[1001], DOWN, pointMap[1], LEFT, p, lineStateMap[{1001, 1}], changedLine[{1001, 1}]);
	//DrawALineBetween2Point (pointMap[1002], UP, pointMap[2], LEFT, p, lineStateMap[{1002, 2}], changedLine[{1002, 2}]);
	//DrawALineBetween2Point (pointMap[1003], DOWN, pointMap[3], RIGHT, p, lineStateMap[{1003, 3}], changedLine[{1003, 3}]);
	//DrawALineBetween2Point (pointMap[1004], UP, pointMap[6], LEFT, p, lineStateMap[{1004, 6}], changedLine[{1004, 6}]);
	//DrawALineBetween2Point (pointMap[1005], DOWN, pointMap[7], RIGHT, p, lineStateMap[{1005, 7}], changedLine[{1005, 7}]);
	//DrawALineBetween2Point (pointMap[1006], UP, pointMap[8], RIGHT, p, lineStateMap[{1006, 8}], changedLine[{1006, 8}]);
	//DrawALineBetween2Point (pointMap[4], RIGHT, pointMap[5], LEFT, p, lineStateMap[{4, 5}], changedLine[{4, 5}]);

	DrawALineBetween2Point (pointMap[1001], DOWN, pointMap[1], LEFT, p, lineStateMap[{1001, 1}]);
	DrawALineBetween2Point (pointMap[1002], UP, pointMap[2], LEFT, p, lineStateMap[{1002, 2}]);
	DrawALineBetween2Point (pointMap[1003], DOWN, pointMap[3], RIGHT, p, lineStateMap[{1003, 3}]);
	DrawALineBetween2Point (pointMap[1004], UP, pointMap[6], LEFT, p, lineStateMap[{1004, 6}]);
	DrawALineBetween2Point (pointMap[1005], DOWN, pointMap[7], RIGHT, p, lineStateMap[{1005, 7}]);
	DrawALineBetween2Point (pointMap[1006], UP, pointMap[8], RIGHT, p, lineStateMap[{1006, 8}]);
	DrawALineBetween2Point (pointMap[4], RIGHT, pointMap[5], LEFT, p, lineStateMap[{4, 5}]);

	const int dx = 5;
	p->setPen (QPen ());
	p->drawText (pointMap[1].x () - 10, pointMap[1].y () - 3, "1");
	p->drawText (pointMap[2].x () - 10, pointMap[2].y () + 12, "2");
	p->drawText (pointMap[3].x () + 5, pointMap[3].y () - 3, "3");
	p->drawText (pointMap[4].x () + 5, pointMap[4].y () + 12, "4");
	p->drawText (pointMap[5].x () - 10, pointMap[5].y () - 3, "1");
	p->drawText (pointMap[6].x () - 10, pointMap[6].y () + 12, "2");
	p->drawText (pointMap[7].x () + 5, pointMap[7].y () - 3, "3");
	p->drawText (pointMap[8].x () + 5, pointMap[8].y () + 12, "4");
}



// 在两点之间画线，state不为0则用彩线
void Painter::DrawALineBetween2Point (QPoint p1, int direction1, QPoint p2, int direction2, QPainter* p, int state, bool newPolyline)
{
	if (state == 1)
		p->setPen (QPen (Qt::red, 5));
	else if (state == 2)
		p->setPen (QPen (Qt::green, 5));
	else if (state == 3)
		p->setPen (QPen (Qt::blue, 5));
	else
		p->setPen (QPen ());
	p->drawPolyline (makePolylinewithTime (p1, direction1, p2, direction2, newPolyline));
}

QPolygon Painter::makePolyline (QPoint p1, int direction1, QPoint p2, int direction2)
{
	if ((direction1 == UP || direction1 == DOWN) && (direction2 == LEFT || direction2 == RIGHT)) {
		QPoint xp{ p1.x (),p2.y () };
		return QVector<QPoint>{ p1, xp, p2 };
	}
	else if (direction1 == LEFT && direction2 == RIGHT && p1.y () != p2.y ()) {
		QPoint xp1{ (p1.x () + p2.x ()) / 2,p1.y () };
		QPoint xp2{ (p1.x () + p2.x ()) / 2,p2.y () };
		return QVector<QPoint>{ p1, xp1, xp2, p2 };
	}
	else {
		return makePolyline (p2, direction2, p1, direction1);
	}
}

constexpr double distanceBetweenTwoPoints (const QPoint& p1, const QPoint& p2)
{
	if (p1.x () == p2.x ())
		return abs (p1.y () - p2.y ());
	else if (p1.y () == p2.y ())
		return abs (p1.x () - p2.x ());
	return sqrt ((p1.x () - p2.x ()) * (p1.x () - p2.x ())
		+ (p1.y () - p2.y ()) * (p1.y () - p2.y ()));
}

// 根据点集中的点和比例构造线段
QPolygon Painter::makePolylinewithLambda (QVector<QPoint> pointv, double lam)
{
	QPolygon polygen;
	polygen.push_back (pointv[0]);
	QVector<double> distancev;
	double distance{};
	auto iteri = pointv.cbegin ();
	for (auto iterj = pointv.cbegin () + 1; iterj != pointv.cend ();) {
		distancev.push_back (distanceBetweenTwoPoints (*iteri, *iterj));
		distance += distancev.back ();
		++iteri;
		++iterj;
	}
	double realdis = distance * lam;

	double tmpdis{};
	for (size_t i = 0; i < distancev.size (); i++) {
		if ((tmpdis + distancev[i]) > realdis) {
			// 超过实际长度，当前线段被截短
			// 截短比例
			double _lam = (realdis - tmpdis) / distancev[i];
			polygen.push_back (DefiniteRatioPoint (pointv[i], pointv[i + 1], _lam));
			break;
		}
		else {
			polygen.push_back (pointv[i + 1]);
		}

	}

	return polygen;

}

QPoint Painter::DefiniteRatioPoint (QPoint sourceP, QPoint targetP, double lam)
{
	QPoint pp{ static_cast<int>((sourceP.x () + lam * targetP.x ()) / (1 + lam)),static_cast<int>((sourceP.y () + lam * targetP.y ()) / (1 + lam)) };
	return pp;
}


QPolygon Painter::makePolylinewithTime (QPoint sourceP, int direction1, QPoint targetP, int direction2, bool newPolyline)
{
	QPolygon polygen;
	// 求出时间差
	if (newPolyline) {
		lastTime = QTime::currentTime ();
		qDebug () << "newPolyline" << endl;
	}
	double dt = lastTime.msecsTo (QTime::currentTime ());
	if ((direction1 == UP || direction1 == DOWN) && (direction2 == LEFT || direction2 == RIGHT)) {
		QPoint xp{ sourceP.x (),targetP.y () };

		if (dt > k) {
			// 超过动画时间
			polygen.push_back (sourceP);
			polygen.push_back (xp);
			polygen.push_back (targetP);
			return polygen;
		}
		else {
			// 动画时间之内
			double lam = dt / k;
			QVector<QPoint> pointv{ sourceP,xp,targetP };
			return makePolylinewithLambda (pointv, lam);
		}
	}
	else if (direction1 == RIGHT && direction2 == LEFT && sourceP.y () != targetP.y ()) {
		QPoint xp1{ (sourceP.x () + targetP.x ()) / 2,sourceP.y () };
		QPoint xp2{ (sourceP.x () + targetP.x ()) / 2,targetP.y () };
		// 根据系统时间动态更新linev
		if (dt > k) {
			// 超过动画时间
			polygen.push_back (sourceP);
			polygen.push_back (xp1);
			polygen.push_back (xp2);
			polygen.push_back (targetP);
			return polygen;
		}
		else {
			double lam = dt / k;
			QVector<QPoint> pointv{ sourceP,xp1,xp2,targetP };
			return makePolylinewithLambda (pointv, lam);
		}
	}
	else {
		return makePolyline (targetP, direction2, sourceP, direction1);
	}
}

void Painter::updateLineStateMap ()
{
	using LinkProtocal::PCv;
	using LinkProtocal::messagePoll;
	using LinkProtocal::hasMessageBetweenPorts;

	if (stateCnt == 1) {
		for (auto iter = lineStateMap.keyBegin (); iter != lineStateMap.keyEnd (); ++iter) {
			if (hasMessageBetweenPorts (iter->first, iter->second)) {
				lineStateMap[*iter] = 1;
			}
		}
	}

	if (stateCnt > 1) {
		for (auto iter = lineStateMap.keyBegin (); iter != lineStateMap.keyEnd (); ++iter) {
			if (hasMessageBetweenPorts (iter->first, iter->second)) {
				lineStateMap[*iter] = 1;
			}
			else if (lineStateMap[*iter]) {
				lineStateMap[*iter]++;
			}
		}
	}

	if (stateCnt > 4) {
		for (auto iter = lineStateMap.keyBegin (); iter != lineStateMap.keyEnd (); ++iter) {
			lineStateMap[*iter] = 0;
		}
	}

}

void Painter::paintEvent (QPaintEvent* e)
{
	QPainter p (this);
	draw (&p);
}
