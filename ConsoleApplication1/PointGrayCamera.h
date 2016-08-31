#pragma once
///////////////////////////////
//
//2015.09.01 èºà¸Å@çÏê¨
//
///////////////////////////////////////


#include <opencv2/opencv.hpp>
//#include "FlyCap2OpenCV.h"

#include <FC1\PGRFlyCapture.h>
#include <FC1\PGRCameraGUI.h>

class PointGrayCamera
{
	private:
	long serialNumber;
	INT_PTR dialogStatus;

	public:
	int initServerCamera();
	int initClientCamera();
	int dst(void);
	int Capture();
	FlyCaptureContext context;
	FlyCaptureError error;
	FlyCaptureImage rawImage, colorImage;
	cv::Mat_<cv::Vec3b> buf;
	cv::Mat_<cv::Vec3b> cv_image;


};