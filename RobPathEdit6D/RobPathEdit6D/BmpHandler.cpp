#include "BmpHandler.h"

CBmpHandler::CBmpHandler(void)
{
}

CBmpHandler::CBmpHandler(string imageFilename)
{
	originalImage = bitmap_image(imageFilename);
	imageWidth = originalImage.width();
	imageHeight = originalImage.height();
	grayscaleImage = originalImage;
}

CBmpHandler::~CBmpHandler(void)
{
}

void CBmpHandler::image2Grayscale(void)
{
	grayscaleImage.convert_to_grayscale();
}

void CBmpHandler::image2Binary(unsigned char threshold)
{
	binaryArray = valarray<unsigned char>(imageWidth * imageHeight);
	// altern: binaryArray.resize(grayscaleImage.height() * grayscaleImage.width()); 
	binaryImage = grayscaleImage;

	rgb_store grayCol;
	
	const unsigned char white=255, black=0; // full RGB intensity: white; zero intensity: dark=black

	for (unsigned int y = 0; y < imageHeight; ++y) // pixel [x,y] is [column,row] !?!
	{
		for (unsigned int x = 0; x < imageWidth; ++x)
		{
			grayscaleImage.get_pixel(x,y, grayCol.red, grayCol.green, grayCol.blue); 

			if(!(grayCol.red == grayCol.green && grayCol.green == grayCol.blue)) 
			{
				throw exception("Invalid color in gray image!"); // altern: dynamic assert()
			}

			if(grayCol.red < threshold)
			{
				binaryImage.set_pixel(x,y,black,black,black);
				binaryArray[y*imageWidth+x]=1; // a black pixel is a true in the matrix
			}
			else 
			{
				binaryImage.set_pixel(x,y,white,white,white);
				binaryArray[y*imageWidth+x]=0; // a white pixel is a false in the matrix
			}
		}
	}
}

void CBmpHandler::saveGrayscaleImage(string imageFilename)
{
	grayscaleImage.save_image(imageFilename);
}

void CBmpHandler::saveBinaryImage(string imageFilename)
{
	binaryImage.save_image(imageFilename);
}

void CBmpHandler::saveWorkingImage(string imageFilename)
{
	workingImage.save_image(imageFilename);
}

valarray<unsigned char>& CBmpHandler::getBinaryArray(void)
{
	return binaryArray;
}

unsigned int CBmpHandler::getWidth(void)
{
	return imageWidth;
}

unsigned int CBmpHandler::getHeight(void)
{
	return imageHeight;
}

void CBmpHandler::drawOnBitmap(const valarray<unsigned char>& Array, BitmapCanvas canvas)
{
	setWorkingImage(canvas);

	image_drawer draw(workingImage);
	draw.pen_width(1);
	
	for (unsigned int y = 0; y < workingImage.height(); y++) // for all rows
	{
		for (unsigned int x = 0; x < workingImage.width(); x++)// for all columns
		{
			if (Array[y*imageWidth+x] > 1) // pixel with more than one neighbour shall be red
			{
				draw.pen_color(255,0,0);
				draw.plot_pixel((int)x,(int)y);
			}

			if (Array[y*imageWidth+x] == 1) // start/end pixel shall be green
			{
				draw.pen_color(0,255,0);
				draw.plot_pixel((int)x,(int)y);
			}
		}
	}
}


void CBmpHandler::drawOnBitmap(vector<list<CPoint2D>>& Segments, BitmapCanvas canvas, DrawOnBitmapStyle style)
{
	setWorkingImage(canvas);

	image_drawer draw(workingImage);
	draw.pen_width(1);

	double r, g, b;

	bool firstPixel;

	switch (style)
	{
		
	case lineConnect:

		unsigned int x1, x2, y1, y2;

		for (size_t s = 0; s < Segments.size(); s++)
		{
			double colorInc = 255.0 / Segments[s].size();

			firstPixel = true;
			r = g = b = 0;
			
			for(list<CPoint2D>::iterator itr = Segments[s].begin(); itr != --(Segments[s].end());) //Incerement of iterator
			{
				if (firstPixel)
				{
					r = 255;
					b = 0;
				}

				x1 = itr->getX();
				y1 = itr->getY();

				itr++;

				x2 = itr->getX();
				y2 = itr->getY();
				
				draw.pen_color((unsigned char)r,(unsigned char)g,(unsigned char)b);

				draw.circle(x1,y1,4);

				if (firstPixel)
				{
					r = 0;
					b = 255;
					firstPixel = false;
				}

				draw.pen_color((unsigned char)r,(unsigned char)g,(unsigned char)b);
				draw.line_segment(x1, y1, x2, y2); 
				
				r += colorInc;
			}
			if (s < Segments.size() - 1)
			{
				draw.pen_color(0,255,0);
				draw.line_segment(x2, y2, Segments[s+1].begin()->getX(), Segments[s+1].begin()->getY()); 
			}

			draw.rectangle(x2 - 3,y2 - 3,x2 + 3,y2 + 3);
		}
		break;

	case pixel:
		for (size_t s = 0; s < Segments.size(); s++)
		{
			firstPixel = true;
			r = g = b = 0;

			for(list<CPoint2D>::iterator itr = Segments[s].begin(); itr != Segments[s].end(); itr++)
			{
				double colorInc = 255.0 / Segments[s].size();

				if (firstPixel)
				{
					g = 255;
					b = 0;
				}

				draw.pen_color((unsigned char)r,(unsigned char)g,(unsigned char)b);
			
				draw.plot_pixel(itr->getX(), itr->getY());

				if (firstPixel)
				{
					g = 0;
					b = 255;
					firstPixel = false;
				}

				g += colorInc;
			}
		}
		break;
	}
}

void CBmpHandler::setWorkingImage(BitmapCanvas canvas)
{
	switch (canvas)
	{
	case original: 
		workingImage = originalImage;
		break;

	case gray: 
		workingImage = grayscaleImage;
		break;

	case binary: 
		workingImage = binaryImage;
		break;

	case working: 
		break;

	case white: 
		workingImage = bitmap_image(imageWidth, imageHeight);
		break;
	}
}
