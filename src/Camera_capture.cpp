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
    imshow("frame", frame);
    return process_user_input();
}

bool Camera_capture::process_user_input(){
	switch((char)waitKey(30)){
	case ' ':
		add_image_of_object();break;
	case '\e':
		return false;
	}
	return true;
}

void Camera_capture::add_image_of_object(){
	Component_finder finder(frame);
	Point object_center = finder.get_largest_component_center();
	Mat cropped_image =  get_cropped_image(object_center);
	images.push_back(cropped_image);
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

