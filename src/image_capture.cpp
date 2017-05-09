#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "Camera_capture.h"
#include "Image_writer.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace cv;
using namespace std;

Camera_capture camera_capture;
Image_writer image_writer;

bool parse_options(int argc, char* argv[]) {
	int image_width,image_height;
	try {
		po::options_description desc("Allowed Options");
		desc.add_options()
			("help", "produce help message")
			("image_width,w", po::value<int>(),
			 "Width of image captured")
			("image_height,h", po::value<int>(),
			 "Height of image captured")
			("resize_scale,s", po::value<float>()->default_value(1.0),
			 "Scale used to resize image")
			("file_pattern,f", po::value<string>()->default_value("*.jpg"),
			 "Pattern for image filenames. Must include valid extention and \'*\', which will be replace with #.")
			("file_dir,d", po::value<string>()->default_value("./"),
			 "Path to where the files will be written")
			;
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return false;
        }
		if(vm.count("image_width"))
			image_width = vm["image_width"].as<int>();
		else{
			cout << "Must provide image width" << endl;
			return false;
		}
		if(vm.count("image_height"))
			image_height = vm["image_height"].as<int>();
		else{
			cout << "Must provide image height" << endl;
			return false;
		}
		if(vm.count("resize_scale")){
			image_writer.set_resize_scale(vm["resize_scale"].as<float>());
		}
		if(vm.count("file_pattern")){
			string file_pattern = vm["file_pattern"].as<string>();
			if(Image_writer::is_file_pattern_valid(file_pattern))
				image_writer.set_file_pattern(file_pattern);
			else{
				cout << "File pattern not valid\n";
				return false;
			}
		}
		
		if(vm.count("file_dir")){
			image_writer.set_path(vm["file_dir"].as<string>());
		}
		camera_capture.set_image_size(image_width,image_height);
		return true;
	} // try
	catch(exception& e) {
		cerr << "error: " << e.what() << "\n";
		return false;
	}
	catch(...) {
		cerr << "Exception of unknown type!\n";
	}
	return true;
}

int main(int argc, char* argv[]){
	if(!parse_options(argc, argv))return 0;
    while(1){
		if(!camera_capture.tick())
			break;
	}
	image_writer.write_images(camera_capture.get_images());
    return 0;	
}
