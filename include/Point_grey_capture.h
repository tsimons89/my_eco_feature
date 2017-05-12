#pragma once

#include <opencv2/opencv.hpp>
#include "FlyCapture2.h"
#include "Camera_capture.h"

using namespace std;
using namespace cv;
using namespace FlyCapture2;
//using namespace FlyCapture2;

class Point_grey_capture : public Camera_capture{
private:
	void set_frame();
	Camera camera;
	void init_camera();
public:
	Point_grey_capture() {init_camera();}
	Point_grey_capture(int width, int height) :Camera_capture(width,height) {init_camera();}
};
