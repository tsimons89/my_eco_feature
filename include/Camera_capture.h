#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Camera_capture{

private:
	VideoCapture camera;
	int width, height;
	Mat frame;
	vector<Mat> images;
	void set_frame();
	void check_camera();
	bool process_user_input();
	void add_image_of_object();
	Mat get_cropped_image(Point center);
	void display_frame();
	void set_camera_params();

public:
	Camera_capture() {camera.open(0);set_camera_params();}
	Camera_capture(int width, int height) :width(width),height(height) {camera.open(0);set_camera_params();}
	void set_image_size(int _width, int _height) {width = _width;height = _height;}
	bool tick();
	vector<Mat> get_images(){return images;}
};

