#include "ImgProcessor.h"

CImgProcessor::CImgProcessor(void)
{
}

CImgProcessor::CImgProcessor(unsigned int width, unsigned int height)
{
	imageWidth = width;
	imageHeight = height;
}


CImgProcessor::~CImgProcessor(void)
{
}

void CImgProcessor::thinImage(const valarray<unsigned char> &binaryArray)
{
	binaryArrayThinned = binaryArray;

	int candidatesCounter = 0; //Zhang-Suen paper -> C
	unsigned char A, B; //variable names from Zhang-Suen paper

	vector<unsigned int> candidatesVec;

	do
	{
		for (unsigned int y = 1; y < imageHeight - 1; y++) //for all rows
		{
			for (unsigned int x = 1; x < imageWidth - 1; x++) //for all columns
			{
				if (binaryArrayThinned[y*imageWidth+x] == 1) //if pixel is black
				{
					assignP(x,y); //store neighbourhood in P2 - P9
					A = calcA(); //count number of 0-1 changes around P (P2-P3, P3-P4, .... P9-P2)
					B = calcB(); //number of black pixel around P

					if (B > 1 && B < 7 && A == 1 && (P2*P4*P6) == 0 && (P4*P6*P8) == 0)
					{
						candidatesVec.push_back(y*imageWidth+x);//current pixel is candidate for deletion
						candidatesCounter++;//number of candidates
					}
				}
			}
		}

		if (candidatesCounter > 0)
		{
			//printf("C = %d\n", C);
			candidatesCounter = 0;
		}
		else
		{
			break;//if no more candidates found -> end do-while
		}

		for (size_t i = 0; i < candidatesVec.size(); i++)
		{
			binaryArrayThinned[candidatesVec[i]] = 0;
		}

		candidatesVec.clear();

		for (unsigned int y = 1; y < imageHeight - 1; y++)
		{
			for (unsigned int x = 1; x < imageWidth - 1; x++)
			{
				if (binaryArrayThinned[y*imageWidth+x] == 1)
				{
					assignP(x,y);
					A = calcA();
					B = calcB();

					if (B > 1 && B < 7 && A == 1 && (P2*P4*P8) == 0 && (P2*P6*P8) == 0)
					{
						candidatesVec.push_back(y*imageWidth+x);
						candidatesCounter++;
					}
				}
			}
		}

		if (candidatesCounter == 0)
		{
			break;
		}

		for (size_t i = 0; i < candidatesVec.size(); i++)
		{
			binaryArrayThinned[candidatesVec[i]] = 0;
		}

		candidatesVec.clear();
		//printf("C = %d\n", C);
	}
	while (candidatesCounter > 0);

	//removal of pixel with more than two neighbours (steps or intersections)
	/* 
	for (unsigned int y = 1; y < imageHeight - 1; y++)
	{
		for (unsigned int x = 1; x < imageWidth - 1; x++)
		{
			if (binaryArrayThinned[y*imageWidth+x] == 1)
			{
				assignP(x,y);

				if (P2*P4 == 1 || P4*P6 == 1 || P6*P8 == 1 || P8*P2 == 1)// || P3*P5*P4 == 1 || P5*P7*P6 == 1 || P7*P9*P8 == 1 || P9*P3*P2 == 1)
				{
					binaryArrayThinned[y*imageWidth+x] = 0;
				}
			}
		}
	}
	*/

	encodeNeighbourhood();
	
}

valarray<unsigned char>& CImgProcessor::getBinaryArrayThinned(void)
{
	return binaryArrayThinned;
}

void CImgProcessor::assignP(unsigned int x, unsigned int y)
{
	P2 = binaryArrayThinned[(y - 1) * imageWidth + x];
	P3 = binaryArrayThinned[(y - 1) * imageWidth + x + 1];
	P4 = binaryArrayThinned[y * imageWidth + x + 1];
	P5 = binaryArrayThinned[(y + 1) * imageWidth + x + 1];
	P6 = binaryArrayThinned[(y + 1) * imageWidth + x];
	P7 = binaryArrayThinned[(y + 1) * imageWidth + x - 1];
	P8 = binaryArrayThinned[y * imageWidth + x - 1];
	P9 = binaryArrayThinned[(y - 1) * imageWidth + x - 1];
}

unsigned char CImgProcessor::calcA(void) 
{
	//go around P clockwise
	unsigned char ret = 0;

	if (P2 == 0 && P3 != 0)
	{
		ret++;
	}

	if (P3 == 0 && P4 != 0)
	{
		ret++;
	}

	if (P4 == 0 && P5 != 0)
	{
		ret++;
	}

	if (P5 == 0 && P6 != 0)
	{
		ret++;
	}

	if (P6 == 0 && P7 != 0)
	{
		ret++;
	}

	if (P7 == 0 && P8 != 0)
	{
		ret++;
	}

	if (P8 == 0 && P9 != 0)
	{
		ret++;
	}

	if (P9 == 0 && P2 != 0)
	{
		ret++;
	}

	return ret;
}

unsigned char CImgProcessor::calcB(void)
{	
	// count the number of neighbours that are not white (white==0)

	int ret = 0;

	if (P2 != 0)
		ret++;

	if (P3 != 0)
		ret++;

	if (P4 != 0)
		ret++;

	if (P5 != 0)
		ret++;

	if (P6 != 0)
		ret++;

	if (P7 != 0)
		ret++;

	if (P8 != 0)
		ret++;

	if (P9 != 0)
		ret++;
	
	return ret;
}

void CImgProcessor::encodeNeighbourhood()
{
	unsigned char B;

	for (unsigned int y = 1; y < imageHeight - 1; y++)
	{
		for (unsigned int x = 1; x < imageWidth - 1; x++)
		{
			if (binaryArrayThinned[y*imageWidth+x] != 0)
			{
				assignP(x,y);
				B = calcB();

				binaryArrayThinned[y*imageWidth+x] = B;
			}
		}
	}
}

void CImgProcessor::removeIsolatedPixel()
{
	unsigned char B;

	for (unsigned int y = 1; y < imageHeight - 1; y++)
	{
		for (unsigned int x = 1; x < imageWidth - 1; x++)
		{
			if (binaryArrayThinned[y*imageWidth+x] > 0)
			{
				assignP(x,y);
				B = calcB();

				if (B == 0)
				{
					binaryArrayThinned[y*imageWidth+x] = 0;
				}
			}
		}
	}
}