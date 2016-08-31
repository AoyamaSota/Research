#include<time.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "cv.h"
#include "PointGrayCamera.h"
#include <GL/glut.h>
#pragma warning(disable:4996)
#define WIDTH 1280
#define HEIGHT 960

#define WIDTH 640
#define HEIGHT 480

cv::Mat bin_img;
CvMat *h;
PointGrayCamera camera;
CvMoments moment;
cv::Point2d img_center;
//CvPoint2D32f center[2] = {0,0};
CvPoint2D32f hand_center;
int i = 0, sum;
int flag;
cv::Mat mat2;
cv::Mat gray_img;
cv::Mat bininv_img, trunc_img, tozero_img, tozeroinv_img;
int counter = 0;
cv::Mat dst_img;
cv::Mat src_img;
cv::Mat projection ;
//cv::Mat
cv::Mat background;
cv::Mat pattern;
cv::Mat_<double> homo;
//int count=0;
int j = 0;
int sign=1;
double h_tmp[16] = {0};
cv::Size patch2(1280, 960);
cv::Size patch(640, 480);
DWORD startTime;
double timer;
int fps = 0;
int move = 0;
int condition;
// アニメーションの周期
#define CYCLE 1
// テクスチャオブジェクト
static GLuint texname;

std::vector<cv::Point2d> center;
std::vector<double> v;

//初期化
void init(void);

//ウインドウのリサイズ
static void resize(int w, int h);

// テクスチャ生成
//bool  loadTextureFromMat1(cv::Mat& image, GLuint* texture);

//アニメーション
static void idle(void);

//CVで処理した画像をGLのテクスチャに変換
static void getTexture(void);

//GLで表示
static void display(void);

// OpenGL の初期化
static void glInit(void);

static void keyboard(unsigned char key, int x, int y);


// asa
cv::Mat tmpmat;
cv::Mat tmpmat2;
std::vector<cv::Point2d> tnkpoint;

int main(int argc, char* argv[])
{
	center.push_back(cv::Point2d(0, 0));
	center.push_back(cv::Point2d(0, 0));
	for (int ii = 0; ii < 7; ii++)
		tnkpoint.push_back(cv::Point2d(0, 0));

	tmpmat = cv::Mat::zeros(HEIGHT + 100, WIDTH, CV_8UC1);
	cv::Rect rect(cv::Point(0, 100), cv::Point(WIDTH, HEIGHT + 100));
	tmpmat2 = tmpmat(rect);

	init();


	// GLUT の初期化
	glutInit(&argc, argv);

	glutInitWindowSize(640, 480);
	glutInitWindowPosition(1600, 30);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("Projection");
	//if (flag != 1){
	//	flag = 1;
	//	startTime = GetTickCount();
	//}
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	
	glInit();

	glutMainLoop();



	return 0;

}

static void glInit(void)
{

	// テクスチャメモリの確保
	glGenTextures(1, &texname);
	glBindTexture(GL_TEXTURE_2D, texname);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// 隠面消去
	//	glEnable(GL_DEPTH_TEST);
	// 背景色
	glClearColor(0.0, 0.0, 0.0, 1.0);

}

static void idle(void)
{
	glutPostRedisplay();
}
int sel = 1;

