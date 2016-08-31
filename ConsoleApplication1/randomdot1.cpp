#include <opencv2/opencv.hpp>
#include "cv.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 1600
#define HEIGHT 1024
#define size 1
#define NUM 28000


int main(int argc, char *argv[])
{
	cv::Mat img = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
	cv::Scalar color = (255, 255, 255);
	int i, dis1, dis2, SIZE;
	//double 
	float r, g, b;

	srand((unsigned)time(NULL));

	

	for (i = 0; i <= NUM; ){

		//	size = 5;
	//	SIZE = (1 + (rand() % size));
		SIZE = size;
		/*
		r = (1 + (rand() % 10)) / 10;
		g = (1 + (rand() % 10)) / 10;
		b = (1 + (rand() % 10)) / 10;*/

		dis1 =/* SIZE * 2 + */ /*(SIZE) +*/ (rand() % (WIDTH - SIZE)); //ドットのx座標
		dis2 =/* SIZE * 2 +*/  /*(SIZE) +*/ (rand() % (HEIGHT - SIZE)); //ドットのy座標



		if (dis1 >= SIZE && dis2 >= SIZE){
			/*if (img.at<cv::Vec3b>(dis2, dis1)[0] != 255 && img.at<cv::Vec3b>(dis2 + SIZE / 2, dis1)[0] != 255 && img.at<cv::Vec3b>(dis2, dis1 + SIZE / 2)[0] != 255
				&& img.at<cv::Vec3b>(dis2 + SIZE / 2, dis1 + SIZE / 2)[0] != 255 && img.at<cv::Vec3b>(dis2 + SIZE, dis1)[0] != 255
				&& img.at<cv::Vec3b>(dis2, dis1 + SIZE)[0] != 255 && img.at<cv::Vec3b>(dis2 + SIZE, dis1 + SIZE)[0] != 255 && img.at<cv::Vec3b>(dis2 + 1, dis1)[0] != 255
				&& img.at<cv::Vec3b>(dis2, dis1 + 1)[0] != 255 && img.at<cv::Vec3b>(dis2 + 1, dis1 + 1)[0] != 255 && img.at<cv::Vec3b>(dis2 + 3, dis1)[0] != 255
				&& img.at<cv::Vec3b>(dis2, dis1 + 3)[0] != 255 && img.at<cv::Vec3b>(dis2 + 3, dis1 + 3)[0] != 255
				&& img.at<cv::Vec3b>(dis2 - SIZE / 2, dis1)[0] != 255 && img.at<cv::Vec3b>(dis2, dis1 - SIZE / 2)[0] != 255
				&& img.at<cv::Vec3b>(dis2 - SIZE / 2, dis1 - SIZE / 2)[0] != 255 && img.at<cv::Vec3b>(dis2 - SIZE, dis1)[0] != 255
				&& img.at<cv::Vec3b>(dis2, dis1 - SIZE)[0] != 255 && img.at<cv::Vec3b>(dis2 - SIZE, dis1 - SIZE)[0] != 255 && img.at<cv::Vec3b>(dis2 - 1, dis1)[0] != 255
				&& img.at<cv::Vec3b>(dis2, dis1 - 1)[0] != 255 && img.at<cv::Vec3b>(dis2 - 1, dis1 - 1)[0] != 255 && img.at<cv::Vec3b>(dis2 - 3, dis1)[0] != 255
				&& img.at<cv::Vec3b>(dis2, dis1 - 3)[0] != 255 && img.at<cv::Vec3b>(dis2 - 3, dis1 - 3)[0] != 255
				&& img.at<cv::Vec3b>(dis2+SIZE, dis1 - SIZE)[0] != 255 && img.at<cv::Vec3b>(dis2 - SIZE, dis1 + SIZE)[0] != 255 && img.at<cv::Vec3b>(dis2 - 1, dis1+1)[0] != 255
				&& img.at<cv::Vec3b>(dis2+1, dis1 - 1)[0] != 255 && img.at<cv::Vec3b>(dis2 - 2, dis1 + 2)[0] != 255 && img.at<cv::Vec3b>(dis2 - 3, dis1)[0] != 255
				&& img.at<cv::Vec3b>(dis2 + 2, dis1 - 2)[0] != 255 && img.at<cv::Vec3b>(dis2 - 3, dis1 + 3)[0] != 255 && img.at<cv::Vec3b>(dis2 + 3, dis1 - 3)[0] != 255){
*/
				i++;
				// 画像，円の中心座標，半径，色，線太さ，種類
				cv::circle(img, cv::Point(dis1, dis2), SIZE, cv::Scalar(255, 255, 255), -1, CV_AA);
				
			//		cv::rectangle(img, cv::Point(dis1, dis2), cv::Point(dis1+SIZE*2, dis2+SIZE*2), cv::Scalar(255, 255, 255), -1, CV_AA);
	//		}


		}
	}

	cv::namedWindow("drawing", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::moveWindow("drawing", 0, 0);
	cv::imshow("drawing", img);
	cv::waitKey(0);
	cv::imwrite("dp.jpg", img);
}