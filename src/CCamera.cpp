#include "CCamera.h"
#include "utils.h"
#include <unistd.h>

#define FRAME_W 		640
#define FRAME_H 		480

#define CAM_BRIGHTNESS	0.4  //-> 20
#define CAM_CONTRAST	0.50 //-> 0
#define CAM_SATURATION	0.6  //-> 10

#define DEV_ID 			0

#define PATH 			"/etc/image.jpg"

#include <imgcodecs.hpp>

using namespace cv;
using namespace std;
	
CCamera::CCamera(string camDevName)
{
    open();

    /******** Camera Configurations **********/    
	camDev.set(CAP_PROP_FRAME_WIDTH , FRAME_W);
    camDev.set(CAP_PROP_FRAME_HEIGHT, FRAME_H);
    camDev.set(CAP_PROP_BRIGHTNESS, CAM_BRIGHTNESS);
    camDev.set(CAP_PROP_CONTRAST, CAM_CONTRAST);
    camDev.set(CAP_PROP_SATURATION, CAM_SATURATION);

    system("v4l2-ctl --set-ctrl=auto_exposure=0");
    system("v4l2-ctl --set-ctrl=iso_sensitivity=4");
    system("v4l2-ctl --set-ctrl=auto_exposure_bias=18");
    system("v4l2-ctl --set-ctrl=exposure_time_absolute=1000");
    system("v4l2-ctl --set-ctrl=exposure_dynamic_framerate=1");
    system("v4l2-ctl --set-ctrl=white_balance_auto_preset=7");

    system("v4l2-ctl --set-ctrl=sharpness=30");

    sleep(2);
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

    // Convert to grey scale
    cvtColor(lastFrame,lastFrame, CV_BGR2GRAY);

    // write image to path
    imwrite(PATH, lastFrame);

    // system("scp image.jpg fernandes@10.42.0.1:/home/fernandes/code/slipad/doc/report/images/13tests/camera/");

    return true;
}

