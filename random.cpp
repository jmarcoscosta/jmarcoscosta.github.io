#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <time.h>
using namespace std;
using namespace cv;

int main(int argc, char** argv){
	Mat image,upper,lower,final_image;
	image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	srand(time(NULL));
	int random_region= rand() % 24;
	int probabilities[24][5]={
	{0,1,2,3},
	{0,2,1,3},
	{0,3,2,1},
	{0,1,3,2},
	{0,2,3,1},
	{0,3,1,2},
	{1,0,3,2},
	{1,0,2,3},
	{1,2,0,3},
	{1,2,3,0},
	{1,3,0,2},	
	{1,3,2,0},	
	{2,1,0,3},	
	{2,1,3,0},	
	{2,3,1,0},	
	{2,3,0,1},	
	{2,0,3,1},	
	{2,0,1,3},	
	{3,0,1,2},	
	{3,1,0,2},	
	{3,0,2,1},	
	{3,1,2,0},	
	{3,2,0,1},	
	{3,2,1,0}};
	Rect regions[4];
	regions[0]=Rect(0,0,image.rows/2,image.cols/2);
	regions[1]=Rect(0,0.5*image.rows-1,image.rows/2,image.cols/2);
	regions[2]=Rect(0.5*image.cols-1,0,image.rows/2,image.cols/2);
	regions[3]=Rect(0.5*image.cols-1,0.5*image.rows-1,image.rows/2,image.cols/2);
	int a,b,c,d;
	a=probabilities[random_region][0];
	b=probabilities[random_region][1];
	c=probabilities[random_region][2];
	d=probabilities[random_region][3];
	hconcat(image(regions[a]),image(regions[b]),upper); //horizontal concatenation
	hconcat(image(regions[c]),image(regions[d]),lower); //horizontal concatenatio
	vconcat(upper,lower,final_image); //vertical concatenation 
	imshow("final_image",final_image);
	waitKey();
}