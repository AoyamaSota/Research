#include<time.h>
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include "cv.h"
#include "PointGrayCamera.h"


#define KEY_DOWN(key) (GetAsyncKeyState(key) & 0x8000)
#define KEY_PUSH(key) (GetAsyncKeyState(key) & 0x0001)

PointGrayCamera camera;

// --------------------------------------------------------------------------
// main(�����̐��A�������X�g)
// ���C���֐��ł�
// �߂�l ����I��:0 �G���[:-1
// --------------------------------------------------------------------------

static int prefWidth = 640;					// Fullscreen mode width.
static int prefHeight = 480;				// Fullscreen mode height.
static int prefDepth = 32;					// Fullscreen mode bit depth.
static int prefRefresh = 0;					// Fullscreen mode refresh rate. Set to 0 to use default rate.



void init()
{

	camera.initServerCamera();

	// image buffer

	//cv::imwrite("xi_img.png", frame);


	// �E�B���h�E
	//cvNamedWindow("binalized");

	//cvResizeWindow("binalized", 0, 0);

}


int main(int argc, char **argv)
{
	int code = -1;

	init();
	// �J���}���t�B���^
	CvKalman *kalman = cvCreateKalman(4, 2);
	// �����U�s��̐ݒ�
	cvSetIdentity(kalman->measurement_matrix, cvRealScalar(1.0));
	cvSetIdentity(kalman->process_noise_cov, cvRealScalar(1e-5));
	cvSetIdentity(kalman->measurement_noise_cov, cvRealScalar(0.1));
	cvSetIdentity(kalman->error_cov_post, cvRealScalar(1.0));

	// ���������^�����f��
	kalman->DynamMatr[0] = 1.0; kalman->DynamMatr[1] = 0.0; kalman->DynamMatr[2] =1.0; kalman->DynamMatr[3] = 0.0;
	kalman->DynamMatr[4] = 0.0; kalman->DynamMatr[5] = 1.0; kalman->DynamMatr[6] = 0.0; kalman->DynamMatr[7] = 1.0;
	kalman->DynamMatr[8] = 0.0; kalman->DynamMatr[9] = 0.0; kalman->DynamMatr[10] = 1.0; kalman->DynamMatr[11] = 0.0;
	kalman->DynamMatr[12] = 0.0; kalman->DynamMatr[13] = 0.0; kalman->DynamMatr[14] = 0.0; kalman->DynamMatr[15] = 1.0;

	// ���C�����[�v
	while (1) {
		// �X�V
		//	if (!ardrone.update()) break;
		cv::Mat cap;

		camera.Capture();

		cap = camera.cv_image;

		

		cv::cvtColor(cap, cap, CV_RGB2GRAY);

		if (cap.empty()){
			printf(" a");
			return 0;
		}
		cv::threshold(cap, cap, 100, 255, cv::THRESH_OTSU);

	//	cv::imshow("bina", cap);
		//cv::waitKey(1);

		// �m�C�Y�̏���
		//	cv::morphologyEx(cap, cap, NULL, NULL, CV_MOP_CLOSE);


		// �d�S�����߂�
		CvMoments moment;
		/*cvMoments(&cap, &moments, 1);
		int my = (int)(moments.m01 / moments.m00);
		int mx = (int)(moments.m10 / moments.m00);
*/
		moment = cv::moments(cap, 0);
		double m00 = cvGetSpatialMoment(&moment, 0, 0);
		double m10 = cvGetSpatialMoment(&moment, 1, 0);
		double m01 = cvGetSpatialMoment(&moment, 0, 1);

		// �ϑ��l
		float m[] = {m10/m00, m01/m00 };
		CvMat measurement = cvMat(2, 1, CV_32FC1, m);

		// �C���t�F�[�Y
		const CvMat *correction = cvKalmanCorrect(kalman, &measurement);


		// �\���t�F�[�Y
		const CvMat *prediction = cvKalmanPredict(kalman);

	//	IplImage* image = NULL;
	//	 �\��
		cv::circle(cap, cvPointFrom32f(cvPoint2D32f(m[0], m[1])), 10, CV_RGB(255, 255, 0));
		cv::circle(cap, cvPointFrom32f(cvPoint2D32f(prediction->data.fl[0], prediction->data.fl[1])), 1, CV_RGB(255, 255, 0));
			//	cvCircle(image, cvPointFrom32f(cvPoint2D32f(prediction->data.fl[0], prediction->data.fl[1])), 10, CV_RGB(0, 255, 0));
			//cvShowImage("camera", image);
		cv::imshow("aaaaaaa", cap);
					code = cvWaitKey(1);

			if (code == 27) /* �L�[�������ƌ��݂̃V�~�����[�V�������I�� */
				break;

		
	}
	// ���������
	cvReleaseKalman(&kalman);



	return 0;
}

