#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include  "cv.h"
#include "highgui.h"
#include <iostream>
#include <opencv2/opencv.hpp>

IplImage* GetThresholdedImage(IplImage* img){

IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
cvCvtColor(img, imgHSV, CV_BGR2HSV);

IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);

cvInRangeS(imgHSV, cvScalar(25, 40, 40), cvScalar (40, 255, 255), imgThreshed);

cvReleaseImage(&imgHSV);

return imgThreshed;

}

int main(){

FILE *file = fopen("/dev/ttyO1", "w");
int i=0;
char *buf;
static int posX = 0;
static int posY = 0;
int lastX = 0;
int lastY = 0;
int lastX1 = 0;
int lastX2 = 0;
int lastX3 = 0;
int lastX4 = 0;
int lastY1 = 0;
int lastY2 = 0;
int lastY3 = 0;
int lastY4 = 0;

CvCapture *capture = cvCreateCameraCapture(0);

while (i < 10){

IplImage* frame = 0;
frame = cvQueryFrame(capture);

if (!frame){
break;
}

IplImage* imgYellowThresh = GetThresholdedImage(frame);

CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
cvMoments(imgYellowThresh, moments, 1);

double moment10 = cvGetSpatialMoment(moments, 1, 0);
double moment01 = cvGetSpatialMoment(moments, 0, 1);
double area = cvGetCentralMoment(moments, 0, 0);

int lastX4 = lastX3;
int lastX3 = lastX2;
int lastX2 = lastX1;
int lastX1 = lastX;
int lastX = posX;
int lastY4 = lastY3;
int lastY3 = lastY2;
int lastY2 = lastY1;
int lastY1 = lastY;
int lastY = posY;

posX = moment10/area;
posY = moment01/area;

fprintf(file, "%d_%d", posX, posY);
printf ("position:%d_%d\n", posX, posY);

asprintf(&buf, "CaptureThresh.jpg"); 
cvSaveImage(buf, imgYellowThresh);

cvReleaseImage(&imgYellowThresh);

delete moments;

i++;

}

cvReleaseCapture(&capture);
return 0;
}
