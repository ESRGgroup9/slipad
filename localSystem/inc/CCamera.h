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
#include <videoio.hpp>

class CCamera
{
public:
	CCamera(std::string camDevName);
	~CCamera();

	/**
	 * @brief 	Open camera device
	 * @return  True if opened/ false if not
	 * */
	bool open();

	/**
	 * @brief 	Close camera device
	 * */
	void close();

	/**
	 * @brief 	Capture image and store it 
	 * @return  True if captured
	 * */
	bool captureFrame();

protected:
	
private:
	// camera device
	cv::VideoCapture camDev;
	
	// camera device name 
	const std::string camDeviceName;

	// last image captured
	cv::Mat lastFrame;
};
// End CCamera class definition

#endif // !__CCAMERA_H__
