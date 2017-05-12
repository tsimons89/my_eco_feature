#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class Camera_capture{

protected:
	int width, height;
	Mat frame;
	vector<Mat> images;

	virtual void init_camera() = 0;
	virtual void set_frame() = 0;
	bool process_user_input();
	void add_image_of_object();
	Mat get_cropped_image(Point center);
	void display_frame();
	void set_camera_params();

public:
	Camera_capture() {}
	Camera_capture(int width, int height) :width(width),height(height) {}
	void set_image_size(int _width, int _height) {width = _width;height = _height;}
	bool tick();
	vector<Mat> get_images(){return images;}
};

