#include "Camera_capture.h"
#include "Component_finder.h"

void  Camera_capture::set_frame(){
	check_camera();
	camera >> frame;
}

void Camera_capture::check_camera(){
	if(!camera.isOpened()){  
		cout << "Failed to open camera\n";
		exit(-1);
	}
}

bool Camera_capture::tick(){
	set_frame();
	display_frame();
    return process_user_input();
}

bool Camera_capture::process_user_input(){
	switch((char)waitKey(30)){
	case ' ':
		add_image_of_object();break;
	case '\n':
		return false;
	case '\e':
		exit(-1);
	}
	return true;
}

void Camera_capture::add_image_of_object(){
	Component_finder finder(frame);
	Point object_center = finder.get_largest_component_center();
	Mat cropped_image =  get_cropped_image(object_center);
	images.push_back(cropped_image.clone());
}

Mat Camera_capture::get_cropped_image(Point center){
	int x =  (center.x - width/2 < 0)                   ? 0 :
			 ((center.x + width/2) > frame.size().width)? frame.size().width - width:
  	  	 	 											  center.x - width/2;

	int y =  (center.y - height/2 < 0)                    ? 0 :
			 ((center.y + height/2) > frame.size().height)? frame.size().height - height: 
	                                                        center.y - height/2;
	Rect crop_region(x,y,width,height);
	return Mat(frame,crop_region);
}

void Camera_capture::display_frame(){
	Mat display_image = frame.clone();
	putText(display_image,"Enter command:",Point(10,20),FONT_HERSHEY_PLAIN,1.5,Scalar(0,0,255));
	putText(display_image,"Space - capture image",Point(10,40),FONT_HERSHEY_PLAIN,1.5,Scalar(0,0,255));
	putText(display_image,"Enter - save images",Point(10,60),FONT_HERSHEY_PLAIN,1.5,Scalar(0,0,255));
	putText(display_image,"Esc - exit",Point(10,80),FONT_HERSHEY_PLAIN,1.5,Scalar(0,0,255));
	imshow("Image capture", display_image);
}

void Camera_capture::set_camera_params(){
	camera.set(CAP_PROP_FPS,60);
	camera.set(CV_CAP_PROP_BRIGHTNESS,0.5);
	camera.set(CV_CAP_PROP_SATURATION,0.5);
	camera.set(CV_CAP_PROP_CONTRAST,0.5);
	camera.set(CAP_PROP_AUTOFOCUS,0);
	cout << "Contrast: " << camera.get(CV_CAP_PROP_CONTRAST) << endl;
	cout << "Sat: " << camera.get(CV_CAP_PROP_SATURATION) << endl;
	cout << "Hue: " << camera.get(CV_CAP_PROP_HUE) << endl;
	cout << "Mode: " << camera.get(CAP_PROP_MODE) << endl;
	cout << "FPS: " << camera.get(CAP_PROP_FPS) << endl;
}

