#ifndef CAPTURE_HELPER
#define CAPTURE_HELPER

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

using namespace cv;
using namespace std;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

//bool parse_options(int argc, char* argv[]);
VideoCapture get_camera(int width = 0,int height = 0);
bool capture_tick(VideoCapture camera);

#endif
