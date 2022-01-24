#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <string>
#include <iostream>
#include <vector> 

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Declare the output variables
    Mat dst, cdst, cdstP;
    const char* default_file = "sudoku.png";
    const char* filename = argc >=2 ? argv[1] : default_file;
    
    string filePath = "../";
    filePath.append(filename);

    cout << filePath << endl;

    // Loads an image
    Mat src = imread( filePath, IMREAD_GRAYSCALE );

    // Check if image is loaded fine
    if(src.empty()){
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", default_file);
        return -1;
    }

    // Edge detection
    Canny(src, dst, 50, 200, 3);

    imshow("Canny", dst);
    // Wait and Exit
    waitKey();

    // Copy edges to the images that will display the results in BGR
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    // Standard Hough Line Transform
    vector<Vec2f> lines; // will hold the results of the detection

    // (image, lines, rho, theta, threshold, srn, stn, min_theta, max_theta)
    HoughLines(dst, lines, 1, CV_PI/180, 150, 0, 0); // runs the actual detection

    vector<Vec4i> verticalLines;
    vector<Vec4i> horizontalLines;
    // Draw the lines
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        
        Vec4i l = Vec4i(pt1.x,pt1.y,pt2.x,pt2.y);

        if((theta < (-30)) && (theta > 30))
        {
            verticalLines.push_back(l);
            line( cdst, pt1, pt2, Scalar(0,0,255), 1, LINE_AA);
        }
        else if((theta < 130) && (theta > 50))
        {
            horizontalLines.push_back(l);
            line( cdst, pt1, pt2, Scalar(0,0,255), 1, LINE_AA);
        }
        
    }
    
    // // Probabilistic Line Transform
    // vector<Vec4i> linesP; // will hold the results of the detection
    // // (image, lines, rho, theta, threshold, minLineLenght, maxLineGap)
    // HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 30, 20 ); // runs the actual detection
    // // Draw the lines
    // for( size_t i = 0; i < linesP.size(); i++ )
    // {
    //     Vec4i l = linesP[i];

    //     // for (size_t j=i+1; j < linesP.size(); i++)
    //     // {
    //     //     Vec4i l2 = linesP[i];
    //     //     if(l1[0]!=l1[2])
    //     //     {
    //     //         m1=(l1[3]-l1[1])/(l1[2]-l1[0]);
    //     //         b1=l1[1]-l1[0]*m1;
    //     //     }
    //     //     if(l2[0]!=l2[2])
    //     //     {
    //     //         m2=(l2[3]-l2[1])/(l2[2]-l2[0]);
    //     //         b2=l2[1]-l2[0]*m2;
    //     //     }

                
    //     // }

    //     line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, LINE_AA);

    // }


    // Show results
    imshow("Source", src);
    imshow("Detected Lines (in red) - Standard Hough Line Transform", cdst);
    // imshow("Detected Lines (in red) - Probabilistic Line Transform", cdstP);
    // Wait and Exit
    waitKey();
    return 0;
}