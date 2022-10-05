#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sys/time.h>
#include <iostream>


using namespace cv;
using namespace std;


Vec3b ColorListA[32948];
Vec3b ColorListB[58286];

// funkcja wyodrêbnia powtórki z listy wszystkich pixeli obrazka co daje nam unikalne kolory
void SelectDistinctColors(Vec3b arr[], int n, Vec3b ColorList[])
{
		int NumberOfColors = 0;
	for (int i = 0; i < n; i++)
	{

		int j;
		for (j = 0; j < i; j++) 
			if (arr[i] == arr[j])
			{
				break;
			}
		if (i == j)
		{
			ColorList[NumberOfColors] = arr[i];
			NumberOfColors++;
		}
	}	
}

// funckja powinna zwróciæ pixel podobny pixel do palety kolorów
Vec3b ColorSim(Vec3b ActualColor[], Vec3b ColorPalete[])
{
	int i;
	
	Vec3b d = 0;
	int max;
	for (i = 0, max = sizeof(ActualColor); i < max; i++)
	{
		//jak znaleŸæ podobny kolor?
		d += (ActualColor[i] - ColorPalete[i]);
	}
	return d;
}

// funcja zwraca pixel z palety kolorów
Vec3b SelectSimilarColor(Vec3b ActualColor[], Vec3b ColorPalete[])
{
	Vec3b SelectedColor;

	Vec3b CurrentSim = ColorSim(ActualColor, ColorPalete);

	Vec3b NextColor;

	for (int i = 0; i < sizeof(ColorPalete); i++)
	{
		NextColor = ColorSim(ActualColor, ColorPalete);
		if (NextColor == CurrentSim)
		{
			SelectedColor = ColorPalete[i];
			CurrentSim = NextColor;
		}
	}
	return SelectedColor;
}


// funkcja wype³nia listê wszystkimi pixelami RGP obrazka
void GetImgPixels(Vec3b pixelmap[], Mat_<Vec3b>img)
{
	int index = 0;
	for (int j = 0; j < img.size().width; j++)
	{
		for (int f = 0; f < img.size().height; f++)
		{
			Vec3b t = img(f, j);
			pixelmap[index] = t;
			index++;
		}
	}

}

int main()
{
	string pathA = "Files/obraz-A.jpg";
	string pathB = "Files/obraz-B.jpg";

	Mat_<Vec3b>imageA, imageB, outputA, outputB;
	imageA = imread(pathA);
	imageB = imread(pathB);

	Vec3b k;
	int pix = 0;


	// przeskalowanie obrazu
	resize(imageA, outputA, Size(imageA.size().width * 0.1, imageA.size().height * 0.1), 0, 0);
	resize(imageB, outputB, Size(imageB.size().width * 0.1, imageB.size().height * 0.1), 0, 0);


	Vec3b imageApixelmap[82944];
	Vec3b imageBpixelmap[72600];


	GetImgPixels(imageApixelmap, outputA);
	GetImgPixels(imageBpixelmap, outputB);

	int lenA = sizeof(imageApixelmap) / sizeof(imageApixelmap[0]);
	int lenB = sizeof(imageBpixelmap) / sizeof(imageBpixelmap[0]);
	SelectDistinctColors(imageApixelmap, lenA, ColorListA);
	SelectDistinctColors(imageBpixelmap, lenB, ColorListB);
	SelectSimilarColor(imageBpixelmap, ColorListA);

	
	cout << "Kolory obrazu A " <<  sizeof(ColorListA) / 3 << endl;
	cout << "Kolory obrazu B " <<  sizeof(ColorListB) / 3 << endl;

	imshow("zmiejszone", outputA);
	imshow("obrazb", outputB);
	waitKey(0);
	return 0;
}
