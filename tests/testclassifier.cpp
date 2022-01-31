#include <cstdlib>
#include <iostream>
#include <vector>

// OpenCV
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) 
{
  // Read the image in grayscale
  Mat image = imread("cars2.jpg", IMREAD_COLOR);
  if( image.empty() )
  {
    cout << "Image empty!" <<  endl;
    return -1;
  }

  Mat gray;
  cvtColor(image, gray, COLOR_BGR2GRAY);

  // Load a classifier from its XML description
  CascadeClassifier classifier("haar_cars1.xml");

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

  // Draw each feature as a separate green rectangle
  for (auto&& feature : features) 
  {
    rectangle(image, feature, Scalar(0, 0, 255), 2, LINE_AA);
  }

  const string windowName = "Cars Detection";
  namedWindow(windowName, WINDOW_AUTOSIZE);

  // Show the captured image and the detected features
  imshow(windowName, image);
  waitKey();

  return EXIT_SUCCESS;
}