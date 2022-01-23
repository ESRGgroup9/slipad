#include "CCamera.h"
#include "utils.h"

//#include <opencv2/opencv.hpp>

#define FRAME_W 		680
#define FRAME_H 		480

#define CAM_BRIGHTNESS	100
#define CAM_EXPOSURE	100
#define CAM_CONTRAST	100
#define CAM_SATURATION	100

#define DEV_ID 			0

#define PATH 			"/etc/image.jpg"

#include <imgcodecs.hpp>

using namespace cv;
using namespace std;
	
CCamera::CCamera(string camDevName)
{
	camDev.set(CAP_PROP_FRAME_WIDTH , FRAME_W);
    camDev.set(CAP_PROP_FRAME_HEIGHT, FRAME_H);
    camDev.set(CAP_PROP_BRIGHTNESS, CAM_BRIGHTNESS);
    camDev.set(CAP_PROP_EXPOSURE, CAM_EXPOSURE);
    camDev.set(CAP_PROP_CONTRAST, CAM_CONTRAST);
    camDev.set(CAP_PROP_SATURATION, CAM_SATURATION);

    open();
}

CCamera::~CCamera()
{
	close();
}

bool CCamera::open()
{
	// device ID -> camera device
    // apiPreference = cv::CAP_ANY (0) -> autodetect default API
	return camDev.open(DEV_ID, cv::CAP_ANY);
}

void CCamera::close()
{
	camDev.release();
}

bool CCamera::captureFrame()
{
    camDev.read(lastFrame);

    // check if read successfully
    if (lastFrame.empty())
    {
        panic("ERROR! blank frame grabbed.\n");
        return false;
    }

    imwrite(PATH, lastFrame);

    return true;
}

