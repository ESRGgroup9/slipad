#include "CParkDetection.h"

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

//debug
#include <unistd.h>

using namespace cv;
using namespace std;

CParkDetection::CParkDetection():
	vacantsNum(0), 
	parkNum(0), 
	thresh(50),
	levels(11)
{

}

CParkDetection::~CParkDetection()
{

}

bool CParkDetection::getOutline(const Mat& frame)
{
    Mat gray0(frame.size(), CV_8U), gray;

    // Filter the noise
    Mat gaussian; 
    GaussianBlur(frame, gaussian, Size(7, 7), 0);
    
    vector<vector<Point> > contours;

    // empty the vector
    parkCoords.clear();

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&gaussian, 1, &gray0, 1, ch, 1);
        // try several threshold levels
        for( int l = 0; l < levels; l++ )
        {
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);

                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/levels;
            }

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                addPark(contours[i]);
            }
        }
    }

    vacantsNum = parkCoords.size();

    if(vacantsNum == 0)
    	return false;

	return true;
}

int CParkDetection::calcVacants(const Mat& frame)
{
    // no parking spot detected
    if(!parkCoords.size())
        getOutline(frame);

    // detect cars in the frame
	vector<Rect> features = detectCars(frame);

    // Reset the number of parking spots available
    vacantsNum = parkCoords.size();
    fill(parkStatus.begin(), parkStatus.end(), 1);

    // Evaluate the parking spot availability
    for( int f = 0; f < features.size(); f++ )
    {
        vector<Point> rect = getRectPoints(features[f]);
        
        // Is the car over the parking spot?
        //  The center of the car match with the center of the parking spot?
        int pos = isOverlapp(rect);
        if( pos != -1 )
        {
            vacantsNum--;
            // park unavailable
            parkStatus[pos] = 0;
        }
    }
   
	// return number of free parking spots
	return vacantsNum;
}

const int CParkDetection::getVacants(void)
{
	return vacantsNum;
}

vector<Rect> CParkDetection::detectCars(const Mat& frame)
{
    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    // Load a classifier from its XML description
    CascadeClassifier classifier("../haar_cars1.xml");

    Mat gaussian;
    // blur the image
    GaussianBlur(gray, gaussian, Size(5, 5), 0);

    Mat ones = Mat(3, 3, CV_8UC1, Scalar(1));
    Mat dilated;
    // dilate canny output to remove potential
    // holes between edge 
    dilate( gaussian, dilated, ones );

    // get the desired kernel 
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));

    Mat output;
    // apply morphology transformation with the desired kernel
    morphologyEx(dilated, output, MORPH_CLOSE, kernel);

    // Prepare a vector where the detected features will be stored
    vector<Rect> features;

    // Detect the features in the normalized, gray-scale version of the image
    classifier.detectMultiScale(output, features, 1.1, 2,
                          0 | CASCADE_SCALE_IMAGE, Size(50, 50));

    return features;
}

void CParkDetection::addPark( vector<Point>& contour )
{
	vector<Point> approx;

    // aproximate the contour to a polygon
    approxPolyDP(contour, approx, arcLength(contour, true)*0.02, true);

    // A square should have 4 vertices after aproximation &&
    // have a relatively large area &&
    // have a not so big area && 
    // be convex
    if( (approx.size() == 4) &&
        (fabs(contourArea(approx)) > 3000) &&
        (fabs(contourArea(approx)) < 50000) &&
        isContourConvex(approx) )
    {
        double maxCosine = 0;
        // find the maximum cosine of all angles of the polygon
        for( int j = 2; j < 5; j++ )
        {
            double cosine = fabs(angle_cosine(approx[j%4], approx[j-2], approx[j-1]));
            maxCosine = MAX(maxCosine, cosine);
        }

        // to be consired a rectangle it must have small cosines (~90 degrees) 
        // && not be overlapped with other rectangles
        if( maxCosine < 0.1 && (isOverlapp(approx) == -1) )
        {
            parkStatus.push_back(1); // all available
            parkCoords.push_back(approx);
            vacantsNum++;
        }
    }
}

int CParkDetection::isOverlapp(vector<Point>& approx)
{
    Point approxCenter = findCenter(approx);

    for( int i=0; i < parkCoords.size(); i++ )
    {        
        Point parkCoordsCenter = findCenter(parkCoords[i]);

        // centers are overlapped? Distance between centers inferior to 30
        if( fabs(parkCoordsCenter.x - (approxCenter.x)) < 30 &&
            fabs(parkCoordsCenter.y - (approxCenter.y)) < 30 )   
            return i;
    }

    return -1;
}

Point CParkDetection::findCenter(vector<Point>& polygon)
{
    Moments m = moments(polygon);

    return Point( static_cast<float>(m.m10 / (m.m00 + 1e-5)),
                  static_cast<float>(m.m01 / (m.m00 + 1e-5)) );
}

double CParkDetection::angle_cosine(const Point pt1, const Point pt2, const Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;

    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

vector<Point> CParkDetection::getRectPoints(const Rect& rectangle)
{
    vector<Point> points;

    float width = rectangle.width;
    float height = rectangle.height;

    // down left point
    points.push_back(Point(rectangle.x, rectangle.y));

    // down right point
    points.push_back(Point(rectangle.x+width, rectangle.y));

    // top right point
    points.push_back(Point(rectangle.x+width, rectangle.y+height));

    // top left point 
    points.push_back(Point(rectangle.x, rectangle.y+height));

    return points;
}

/**
 * Debug Function 
 */
void CParkDetection::writeParks(Mat& frame)
{
    for( int i = 0; i < parkCoords.size(); i++ )
    { 
        // Available
        if( parkStatus[i] == 1 )
            polylines(frame, parkCoords[i], true, Scalar(0, 255, 0), 1, LINE_AA);
        // unavailable
        else if( parkStatus[i] == 0 )
            polylines(frame, parkCoords[i], true, Scalar(0, 0, 255), 1, LINE_AA);
    }
    
    imwrite("image.jpg", frame);
    system("scp image.jpg fernandes@10.42.0.1:/home/fernandes/code/slipad/opencv");
}