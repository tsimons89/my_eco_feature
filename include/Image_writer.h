#pragma once

#include <opencv2/opencv.hpp>
#include <set>

using namespace std;
using namespace cv;

class Image_writer{
private:
	string path;
	string file_pattern;

	bool is_valid_file_num(int num);
	string get_filename(int file_num);
	void check_file_pattern();
	static bool is_there_wildcard(string file_pattern);
	static bool is_there_valid_extention(string file_pattern);

public:
	Image_writer() :path("./"), file_pattern("*.jpg") {}
	Image_writer(string file_pattern) :path("./"), file_pattern(file_pattern) {}
	Image_writer(string path, string file_pattern): path(path), file_pattern(file_pattern) {}


	void set_file_pattern(string _file_pattern){file_pattern = _file_pattern;}
	void set_path(string _path){path = _path;}
	void write_image(Mat image);
	void write_images(vector<Mat> images);
	static bool is_file_pattern_valid(string file_pattern);
};
