#include "CCamera.h"
#include "utils.h"

//#include <opencv2/opencv.hpp>

#define FRAME_W 		640
#define FRAME_H 		480

#define CAM_BRIGHTNESS	50//50
#define CAM_EXPOSURE	20//20
#define CAM_CONTRAST	80//0
#define CAM_SATURATION	100//50
#define CAM_HUE  180//50

#define DEV_ID 			0

#define PATH 			"/etc/image.jpg"

#include <imgcodecs.hpp>

using namespace cv;
using namespace std;
	
CCamera::CCamera(string camDevName)
{
    open();

	camDev.set(CAP_PROP_FRAME_WIDTH , FRAME_W);
    camDev.set(CAP_PROP_FRAME_HEIGHT, FRAME_H);
    camDev.set(CAP_PROP_BRIGHTNESS, CAM_BRIGHTNESS);
    camDev.set(CAP_PROP_EXPOSURE, CAM_EXPOSURE);
    camDev.set(CAP_PROP_CONTRAST, CAM_CONTRAST);
    camDev.set(CAP_PROP_SATURATION, CAM_SATURATION);
    // camDev.set(CAP_PROP_WHITE_BALANCE_RED_V, CAM_HUE);

    //camDev.set(CAP_PROP_SHARPNESS, CAM_SHARPNESS);

    
    // printf("BRIGHTNESS: %d\n", camDev.get(CAP_PROP_BRIGHTNESS));
    // printf("EXPOSURE: %d\n", camDev.get(CAP_PROP_EXPOSURE));
    // printf("CONTRAST: %d\n", camDev.get(CAP_PROP_CONTRAST));
    // printf("SATURATION: %d\n", camDev.get(CAP_PROP_SATURATION));
}

CCamera::~CCamera()
{
	close();
}

bool CCamera::open()
{
	// device ID -> camera device
    // apiPreference = cv::CAP_ANY (0) -> autodetect default API
	// return camDev.open(DEV_ID, cv::CAP_ANY);
    return camDev.open(DEV_ID, cv::CAP_V4L2);
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
    cvtColor(lastFrame,lastFrame, CV_BGR2GRAY);
    imwrite(PATH, lastFrame);

    return true;
}

