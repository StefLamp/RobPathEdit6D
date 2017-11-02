#include <valarray>
#include <vector>
#include <list>
#include "Point2D.h"

using namespace std;

#pragma once
class CPointSequencer
{
public:
	CPointSequencer(void);
	~CPointSequencer(void);

	void createSegments(const valarray<unsigned char>& binaryArray, unsigned int imageWidth, unsigned int imageHeight);

	vector<list<CPoint2D>>& getSegmentsVector();

private:
	vector<list<CPoint2D>> segments;

	bool findNeighbour(unsigned int x, unsigned int y, valarray<unsigned char>& binaryArray, CPoint2D& p, unsigned int imageWidth, unsigned int imageHeight);

	bool getIndexOfFirstMostLonesomePoint(valarray<unsigned char>& binaryArray, unsigned int& index);
};

