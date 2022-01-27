#include <iostream>
#include <opencv2/opencv.hpp>	
#include"CParkDetection.h"
#include"CCamera.h"
#include <unistd.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	CParkDetection park;
	CCamera camera("video0");
	int numParks = 0;

	if( !camera.captureFrame() )
	{
		cout << "Blank frame captured!" << endl;
		return -1;
	}
	
	Mat image = imread("/etc/image.jpg", IMREAD_COLOR);

	park.getOutline(image);

	cout << "Parks detected num: " << park.getVacants() << endl; 

	cout << "Put the cars!" << endl;
	char ch;
	cin >> ch;
		
	if( !camera.captureFrame() )
	{
		cout << "Blanck frame captured!" << endl;
		return -1;
	}

	Mat imageWCars = imread("/etc/image.jpg", IMREAD_COLOR);

	numParks = park.calcVacants(imageWCars);
	cout << "numParksAvailable=" << numParks << endl;

	park.writeParks(imageWCars);

	return 0;
}