#include <valarray>
#include <vector>
#include <list>
#include "bitmap_image.h"
#include "Point2D.h"

using namespace std;

enum BitmapCanvas {original, gray, binary, working, white};
enum DrawOnBitmapStyle {lineConnect, pixel};

#pragma once
class CBmpHandler
{
public:
	CBmpHandler(void);
	CBmpHandler(string imageFilename);
	~CBmpHandler(void);

	void image2Grayscale(void);
	void image2Binary(unsigned char threshold);

	void saveGrayscaleImage(string imageFilename);
	void saveBinaryImage(string imageFilename);
	void saveWorkingImage(string imageFilename);

	valarray<unsigned char>& getBinaryArray(void);
	unsigned int getWidth(void);
	unsigned int getHeight(void);

	void drawOnBitmap(const valarray<unsigned char>& Array, BitmapCanvas canvas);
	void drawOnBitmap(vector<list<CPoint2D>>& Segments, BitmapCanvas canvas, DrawOnBitmapStyle style);

private:
	bitmap_image originalImage;
	bitmap_image grayscaleImage;
	bitmap_image binaryImage;
	bitmap_image workingImage;
	valarray<unsigned char> binaryArray;
	unsigned int imageWidth; //altern: stride
	unsigned int imageHeight;

	void setWorkingImage(BitmapCanvas canvas);
};

