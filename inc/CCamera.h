/**
 * @file 	CCamera.cpp, CCamera.h
 * @author 	Diogo Fernandes, Tomas Abreu
 * @date 	21/01/2022
 *
 * @brief	Class that deals with camera device
 */

#ifndef __CCAMERA_H__
#define __CCAMERA_H__

#include <opencv2/opencv.hpp>

// #include <opencv/cv.h>
//#include <highgui.h>

#include <videoio.hpp>

class CCamera
{
public:
	CCamera(std::string camDevName);
	~CCamera();

	bool open();
	void close();

	//bool isOpen();

	bool captureFrame();

protected:
	
private:
	cv::VideoCapture camDev;
	std::string camDeviceName; // >>>>>>>>>>>>>>>>>>>>>>>>> See if useful

	//std::vector<Mat>lastFrame;
	cv::Mat lastFrame;
};
// End CCamera class definition

#endif // !__CCAMERA_H__
