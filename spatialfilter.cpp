#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


void menu(){
  cout << "\npress a button to apply a filter: \n"
	"a - calculate module\n"
    "m - media\n"
    "g - gauss\n"
    "v - vertical\n"
	"h - horizontal\n"
    "l - laplacian\n"
  "o - laplacian & gauss\n"
	"esc - exit\n";
}

int main(int argvc, char** argv){

  float media[] = {1,1,1,
				   1,1,1,
				   1,1,1};
  float gauss[] = {1,2,1,
				   2,4,2,
				   1,2,1};
  float vertical[]={-1,0,1,
					  -2,0,2,
					  -1,0,1};
  float horizontal[]={-1,-2,-1,
					0,0,0,
					1,2,1};
  float laplacian[]={0,-1,0,
					 -1,4,-1,
					 0,-1,0};

  Mat cap, frame, frame32f, frameFiltered;
  Mat mask(3,3,CV_32F), mask1;
  Mat result, result1;
  double width, height;
  int absolut;
  char key;
  cap=imread(argv[1]);
  width=cap.size().width;
  height=cap.size().height;
  cout << "width=" << width << "\n";;
  cout << "height =" << height<< "\n";;

  namedWindow("spatialfilter",1);

  mask = Mat(3, 3, CV_32F, media); 
  scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
  swap(mask, mask1);
  absolut=1; 

  menu();
  cin>>key;
  cvtColor(cap, frame, CV_BGR2GRAY);
  imshow("original", frame);
  frame.convertTo(frame32f, CV_32F);


  switch(key){
  case 'a':
    absolut=!absolut;
    break;
  case 'm':
    mask = Mat(3, 3, CV_32F, media);
    scaleAdd(mask, 1/9.0, Mat::zeros(3,3,CV_32F), mask1);
    mask = mask1;
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0); 
    if(absolut)frameFiltered=abs(frameFiltered);
    frameFiltered.convertTo(result, CV_8U);
    hconcat(frame,result,result);
    imwrite("media_filter_result.jpg",result);
    break;
  case 'g':
    mask = Mat(3, 3, CV_32F, gauss);
    scaleAdd(mask, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
    mask = mask1;
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
    if(absolut)frameFiltered=abs(frameFiltered);
    frameFiltered.convertTo(result, CV_8U);
    hconcat(frame,result,result);
    imwrite("gaussian_filter_result.jpg",result);
    break;
  case 'h':
    mask = Mat(3, 3, CV_32F, horizontal);
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
    if(absolut)frameFiltered=abs(frameFiltered);
    frameFiltered.convertTo(result, CV_8U);
    hconcat(frame,result,result);
    imwrite("horizontal_filter_result.jpg",result);
    break;
  case 'v':
    mask = Mat(3, 3, CV_32F, vertical);
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
    if(absolut)frameFiltered=abs(frameFiltered);
    frameFiltered.convertTo(result, CV_8U);
    hconcat(frame,result,result);
    imwrite("vertical_filter_result.jpg",result);
    break;
  case 'l':
    mask = Mat(3, 3, CV_32F, laplacian);
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask, Point(1,1), 0);
    if(absolut)frameFiltered=abs(frameFiltered);
    frameFiltered.convertTo(result, CV_8U);
    hconcat(frame,result,result);
    imwrite("laplacian_filter_result.jpg",result);
    break;
  case 'o':
    Mat maskG = Mat(3, 3, CV_32F, gauss);
    scaleAdd(maskG, 1/16.0, Mat::zeros(3,3,CV_32F), mask1);
    maskG = mask1;
    Mat maskL=Mat(3,3,CV_32F,laplacian);
    filter2D(frame32f, frameFiltered, frame32f.depth(), maskG, Point(1,1), 0);
    filter2D(frameFiltered, frameFiltered, frameFiltered.depth(), maskL, Point(1,1), 0);
    if(absolut) frameFiltered=abs(frameFiltered);
    frameFiltered.convertTo(result, CV_8U);
    hconcat(frame,result,result);
    imwrite("lapgauss_filter_result.jpg",result);
    break;

  }    
  return 0;
}