#pragma once
#ifndef PAINT_H
#define PAINT_H
//#include"qt.h"
#include <QWidget>
#include <QTime>
#include <QPoint>
#include <QMap>
#include <cmath>
#include <QPainter>
#include <stack>
#include <cstdio>
#include <QStack>
#include "TransparentBridge.h"

enum
{
	UP, DOWN, LEFT, RIGHT
};

class Painter : public QWidget
{
	Q_OBJECT
public:
	explicit Painter (QWidget* parent = 0);
	QTime lastTime;
	const double k = 0;
	int stateCnt;
	void updateLineStateMap ();
	QMap<int, QPoint> pointMap;
	QMap<QPair<int, int>, int>lineStateMap;
	QMap<QPair<int, int>, int>lineStateMap2;

protected:
	void paintEvent (QPaintEvent*);
	void draw (QPainter* p);
	void DrawALineBetween2Point (QPoint p1, int direction1, QPoint p2, int direction2, QPainter* p, int flag, bool newPolyline = false);
	QPolygon makePolyline (QPoint p1, int direction1, QPoint p2, int direction2);
	QPolygon makePolylinewithLambda (QVector<QPoint> pointv, double lam);
	QPoint DefiniteRatioPoint (QPoint sourceP, QPoint targetP, double lam);
	QPolygon makePolylinewithTime (QPoint p1, int direction1, QPoint p2, int direction2, bool newPolyline = false);


private:

};

#endif // PAINT_H