void init(void)
{



	camera.initServerCamera();

	//ホモグラフィ行列の算出
	const int NUM_POINTS = 4;

	// カメラでの座標
	FILE *fp;

	char data[8];

	if ((fp = fopen("corner.txt", "rt")) == NULL){ /*ファイルを開いて*/
		printf("file not open￥n");
		exit(1);
	}
	int i;
	double cam[8];

	for (i = 0; i < 8; i++){
		fgets(data, 8, fp);
		cam[i] = atof(data); /*文字列として読んだものを数値化*/
	}
	fclose(fp);


	CvMat src_point;
	//double cam[] = { 380,292, 350,155, 540,117, 568,254 };
	src_point = cvMat(NUM_POINTS, 2, CV_64FC1, cam);

	// プロジェクター画像での座標
	CvMat dst_point;
	double projector[] = { 500,150, 500,300, 300,300, 300,150 };
	dst_point = cvMat(NUM_POINTS, 2, CV_64FC1, projector);



	// homography行列を計算
	h = cvCreateMat(3, 3, CV_64FC1);
	cvFindHomography(&src_point, &dst_point, h); //ホモグラフィ行列を計算

	homo = h;

	h_tmp[0] = homo(0, 0);
	h_tmp[1] = homo(1, 0);
	h_tmp[2] = 0;
	h_tmp[3] = homo(2, 0);
	h_tmp[4] = homo(0, 1);
	h_tmp[5] = homo(1, 1);
	h_tmp[6] = 0;
	h_tmp[7] = homo(2, 1);
	h_tmp[8] = 0;
	h_tmp[9] = 0;
	h_tmp[10] = 0;
	h_tmp[11] = 0;
	h_tmp[12] = homo(0, 2);
	h_tmp[13] = homo(1, 2);
	h_tmp[14] = 0;
	h_tmp[15] = homo(2, 2);



	img_center.x = WIDTH/2;
	img_center.y = HEIGHT/2;


	//cv::Mat
//	int sel=3;

	//printf("pattern is 1 or 2 ");
	//scanf("%d", &sel);
	background = cv::imread("base2.jpg", 1);
	if (background.empty()) return;

	if (sel == 1){
	pattern = cv::imread("sampleDot.jpg", 1);
//	pattern = cv::imread("dp.jpg", 1);
	if (pattern.empty()) return;
	}
	
	else if (sel == 3){
		pattern = cv::imread("base2.jpg", 1);
		if (pattern.empty()) return;
	}

	printf("condition is 1 or 2 or 3 ");
	scanf("%d", &condition);
	dst_img = background.clone();
	src_img = pattern.clone();
	projection = background.clone();
	startTime = GetTickCount();

	// カルマンフィルタ
	CvKalman *kalman = cvCreateKalman(4, 2);
	// 共分散行列の設定
	cvSetIdentity(kalman->measurement_matrix, cvRealScalar(1.0));
	cvSetIdentity(kalman->process_noise_cov, cvRealScalar(1e-5));
	cvSetIdentity(kalman->measurement_noise_cov, cvRealScalar(0.1));
	cvSetIdentity(kalman->error_cov_post, cvRealScalar(1.0));

	// 等速直線運動モデル
	kalman->DynamMatr[0] = 1.0; kalman->DynamMatr[1] = 0.0; kalman->DynamMatr[2] = 1.0; kalman->DynamMatr[3] = 0.0;
	kalman->DynamMatr[4] = 0.0; kalman->DynamMatr[5] = 1.0; kalman->DynamMatr[6] = 0.0; kalman->DynamMatr[7] = 1.0;
	kalman->DynamMatr[8] = 0.0; kalman->DynamMatr[9] = 0.0; kalman->DynamMatr[10] = 1.0; kalman->DynamMatr[11] = 0.0;
	kalman->DynamMatr[12] = 0.0; kalman->DynamMatr[13] = 0.0; kalman->DynamMatr[14] = 0.0; kalman->DynamMatr[15] = 1.0;
}

