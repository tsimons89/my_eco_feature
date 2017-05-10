#pragma once

#include <opencv2/opencv.hpp>

#include "Camera_capture.h"

using namespace std;
using namespace cv;


class Webcam_capture : public Camera_capture{
private:
	VideoCapture camera;
	void set_frame();
	void check_camera();
	void init_camera();
public:
	Webcam_capture() {init_camera();}
	Webcam_capture(int width, int height) :Camera_capture(width,height) {init_camera();}
};
