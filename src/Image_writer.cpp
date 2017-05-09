#include "Image_writer.h"

set<string> valid_file_extentions = {"jpg","jpeg","jpe","bmp","png","pbm","pgm","ppm","pxm","pnm"};

void Image_writer::write_image(Mat image){
	unsigned int file_num = 0;
	while(!is_valid_file_num(file_num))
		file_num++;
	imwrite(path + get_filename(file_num),image);
}

bool Image_writer::is_valid_file_num(int num){
	string filename = get_filename(num);
	vector<String> filenames;
	cv::glob(path,filenames);
	return (find(filenames.begin(),filenames.end(),filename) == filenames.end());
}
string Image_writer::get_filename(int file_num){
	check_file_pattern();
	string filename = file_pattern;
	filename.replace(file_pattern.find("*"),1,to_string(file_num));
	return filename;
}

bool Image_writer::is_file_pattern_valid(string file_pattern){
	return 	is_there_wildcard(file_pattern) && 
			is_there_valid_extention(file_pattern);
}

bool Image_writer::is_there_wildcard(string file_pattern){
	return (file_pattern.find("*") != string::npos);
}
bool Image_writer::is_there_valid_extention(string file_pattern){
	size_t pos = file_pattern.find(".");
	string ext = file_pattern.substr(pos);
	return valid_file_extentions.find(ext) != valid_file_extentions.end();
}

void Image_writer::check_file_pattern(){
	if(!is_file_pattern_valid(file_pattern)){
		cout << "File pattern not valid\n";
		exit(-1);
	}
}

void Image_writer::write_images(vector<Mat> images){
	for(Mat image:images){
		write_image(image);
	}
}
