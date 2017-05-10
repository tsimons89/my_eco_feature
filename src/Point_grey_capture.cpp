#include "Point_grey_capture.h"

void Point_grey_capture::init_camera(){
	camera.Connect( 0 );
	camera.StartCapture();
}

void Point_grey_capture::set_frame(){
	Image rawImage;
	if ( camera.RetrieveBuffer( &rawImage ) != PGRERROR_OK )
		cout << "capture error" << endl;
    Image rgbImage;
    rawImage.Convert( PIXEL_FORMAT_BGR, &rgbImage );
	unsigned int rowBytes = (double)rgbImage.GetReceivedDataSize()/(double)rgbImage.GetRows();       
	frame = Mat(rgbImage.GetRows(), rgbImage.GetCols(), CV_8UC3, rgbImage.GetData(),rowBytes).clone();
}

