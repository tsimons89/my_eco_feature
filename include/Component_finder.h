#pragma once
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Component_finder{

private:
	void process_image_for_segmentation();
	void set_largest_component();
	void set_components_stats();
	void remove_background_stats();
	int largest_label;
	double largest_area;
	Mat stats,centers;
	Mat image;

public:
	Component_finder(){}
	Component_finder(Mat image){set_image(image);}
	void set_image(Mat image);
	Point get_largest_component_center();

};
