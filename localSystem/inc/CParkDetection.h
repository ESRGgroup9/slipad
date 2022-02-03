/**
 * @file 	CParkDetection.cpp, CParkDetection.h
 * @author 	Diogo Fernandes, Tomas Abreu
 * @date 	25/01/2022
 *
 * @brief	Class that detects a parking spot and 
 * 				informs if it is free or occupied
 * 				by detecting cars in it
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

	/**
	 * @brief 	Get parking spots coordinates
	 * @param 	Frame that have the parking spots without any car
	 * @return  True if found parking spot/ false if not
	 * */
	bool getOutline(void);
	
	/**
	 * @brief 	Calculate de number of free parking spots
	 * @param 	Frame of the parking spot with/without cars 
	 * @return  Number of parking spots
	 * */
	int calcVacants(void);

	/**
	 * @brief 	Get free parking spots number
	 * @return  vacantsNum
	 * */
	const int getVacants(void);

	/**
	 * @brief 	Circulate the free parks in green and
	 * 					the occupied parks in red
	 * @param 	3 points forming an angle
	 * @return  Cosine of the angle
	 * */
	void writeParks(cv::Mat& frame);

private:
	/**
	 * @brief 	Calculate the cosine of an angle formed by 3 points
	 * @param 	3 points forming an angle
	 * @return  Cosine of the angle
	 * */
	double angle_cosine(const cv::Point pt1, const cv::Point pt2, const cv::Point pt0 );
	
	/**
	 * @brief 	Verifies if a polygon is a square and add
	 * 				its coords to the parkCoords
	 * @param 	Vector of points forming a polygon 
	 * */
	void addPark( std::vector<cv::Point>& squares );

	/**
	 * @brief 	Verifies if a object is overlapping detected
	 * 				a parking spot
	 * @param 	Vector of points forming an object
	 * @return  The number of the parking spot if it overlaps one
	 * 				and -1 if it dont overlaps 
	 * */
	int isOverlapp(std::vector<cv::Point>& approx);

	/**
	 * @brief 	Finds the center of a polygon
	 * @param 	Vector of points forming a polygon 
	 * @return  Center point  
	 * */
	cv::Point findCenter(std::vector<cv::Point>& polygon);

	/**
	 * @brief	Detects cars in a frame
	 * @param 	Frame 
	 * @return  Rectangles involving cars
	 * */
	std::vector<cv::Rect> detectCars(const cv::Mat& frame);

	/**
	 * @brief	Calculate external points of a rectangle
	 * @param 	Frame 
	 * @return  Rectangles involving cars
	 * */
	std::vector<cv::Point> getRectPoints(const cv::Rect& rectangle);

	/**
	 * @brief 	Detect cars using the Haar cascade 
	 * @param 	Image
	 * @return  Vector of rectangles involving cars
	 * */
	std::vector<cv::Rect> findCars(const cv::Mat& frame);

private:
	// Number of parking spaces available
	int vacantsNum;

	// Number of paking spaces available or not
	int parkNum;

	// Parking spots coordinates
	std::vector<std::vector<cv::Point> > parkCoords;

	std::vector<short> parkStatus;
	
	// Threshould value of the canny detection
	const int thresh;

	// Number of levels of thresould to try
	const int levels;
};
// End CParkDetection class definition

#endif // !__CPARKDETECTION_H__
