#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "cv.h"
#include "PointGrayCamera.h"
#include <gl/glut.h>
#pragma comment(lib, "glut32.lib")


#define WIDTH 640
#define HEIGHT 480

#pragma warning(disable:4996)
int cam[8] = { 0 };
CvMat src;
int i = 0;
PointGrayCamera camera;




void onMouse(int event, int x, int y, int flag, void*)
{
	std::string desc;

	// �}�E�X�C�x���g���擾
	switch (event) {

	case cv::EVENT_LBUTTONDOWN:
		desc += "LBUTTON_DOWN";
		cam[i++] = x;
		cam[i++] = y;
		break;

	}



int main(int argc, /*const*/ char* argv[]){
		cv::Mat img = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
	
	
		
		//modify by ggggnonaka start
	
		rectangle(img, cv::Point(600/2, 300/2), cv::Point(1000/2, 600/2), cv::Scalar(200, 200, 200), -1, CV_AA);
		//modify by ggggnonaka end]
		
		cv::namedWindow("drawing", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cvMoveWindow("drawing", 1594, 30);
		imshow("drawing", img);
	
	
		PointGrayCamera camera;
	
		camera.initServerCamera();
	
	
	
		while (cvWaitKey(1) == -1){
	
			camera.Capture();
	
			//	cv::resize(camera.cv_image, camera.cv_image, cv::Size(620, 480));
	
			////////////////////////////////��l��
		//	cv::medianBlur(camera.cv_image, camera.cv_image, 3);	//���f�B�A���t�B���^
	
			cv::Mat gray_img;
			cvtColor(camera.cv_image, gray_img, CV_RGB2GRAY);
			if (gray_img.empty()) return -1;
	
			// �Œ��臒l����
			cv::Mat bin_img, bininv_img, trunc_img, tozero_img, tozeroinv_img;
			//// ���͉摜�C�o�͉摜�C臒l�CmaxVa@l�C臒l������@
			cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	
			//bin_img = gray_img;
	
			cv::setMouseCallback("Binary", onMouse, 0);
			// ���ʉ摜�\��
			cv::namedWindow("Binary", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	
			cv::imshow("Binary", bin_img);
	
			
			
		//	cv::imwrite("Homography.jpeg", bin_img);
	
		
		}
	
		FILE *fp;
		fp = fopen("corner.txt", "w");
	
		if (fp == NULL){
			perror("error");
			exit(1);
		}
	
		for (i = 0; i<8; i++)
			fprintf(fp, "%d\n", cam[i]);
	
		if (fclose(fp) != 0){
			perror("error");
			exit(1);
		}
	
		return 0;
	}