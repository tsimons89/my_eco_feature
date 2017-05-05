#include <opencv2/opencv.hpp>
#include "capture_helper.h"
using namespace cv;
using namespace std;

int main(int argc, char* argv[]){
	//parse_options(argc, argv);
	VideoCapture camera = get_camera();
    while(1){
		if(capture_tick(camera))
			break;
    }

    return 0;	
}
