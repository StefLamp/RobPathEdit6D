class CLine2D;

using namespace std;

#pragma once
class CPoint2D
{
public:
	CPoint2D(void);
	CPoint2D(unsigned int x, unsigned int y);
	~CPoint2D(void);

	unsigned int getX();
	unsigned int getY();

	void setX(unsigned int X);
	void setY(unsigned int X);

	double distanceTo(CPoint2D point);
	double distanceTo(CLine2D line);
		 
private:
	unsigned int x;
	unsigned int y;
};

