#include "Point2D.h"
#include <math.h>

using namespace std;

#pragma once
class CLine2D
{
public:
	CLine2D(void);
	CLine2D(CPoint2D P1, CPoint2D P2);
	~CLine2D(void);

	CPoint2D p1;
	CPoint2D p2;
};

