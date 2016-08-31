
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include <iostream>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "cv.h"
#include "PointGrayCamera.h"
#include <GL/glut.h>


#define WIDTH 1024
#define HEIGHT 768

int corner(int argc, char **argv)
{
	IplImage *src_img, *dst_img;
	FILE *fp;
	fp = fopen("out.txt", "wt");

	// グレースケール画像として読み込み
	if (argc != 2 || (src_img = cvLoadImage("camera_edge.jpeg", CV_LOAD_IMAGE_GRAYSCALE)) == 0) return -1;

	dst_img = cvCreateImage(cvGetSize(src_img), IPL_DEPTH_8U, 1);

	// Cannyによるエッジ画像の作成
	cvCanny(src_img, dst_img, 50.0, 200.0); //ここの閾値は適当に設定

	// 座標の出力
	for (int y = 0; y<dst_img->height; y++){
		for (int x = 0; x<dst_img->width; x++){
			if (dst_img->imageData[x + dst_img->width * y] > 128) { //輝度値255の点なら
				fprintf(fp, "(%d,%d)\n", x, y);
			}
		}
	}
	fclose(fp);

}