static void getTexture(void){

	
//	startTime = GetTickCount();

	/*	DWORD endTime = GetTickCount();
			std::cout << (double)(endTime - startTime) << "ms" << std::endl;
	
		if ((double)(endTime - startTime) < 1000){
			fps++;
		}
		else{
			printf("fps = %d", fps);
			cv::waitKey();
		}*/
	//
	//
	//	if (count == 3){
	//		//flag = 1;
	//		count = 69;
	//		startTime = GetTickCount();
	//	}



	camera.Capture();


//	cv::imwrite("samp.jpg", camera.cv_image);


	////////////////////////////////二値化



	cvtColor(camera.cv_image, gray_img, CV_RGB2GRAY);

	if (gray_img.empty()){
		return;
	}


	//flag = 0;

	// 固定の閾値処理(大津の二値化)
	// 入力画像，出力画像，閾値，maxVa@l，閾値処理手法
//	cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
	//	cv::imshow("aaaaaaaa", bin_img);

	cv::threshold(gray_img, bin_img, 130, 255, cv::THRESH_BINARY );
//	cv::imwrite("binary.jpg", bin_img);
		cv::imshow("aaaaaaaa", bin_img);
	
	cv::erode(bin_img, bin_img, cv::Mat(), cv::Point(-1, -1), 1);

	//二値化画像の重心
	cv::medianBlur(bin_img, bin_img, 3);	//メディアンフィルタ
//	std::cout << center << std::endl;
	
	
	moment = cv::moments(bin_img, 0);
	double m00 = cvGetSpatialMoment(&moment, 0, 0);
	double m10 = cvGetSpatialMoment(&moment, 1, 0);
	double m01 = cvGetSpatialMoment(&moment, 0, 1);
	

	if (m10 == 0 && m01 == 0)
		return;

	center.erase(center.begin());
	
	center.push_back(cv::Point2d(WIDTH - (m10 / m00/*+i*/),  HEIGHT - m01 / m00));

	tnkpoint.erase(tnkpoint.begin());
	tnkpoint.push_back((center[1] - center[0]) * 3); //線形補間

	
	cv::Point2d tmppoint(0, 0);

	for (int ii = 0; ii < tnkpoint.size(); ii++)
		tmppoint += tnkpoint[ii];
	tmppoint.x /= (double)tnkpoint.size();
	tmppoint.y = tmppoint.y / (double)tnkpoint.size() + 100;


//	std::cout << center << std::endl;

//	std::cout << new_center + tmppoint << std::endl;

	
	bin_img.copyTo(tmpmat2);

	


	//手の画像をシフト
	cv::getRectSubPix(tmpmat, patch, img_center + tmppoint, bin_img, cv::BORDER_CONSTANT);
//	cv::imshow("tetete", bin_img);
	//center[1].x += i;
//	tmppoint.x -= i;
	tmppoint.y += i;
	cv::getRectSubPix(src_img, patch, center[1] + tmppoint, projection, cv::BORDER_CONSTANT);



	//if (center)
//	std::cout << center[1].x << std::endl;

	if ((center[1].x - center[0].x) > 1 && center[1].x > 380 && center[1].x < 390){
			std::cout << center[1].x - center[0].x << std::endl;
			v.push_back(center[1].x - center[0].x);		//手の速度の保存　pix/frame
		}


	if (fps % 2== 0){
		switch (condition){

		case 1:
			break;

		case 2:
			if ((center[1].x - center[0].x) > 1 && center[1].x > 380)	
				i += 3;
		
			else if ((center[1].x - center[0].x) < -1)
				i -= 1.5;
			break;

		case 3:
			if ((center[1].x - center[0].x) > 1 && center[1].x>380)
				i -= 3;
			else if ((center[1].x - center[0].x) < -1)
				i += 1.5;
		//	pattern = cv::imread("sample.bmp", 1);
			break;
		}
	}


	fps++;
	//	

	

	// マスク処理
	projection.copyTo(bin_img, bin_img);

	//cv::imwrite("masked.jpg", bin_img);
	//		src_img.copyTo(bin_img, bin_img);
	//	cv::medianBlur(bin_img, bin_img, 3);	//メディアンフィルタ	

	
	cv::flip(bin_img, bin_img, 0);
	//	cv::flip(projection, projection, 0);

//	cv::imshow("aaaaaaaa", bin_img);
	/******* テクスチャ更新 *******/

	glBindTexture(GL_TEXTURE_2D, texname);



//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bin_img.cols, bin_img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, bin_img.data);


//テクスチャの作成

	for (int y = 0; y < bin_img.size().height; ++y)
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, y, bin_img.size().width, 1, GL_RGB,
			GL_UNSIGNED_BYTE, bin_img.data + bin_img.step * y);
	}



	return;
}

static void display(void)
{

	

		getTexture();

		
		//if ((center[1].x - center[0].x) < -1)		//手が右に動くと投影しない
		//	return;

		// 画面クリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// テクスチャはそのまま表示する（ポリゴン色を無視する）
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	
	
		//	 図形描画
		glBindTexture(GL_TEXTURE_2D, texname);

		glEnable(GL_TEXTURE_2D);

		//	glColor3d(1, 0, 0);
		glBegin(GL_QUADS);

		glTexCoord2d(0.0, 1.0);
		glVertex2d(0, 0);

		glTexCoord2d(0.0, 0.0);
		glVertex2d(0, 480);

		glTexCoord2d(1.0, 0.0);
		glVertex2d(640, 480);

		glTexCoord2d(1.0, 1.0);
		glVertex2d(640, 0);

		glEnd();

		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		//ダブルバッファリング
		glutSwapBuffers();
	
		
	

}



static void resize(int w, int h)
{
	
	// ウィンドウ全体を表示領域にする
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixd(h_tmp);

	/* 変換行列の初期化 */
	//glLoadIdentity();

	//glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);

	//glViewport(0, 0, w, h);

}

static void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
	case 'Q':
	case '\033':
		FILE *fp;
		fp = fopen("sin3.txt", "w");

		if (fp == NULL){
			perror("error");
			exit(1);
		}

		for (i = 0; i != v.size(); i++)
			fprintf(fp, "%f\n", v[i]);

		if (fclose(fp) != 0){
			perror("error");
			exit(1);
		}
		exit(1);

	
	case 'z':
		condition = 1;
		break;

	case 'x':
		condition = 2;
		break;

	case 'c':
		condition = 3;
		break;

	//case 'P':
	//	pattern = cv::imread("sampleDot.jpg", 1);
	//	if (pattern.empty()) return;
	//	break;

	default:
		break;

	}
}




