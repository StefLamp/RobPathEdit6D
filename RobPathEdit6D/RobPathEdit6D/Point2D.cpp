#include "Point2D.h"
#include "Line2D.h"
#include <cmath>

CPoint2D::CPoint2D(void)
{
}

CPoint2D::CPoint2D(unsigned int X, unsigned int Y)
{
	x = X;
	y = Y;
}
	

CPoint2D::~CPoint2D(void)
{
}

unsigned int CPoint2D::getX(void)
{
	return x;
}

unsigned int CPoint2D::getY(void)
{
	return y;
}

void CPoint2D::setX(unsigned int X)
{
	x = X;
}

void CPoint2D::setY(unsigned int Y)
{
	y = Y;
}

double CPoint2D::distanceTo(CPoint2D point)
{
	return sqrt(pow((double)(x - (double)point.getX()), 2) + pow((double)(y - (double)point.getY()), 2));
}

double CPoint2D::distanceTo(CLine2D line)
{
	double bx,by,rv_sq,dist;

	int rvx=line.p1.x-line.p2.x;
	int rvy=line.p1.y-line.p2.y;
	
	rv_sq = (rvx*rvx) + (rvy*rvy);

	bx = x - (double)line.p1.x;
	by = y - (double)line.p1.y;

	dist = abs((bx*rvy-by*rvx))/sqrt(rv_sq);

	return dist;
}

