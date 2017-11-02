#include "PointSequencer.h"

CPointSequencer::CPointSequencer(void)
{
}


CPointSequencer::~CPointSequencer(void)
{
}

void CPointSequencer::createSegments(const valarray<unsigned char>& binaryArrayThinned, unsigned int imageWidth, unsigned int imageHeight)
{
	CPoint2D p;

	valarray<unsigned char> binaryArrayCopy = binaryArrayThinned;

	bool finished = true;
	//bool restart;
	int segmentCount = -1;
	unsigned int startIndex = 0;

	unsigned int x;
	unsigned int y;

	while (getIndexOfFirstMostLonesomePoint(binaryArrayCopy, startIndex) == true)
	{
		y = startIndex / imageWidth;
		x = startIndex % imageWidth;

		segmentCount++;
		segments.push_back(list<CPoint2D>()); // add new segment (i.e., new list) to vector of segments

		p.setX(x);
		p.setY(y);

		do	
		{
			segments[segmentCount].push_back(p); //add new neighbour

			binaryArrayCopy[y*imageWidth+x] = 0;//delete neighbour

			x = p.getX();//set x and y to neighbour coordinates
			y = p.getY();
		} 
		while (findNeighbour(x,y,binaryArrayCopy, p, imageWidth, imageHeight)); //find neighbour until no neighbour left

		if (segments[segmentCount].size() < 2)
		{
			segments.pop_back();
			segmentCount--;
		}
	}
}

vector<list<CPoint2D>>& CPointSequencer::getSegmentsVector()
{
	return segments;
}


bool CPointSequencer::findNeighbour(unsigned int x, unsigned int y, valarray<unsigned char>& binaryArray,CPoint2D& p, unsigned int imageWidth, unsigned int imageHeight)
{
	
	if (y > 0)
	{
		if (binaryArray[(y - 1) * imageWidth + x] > 0)//up
		{
			p.setX(x);
			p.setY(y - 1);
			return true;
		}
	}

	if (x < imageWidth - 1)
	{
		if (binaryArray[y * imageWidth + x + 1] > 0)//right
		{
			p.setX(x + 1);
			p.setY(y);
			return true;
		}
	}

	if (y < imageHeight - 1)
	{
		if (binaryArray[(y + 1) * imageWidth + x] > 0)//down
		{
			p.setX(x);
			p.setY(y + 1);
			return true;
		}
	}

	if (x > 0)
	{
		if (binaryArray[y * imageWidth + x - 1] > 0)//left
		{
			p.setX(x - 1);
			p.setY(y);
			return true;
		}
	}

	if (y > 0 && x < imageWidth - 1)
	{
		if (binaryArray[(y - 1) * imageWidth + x + 1] > 0)//upper right
		{
			p.setX(x + 1);
			p.setY(y - 1);
			return true;
		}
	}

	if (y < imageHeight - 1 && x < imageWidth - 1)
	{
		if (binaryArray[(y + 1) * imageWidth + x + 1] > 0)//lower right
		{
			p.setX(x + 1);
			p.setY(y + 1);
			return true;
		}
	}

	if (y < imageHeight - 1 && x > 0)
	{
		if (binaryArray[(y + 1) * imageWidth + x - 1] > 0)//lower left
		{
			p.setX(x - 1);
			p.setY(y + 1);
			return true;
		}
	}

	if (y > 0 && x > 0)
	{
		if (binaryArray[(y - 1)* imageWidth + x - 1] > 0)//upper left
		{
			p.setX(x - 1);
			p.setY(y - 1);
			return true;
		}
	}

	return false;
}

bool CPointSequencer::getIndexOfFirstMostLonesomePoint(valarray<unsigned char>& binaryArray, unsigned int& index)
{
	unsigned char min = 9;

	//assumes that valarray contains numbers <9

	if (binaryArray.max() == 0)
	{
		return false;
	}

	//search thru valarray so that minimum > 0
	// return false if minimum == 0

	for (int i = 0; i < binaryArray.size(); i++)
	{
		if (binaryArray[i] > 0 && binaryArray[i] < min)
		{
			min = binaryArray[i];
			index = i;		
		}
	}
	
	return true;
}