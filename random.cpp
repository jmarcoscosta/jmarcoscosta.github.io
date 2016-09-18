#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include <time.h>
using namespace std;
using namespace cv;

int main(int argc, char** argv){
	Mat image,upper,lower,final_image;
	image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	srand(time(NULL)); //doing this, every time you execute the code, the random number will be different
	int random_region= rand() % 24; //generates a random number from 0 to 23
	int width,height;
	int possibilities[24][5]={
	{0,1,2,3}, //each row represents a sequence of the four regions
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
	height=image.size().height;
	width=image.size().width;
	Rect regions[4]; 
	regions[0]=Rect(0,0,width/2,height/2);
	regions[1]=Rect(width/2,0,width/2,height/2);
	regions[2]=Rect(0,height/2,width/2,height/2);
	regions[3]=Rect(width/2,height/2,width/2,height/2);
	//Rect is a OpenCV class used to select an image's region of interest
	//the coordinates you give are from the rectangle's top-left corner
	//Rect class constructor : (x coordinate, y coordinate, height, width)
	int a,b,c,d;
	a=possibilities[random_region][0];
	b=possibilities[random_region][1];
	c=possibilities[random_region][2];
	d=possibilities[random_region][3];
	hconcat(image(regions[a]),image(regions[b]),upper); //horizontal concatenation
	hconcat(image(regions[c]),image(regions[d]),lower); //horizontal concatenation
	vconcat(upper,lower,final_image); //vertical concatenation 
	imshow("final_image",final_image);
	imwrite("changed.jpg",final_image);
	waitKey(); //if you touch any key from the keyboard, the image will be closed
}