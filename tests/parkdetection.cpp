#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int thresh = 50, N = 11;

// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle_cosine( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();
    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // // down-scale and upscale the image to filter out the noise
    // pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    // pyrUp(pyr, timg, image.size());

    // Filter the noise
    Mat gaussian; 
    GaussianBlur(image, gaussian, Size(7, 7), 0);

    // imshow("image", image);
    // imshow("noise reduct", timg);
    // imshow("gaussian", gaussian);
    // waitKey();
    
    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&gaussian, 1, &gray0, 1, ch, 1);
        // try several threshold levels
        for( int l = 0; l < N; l++ )
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
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            
            vector<Point> approx;
            
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // aproximate the contour to a polygon
                approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);

                // A square should have 4 vertices after aproximation &&
                // have a relatively large area &&
                // have a not so big area && 
                // be convex
                if( approx.size() == 4 &&
                    fabs(contourArea(approx)) > 1000 &&
                    fabs(contourArea(approx)) < 100000 &&
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
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
}

int main(int argc, char** argv)
{
    string filename = "../image.jpg";

    Mat image = imread(filename, IMREAD_COLOR);
    if( image.empty() )
    {
        cout << "Couldn't load " << filename << endl;
        return -1;
    }

    Mat imageSquares = image.clone();
    
    vector<vector<Point> > squares;
    findSquares(imageSquares, squares);
    
    polylines(imageSquares, squares, true, Scalar(0, 0, 255), 3, LINE_AA);
    
    imshow("Original Image", image);
    imshow("Image with Squares Detected", imageSquares);
    waitKey();

    return 0;
}