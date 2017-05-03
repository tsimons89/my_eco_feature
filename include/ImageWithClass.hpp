/*
 * ImageWithClass.hpp
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef IMAGEWITHCLASS_HPP_
#define IMAGEWITHCLASS_HPP_


#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

class ImageWithClass {
	public:
		Mat image;
		string file_name;
		int classification;
		ImageWithClass(){}
		ImageWithClass(string f, int c) {
			file_name = f;
			image = imread(f, 0);// .c_str()
			//image.convertTo(image, CV_32F, 1.0/255, 0);  //make changes
			image.convertTo(image, CV_32F, 1.0, 0);
			if (image.empty()) {
				cout << "Problem loading image " << f << endl;
				exit(0);
			}
			classification = c;
		}

		~ImageWithClass() {
		}
};


#endif /* IMAGEWITHCLASS_HPP_ */
