#include <iostream>
#include <opencv2/opencv.hpp>	
#include"CParkDetection.h"
#include"CCamera.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	CParkDetection park;
	CCamera camera("video0");
	int numParks = 0;

	Mat image = imread("/etc/image.jpg", IMREAD_COLOR);

	numParks = park.getOutline(image);

	cout << "numParks=" << numParks << endl;

	return 0;
}