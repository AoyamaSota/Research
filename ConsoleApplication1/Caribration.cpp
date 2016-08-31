
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

	// マウスイベントを取得
	switch (event) {

	case cv::EVENT_LBUTTONDOWN:
		desc += "LBUTTON_DOWN";
		cam[i++] = x;
		cam[i++] = y;
		break;

	}
	
//	printf("aaaaaaa%d", cam[i-1]);
	// マウスボタン，及び修飾キーを取得
	if (flag & cv::EVENT_FLAG_LBUTTON)
		desc += " + LBUTTON";

	std::cout << desc << " (" << x << ", " << y << ")" << std::endl;
}

static void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case '\033':
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
		exit(1);


	}
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);	// 画面のクリア色(背景色)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1, 1, 1, 0.0);

	double gl_x = 300 * (2.0 / WIDTH) - 1.0;
	double gl_y = (double)(HEIGHT - 150) * (2.0 / HEIGHT) - 1.0;

	double x = 500 * (2.0 / WIDTH) - 1.0;
	double y = (double)(HEIGHT - 300) * (2.0 / HEIGHT) - 1.0;

	glRectf(gl_x, gl_y, x, y);
	glutSwapBuffers();




		camera.Capture();

		//	cv::resize(camera.cv_image, camera.cv_image, cv::Size(620, 480));

		////////////////////////////////二値化
		//	cv::medianBlur(camera.cv_image, camera.cv_image, 3);	//メディアンフィルタ

		cv::Mat gray_img;
		cvtColor(camera.cv_image, gray_img, CV_RGB2GRAY);
		if (gray_img.empty()) return ;

		// 固定の閾値処理
		cv::Mat bin_img, bininv_img, trunc_img, tozero_img, tozeroinv_img;
		//// 入力画像，出力画像，閾値，maxVa@l，閾値処理手法
		cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

		//bin_img = gray_img;

		cv::setMouseCallback("Binary", onMouse, 0);
		// 結果画像表示
		cv::namedWindow("Binary", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);

		cv::imshow("Binary", bin_img);



		//	cv::imwrite("Homography.jpeg", bin_img);


	

	
}

static void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{

	

	camera.initServerCamera();
	glutInitWindowPosition(1600, 30);	// 画面の出現位置
	glutInitWindowSize(640, 480);		// 画面サイズ(400*300px)
	glutInit(&argc, argv);				// GLUTの初期化
	glutInitDisplayMode(GLUT_RGBA);		// 画面のモード(RGBA色空間を使う)
	glutCreateWindow("Test");			// 画面の名前
	glutDisplayFunc(display);			// GLUTの再描画関数を登録
	glutKeyboardFunc(keyboard);
//	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutMainLoop();						// イベントループを繰り返し実行


	
	
	
	
		
	return 0;
}

//int main(int argc, /*const*/ char* argv[]){
//	cv::Mat img = cv::Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
//
//
//	
//	//modify by ggggnonaka start
//
//	rectangle(img, cv::Point(600/2, 300/2), cv::Point(1000/2, 600/2), cv::Scalar(200, 200, 200), -1, CV_AA);
//	//modify by ggggnonaka end]
//	
//	cv::namedWindow("drawing", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
//	cvMoveWindow("drawing", 1594, 30);
//	imshow("drawing", img);
//
//
//	PointGrayCamera camera;
//
//	camera.initServerCamera();
//
//
//
//	while (cvWaitKey(1) == -1){
//
//		camera.Capture();
//
//		//	cv::resize(camera.cv_image, camera.cv_image, cv::Size(620, 480));
//
//		////////////////////////////////二値化
//	//	cv::medianBlur(camera.cv_image, camera.cv_image, 3);	//メディアンフィルタ
//
//		cv::Mat gray_img;
//		cvtColor(camera.cv_image, gray_img, CV_RGB2GRAY);
//		if (gray_img.empty()) return -1;
//
//		// 固定の閾値処理
//		cv::Mat bin_img, bininv_img, trunc_img, tozero_img, tozeroinv_img;
//		//// 入力画像，出力画像，閾値，maxVa@l，閾値処理手法
//		cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
//
//		//bin_img = gray_img;
//
//		cv::setMouseCallback("Binary", onMouse, 0);
//		// 結果画像表示
//		cv::namedWindow("Binary", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
//
//		cv::imshow("Binary", bin_img);
//
//		
//		
//	//	cv::imwrite("Homography.jpeg", bin_img);
//
//	
//	}
//
//	FILE *fp;
//	fp = fopen("corner.txt", "w");
//
//	if (fp == NULL){
//		perror("error");
//		exit(1);
//	}
//
//	for (i = 0; i<8; i++)
//		fprintf(fp, "%d\n", cam[i]);
//
//	if (fclose(fp) != 0){
//		perror("error");
//		exit(1);
//	}
//
//	return 0;
//}