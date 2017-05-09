#include "Component_finder.h"
#include <string>

void Component_finder::set_image(Mat _image){
	if(_image.channels() != 1)
		cvtColor(_image,image,CV_RGB2GRAY);
	else
		image = _image;
	process_image_for_segmentation();
}

void Component_finder::process_image_for_segmentation(){
	threshold(image,image,100,255,THRESH_OTSU|THRESH_BINARY);
}

Point Component_finder::get_largest_component_center(){
	set_largest_component();
	Vec2f largest_center_info = centers.row(largest_label);
	return Point(largest_center_info);
}

void Component_finder::set_largest_component(){
	set_components_stats();
	double min_area;
	int smallest_label;
	Mat areas;
	stats.col(CC_STAT_AREA).assignTo(areas,CV_32F);
	minMaxLoc(SparseMat(areas),&min_area,&largest_area,&smallest_label,&largest_label);
}

void Component_finder::set_components_stats(){
	Mat labels = Mat(image.size(),image.type());
	connectedComponentsWithStats(image,labels,stats,centers);
	remove_background_stats();
}

void Component_finder::remove_background_stats(){
	stats = stats.rowRange(1,stats.rows);
	centers = centers.rowRange(1,centers.rows);
}




