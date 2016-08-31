
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
	
//	printf("aaaaaaa%d", cam[i-1]);
	// �}�E�X�{�^���C�y�яC���L�[���擾
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
	glClearColor(0.0, 0.0, 0.0, 0.0);	// ��ʂ̃N���A�F(�w�i�F)
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

		////////////////////////////////��l��
		//	cv::medianBlur(camera.cv_image, camera.cv_image, 3);	//���f�B�A���t�B���^

		cv::Mat gray_img;
		cvtColor(camera.cv_image, gray_img, CV_RGB2GRAY);
		if (gray_img.empty()) return ;

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

static void idle(void)
{
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{

	

	camera.initServerCamera();
	glutInitWindowPosition(1600, 30);	// ��ʂ̏o���ʒu
	glutInitWindowSize(640, 480);		// ��ʃT�C�Y(400*300px)
	glutInit(&argc, argv);				// GLUT�̏�����
	glutInitDisplayMode(GLUT_RGBA);		// ��ʂ̃��[�h(RGBA�F��Ԃ��g��)
	glutCreateWindow("Test");			// ��ʂ̖��O
	glutDisplayFunc(display);			// GLUT�̍ĕ`��֐���o�^
	glutKeyboardFunc(keyboard);
//	glutReshapeFunc(resize);
	glutIdleFunc(idle);
	glutMainLoop();						// �C�x���g���[�v���J��Ԃ����s


	
	
	
	
		
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
//		////////////////////////////////��l��
//	//	cv::medianBlur(camera.cv_image, camera.cv_image, 3);	//���f�B�A���t�B���^
//
//		cv::Mat gray_img;
//		cvtColor(camera.cv_image, gray_img, CV_RGB2GRAY);
//		if (gray_img.empty()) return -1;
//
//		// �Œ��臒l����
//		cv::Mat bin_img, bininv_img, trunc_img, tozero_img, tozeroinv_img;
//		//// ���͉摜�C�o�͉摜�C臒l�CmaxVa@l�C臒l������@
//		cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
//
//		//bin_img = gray_img;
//
//		cv::setMouseCallback("Binary", onMouse, 0);
//		// ���ʉ摜�\��
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