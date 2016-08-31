#include <opencv2/opencv.hpp>
#include "cv.h"
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gl/glut.h>
#pragma comment(lib, "glut32.lib")

#define WIDTH 1280
#define HEIGHT 1000
#define SIZE 5
#define NUM 6000


/*
filname: 出力ファイル名
width: 画像横幅
height: 画像縦幅
bpp: 画像色深度(bits per pixel)
この例ではbpp=24専用
*/
bool bmp_out(char* filename, int width, int height, int bpp)
{
	unsigned char *buffer;

	int scanline = ((width * bpp + 31) & ~31) / 8; // 4byte境界
	int image_size = scanline * height;

	buffer = (unsigned char*)malloc(image_size);
	if (!buffer) return false;

	glFlush();
	glReadPixels(0, 0, width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer); /* 1 */

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	ZeroMemory(&bfh, sizeof(BITMAPFILEHEADER));
	bfh.bfType = ((WORD) 'B' | 'M' << 8);
	bfh.bfSize = sizeof(BITMAPFILEHEADER);
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER); /* 2 */

	ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = width;
	bih.biHeight = height;
	bih.biPlanes = 1;
	bih.biBitCount = bpp;

	FILE *fp = fopen(filename, "w");
	if (fp) {
		fwrite(&bfh, sizeof(BITMAPFILEHEADER), 1, fp);
		fwrite(&bih, sizeof(BITMAPINFOHEADER), 1, fp);
		/* 3 */
		fwrite(buffer, image_size, 1, fp);
		fclose(fp);
	}
	else {
		free(buffer);
		return false;
	}

	free(buffer);
	return true;
}

void Point(int x, int y, float size){
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}
void display(void)
{


	int i, dis1, dis2, size;
	float r, g, b;
	srand((unsigned)time(NULL));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(1, 1, 1, 1.0f);

	for (i = 0; i <= NUM; i++){
	//	size = 5;
		/*size = (1 + (rand() % 8));
		r = (1 + (rand() % 10)) / 10;
		g = (1 + (rand() % 10)) / 10;
		b = (1 + (rand() % 10)) / 10;*/
		//glColor4f(r, g, b, 1.0f);
		dis1 = SIZE * 2 + (rand() % WIDTH); //ドットのx座標
		dis2 = SIZE * 2 + (rand() % HEIGHT); //ドットのy座標
	
		Point(dis1, dis2, SIZE);
	}

	glFlush();

	bmp_out("randomDot.bmp", WIDTH, HEIGHT, 24);
}
void Init(){
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
}
int main(int argc, char *argv[])
{
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("Dot");
	glutDisplayFunc(display);


	Init();
	glutMainLoop();


	return 0;
}

