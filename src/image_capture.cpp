#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "Camera_capture.h"
#include "Image_writer.h"
#include "Point_grey_capture.h"
#include "Webcam_capture.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace cv;
using namespace std;

Camera_capture* camera_capture;
Image_writer image_writer;

bool parse_options(int argc, char* argv[]) {
	int capture_width,capture_height,write_width,write_height;
	try {
		po::options_description desc("Allowed Options");
		desc.add_options()
			("help", "produce help message")
			("capture_width,w", po::value<int>(),
			 "Width of image captured")
			("capture_height,h", po::value<int>(),
			 "Height of image captured")
			("write_width,W", po::value<int>(),
			 "Width of image written to file")
			("write_height,H", po::value<int>(),
			 "Height of image written to file")
			("file_pattern,f", po::value<string>()->default_value("*.jpg"),
			 "Pattern for image filenames. Must include valid extention and \'*\', which will be replace with #.")
			("file_dir,d", po::value<string>()->default_value("./"),
			 "Path to where the files will be written")
			("use_point_grey,g", po::value<bool>()->default_value(true),
			 "Is the camera being used a Point Grey camera")
			;
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return false;
        }

		if(vm.count("capture_width"))
			capture_width = vm["capture_width"].as<int>();
		else{
			cout << "Must provide capture width" << endl;
			return false;
		}
		if(vm.count("capture_height"))
			capture_height = vm["capture_height"].as<int>();
		else{
			cout << "Must provide capture height" << endl;
			return false;
		}

		if(vm.count("write_width"))
			write_width = vm["write_width"].as<int>();
		else
			write_width = capture_width;

		if(vm.count("write_height"))
			write_height = vm["write_height"].as<int>();
		else
			write_height = capture_height;

		image_writer.set_size(write_width,write_height);

		if(Image_writer::is_file_pattern_valid(vm["file_pattern"].as<string>()))
			image_writer.set_file_pattern(vm["file_pattern"].as<string>());
		else{
			cout << "File pattern not valid\n";
			return false;
		}

		image_writer.set_path(vm["file_dir"].as<string>());


		if(vm["use_point_grey"].as<bool>())
			camera_capture = new Point_grey_capture(capture_width,capture_height);
		else
			camera_capture = new Webcam_capture(capture_width,capture_height);


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
		if(!camera_capture->tick())
			break;
	}
	image_writer.write_images(camera_capture->get_images());
    return 0;	
}
