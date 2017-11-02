#include <valarray>
#include <vector>
#include "Line2D.h"

using namespace std;

#pragma once
class CImgProcessor
{
public:
	CImgProcessor(void);
	CImgProcessor(unsigned int width, unsigned int height);
	~CImgProcessor(void);

	void thinImage(const valarray<unsigned char> &binaryArray);

	valarray<unsigned char>& getBinaryArrayThinned(void);

private:

	unsigned int imageWidth;
	unsigned int imageHeight;

	valarray<unsigned char> binaryArrayThinned;

	void assignP(unsigned int x, unsigned int y);
	unsigned char calcA(void);
	unsigned char calcB(void);

	void encodeNeighbourhood();
	void removeIsolatedPixel();

	unsigned char P2, P3, P4, P5, P6, P7, P8, P9;
};

