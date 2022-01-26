/**
 * @file 	CParkDetection.cpp, CParkDetection.h
 * @author 	Diogo Fernandes, Tomas Abreu
 * @date 	25/01/2022
 *
 * @brief	Class that detects a parking spot and 
 * 				informs if it is free or occupied
 */

#ifndef __CPARKDETECTION_H__
#define __CPARKDETECTION_H__

#include <opencv2/opencv.hpp>	
#include <vector>

class CParkDetection
{
public:
	CParkDetection();
	~CParkDetection();

	bool getOutline(const cv::Mat& frame);
	
	int calcVacants(const cv::Mat& frame);
	const int getVacants(void);

private:
	double angle_cosine( cv::Point pt1, cv::Point pt2, cv::Point pt0 );
	void isSquare( std::vector<cv::Point>& squares );

private:
	int vacantsNum;
	int parkNum;

	std::vector<std::vector<cv::Point> > parkCoords;
	
	const int thresh;
	const int levels;
};
// End CParkDetection class definition

#endif // !__CPARKDETECTION_H__
