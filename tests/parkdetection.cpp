// #include "opencv2/imgcodecs.hpp"
// #include "opencv2/highgui.hpp"
// #include "opencv2/imgproc.hpp"
// #include <cmath>
// #include <string>
// #include <iostream>
// #include <vector> 

// using namespace cv;
// using namespace std;

// /**
//  * Helper function to find a cosine of angle between vectors
//  * from pt0->pt1 and pt0->pt2
//  */
// static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
// {
//     double dx1 = pt1.x - pt0.x;
//     double dy1 = pt1.y - pt0.y;
//     double dx2 = pt2.x - pt0.x;
//     double dy2 = pt2.y - pt0.y;
//     return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
// }

// /**
//  * Helper function to display text in the center of a contour
//  */
// void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
// {
//     int fontface = cv::FONT_HERSHEY_SIMPLEX;
//     double scale = 0.4;
//     int thickness = 1;
//     int baseline = 0;

//     cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
//     cv::Rect r = cv::boundingRect(contour);

//     cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
//     cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), FILLED);
//     cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
// }


// int main(int argc, char** argv)
// {
//     // Declare the output variables
//     Mat canny, cdst, cdstP;
//     const char* default_file = "image.jpg";
//     const char* filename = argc >=2 ? argv[1] : default_file;
    
//     string filePath = "../";
//     filePath.append(filename);

//     cout << filePath << endl;

//     // Loads an image
//     Mat src = imread( filePath, IMREAD_GRAYSCALE );

//     // Check if image is loaded fine
//     if(src.empty()){
//         printf(" Error opening image\n");
//         printf(" Program Arguments: [image_name -- default %s] \n", default_file);
//         return -1;
//     }

//     // Edge detection
//     //(image, edges, threshold1, threshold2, apertureSize, L2gradient)
//     Canny(src, canny, 50, 150, 3);

//     // Find contours
//     vector<vector<Point>> contours; // lines
//     findContours(canny.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

//     vector<Point> approx;
//     Mat dst = src.clone();

//     for (int i = 0; i < contours.size(); i++)
//     {
//         // Approximate contour with accuracy proportional
//         // to the contour perimeter
//         approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

//         // Skip small or non-convex objects 
//         if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
//             continue;

//         if (approx.size() >= 4 && approx.size() <= 6)
//         {
//             // Number of vertices of polygonal curve
//             int vtc = approx.size();

//             // Get the cosines of all corners
//             std::vector<double> cos;
//             for (int j = 2; j < vtc+1; j++)
//                 cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

//             // Sort ascending the cosine values
//             std::sort(cos.begin(), cos.end());

//             // Get the lowest and the highest cosine
//             double mincos = cos.front();
//             double maxcos = cos.back();

//             // Use the degrees obtained above and the number of vertices
//             // to determine the shape of the contour
//             if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3)
//                 setLabel(dst, "RECT", contours[i]);
//             else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)
//                 setLabel(dst, "PENTA", contours[i]);
//             else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)
//                 setLabel(dst, "HEXA", contours[i]);
//         }
//         else
//         {
//             // Detect and label circles
//             double area = cv::contourArea(contours[i]);
//             cv::Rect r = cv::boundingRect(contours[i]);
//             int radius = r.width / 2;

//             if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
//                 std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
//                 setLabel(dst, "CIR", contours[i]);
//         }
//     }


//     // Copy edges to the images that will display the results in BGR
//     cvtColor(dst, cdst, COLOR_GRAY2BGR);
//     cdstP = cdst.clone();

//     // Show results
//     imshow("Source", src);
//     // imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
//     imshow("Detected Rectangles", dst);
//     // // Wait and Exit
//     waitKey();
//     return 0;
// }

/* void calculate_m_b(xxxx)
{
    // (image, lines, rho, theta, threshold, minLineLenght, maxLineGap)
    HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 15); // runs the actual detection -->> Melhores parametrops
 

    // for (size_t j=i+1; j < linesP.size(); i++)
        // {
        //     Vec4i l2 = linesP[i];
        //     if(l1[0]!=l1[2])
        //     {
        //         m1=(l1[3]-l1[1])/(l1[2]-l1[0]);
        //         b1=l1[1]-l1[0]*m1;
        //     }
        //     if(l2[0]!=l2[2])
        //     {
        //         m2=(l2[3]-l2[1])/(l2[2]-l2[0]);
        //         b2=l2[1]-l2[0]*m2;
        //     }

                
        // }
}*/

// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

const char* wndname = "Square Detection";

int thresh = 50, N = 11;

// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
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
            // hack: use Canny instead of zero threshold level.
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
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
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