#include "Webcam_capture.h"

void Webcam_capture::init_camera(){
	cout << "Camera init\n";
	camera.open(0);
}

void  Webcam_capture::set_frame(){
	check_camera();
	camera >> frame;
}

void Webcam_capture::check_camera(){
	if(!camera.isOpened()){  
		cout << "Failed to open camera\n";
		exit(-1);
	}
}
