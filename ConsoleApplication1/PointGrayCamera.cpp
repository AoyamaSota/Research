//#include "targetver.h"

///////////////////////////////
//
//2015.09.01 �����@�쐬
//
///////////////////////////////////////


#include <opencv2/opencv.hpp>
//#include "OpenCVheader.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

//#define _USE_MATH_DEFINES 1
#include <math.h>

//#include "stdafx.h"
#include <FC1\PGRFlyCapture.h>
#include <FC1\PGRCameraGUI.h>
#include "PointGrayCamera.h"

#define CAPTURE_WIDTH	640		//�J�����̃L���v�`���[�T�C�Y
#define CAPTURE_HEIGHT	480

using namespace std;

int PointGrayCamera::initServerCamera(){
	cout<<"start init"<<endl;

	//serialNumber = 12071156;//�J�������w�肷��ꍇ
//	serialNumber = 10130409;//�J�������w�肷��ꍇ
	serialNumber = 12071156;
	error = flycaptureCreateContext( &context);

	// �J�����̏�����
	flycaptureInitializeFromSerialNumber(context, serialNumber);
	if ( error != FLYCAPTURE_OK){
		cout <<"flycaptureInitializeFromSerialNumber"<<endl;
		return 1;
	}

	//error = flycaptureStart( context, FLYCAPTURE_VIDEOMODE_640x480YUV422, FLYCAPTURE_FRAMERATE_30);
	error = flycaptureStart( context, FLYCAPTURE_VIDEOMODE_ANY, FLYCAPTURE_FRAMERATE_ANY);
	if ( error != FLYCAPTURE_OK)
		return 1;

	// �摜�̃T�C�Y�𓾂邽�߂Ɉ�x�L���v�`���[����
	//
	error = flycaptureGrabImage2( context, &rawImage);
	if ( error != FLYCAPTURE_OK){
		cout<<"flycaptureGrabImage2"<<endl;
		return 1;
	}
	colorImage = rawImage;
	buf = cv::Mat_<cv::Vec3b>(colorImage.iRows,colorImage.iCols);

	// �x�C���[�摜 -> �J���[�摜�ϊ��p�̃p�����[�^�Z�b�g
	//
	colorImage.pixelFormat = FLYCAPTURE_BGR;
	colorImage.pData = (unsigned char*)buf.data; // �ϊ���̃o�b�t�@��OpenCV�̉摜�̃o�b�t�@�Ɏw��

	return 0;
}

int PointGrayCamera::initClientCamera(){
	cout<<"start init"<<endl;

	//serialNumber = 12071156;//�J�������w�肷��ꍇ
	serialNumber = 10130409;//�J�������w�肷��ꍇ
//	serialNumber = 15307968;
	error = flycaptureCreateContext( &context);
	if( error != FLYCAPTURE_OK){
		cout<<"error create context"<<endl;
		return 1;
	}

	// �J�����̏�����
	flycaptureInitializeFromSerialNumber(context, serialNumber);
	if ( error != FLYCAPTURE_OK){
		cout <<"client flycaptureInitializeFromSerialNumber"<<endl;
		return 1;
	}

	//error = flycaptureStart( context, FLYCAPTURE_VIDEOMODE_640x480YUV422, FLYCAPTURE_FRAMERATE_30);
	error = flycaptureStart( context, FLYCAPTURE_VIDEOMODE_ANY, FLYCAPTURE_FRAMERATE_ANY);
	
	if ( error != FLYCAPTURE_OK)return 1;

	// �摜�̃T�C�Y�𓾂邽�߂Ɉ�x�L���v�`���[����
	//
	error = flycaptureGrabImage2( context, &rawImage);
	if ( error != FLYCAPTURE_OK){
		cout<<"client flycaptureGrabImage2 "<<endl;
		return 1;
	}
	colorImage = rawImage;
	buf = cv::Mat_<cv::Vec3b>(colorImage.iRows,colorImage.iCols);

	// �x�C���[�摜 -> �J���[�摜�ϊ��p�̃p�����[�^�Z�b�g
	//
	colorImage.pixelFormat = FLYCAPTURE_BGR;
	colorImage.pData = (unsigned char*)buf.data; // �ϊ���̃o�b�t�@��OpenCV�̉摜�̃o�b�t�@�Ɏw��

	return 0;
}


int PointGrayCamera::Capture() {
	error = flycaptureGrabImage2( context, &rawImage);
	if (error != FLYCAPTURE_OK){
		cout<<"error flycaptureGrabImage2" <<endl;
		return 1;
	}
	error = flycaptureConvertImage(context, &rawImage, &colorImage);
    	if (error != FLYCAPTURE_OK){
			cout<<"error flycaptureConvertImage"<<endl;
			return 1;
		}

	colorImage.pData = (unsigned char*)buf.data; // �ϊ���̃o�b�t�@��OpenCV�̉摜�̃o�b�t�@�Ɏw��
	buf.copyTo(cv_image);


	return 0;
}


int PointGrayCamera::dst(void){

	flycaptureStop( context);
	flycaptureDestroyContext( context);
	return 0;
}


