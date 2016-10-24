#include <iostream>
#include "opencv2/opencv.hpp"
#include <stack>
#include <ctime>

using namespace std;
using namespace cv;



int main(int argc, char**argv){
 Mat image;
 Mat fromcanny;
 CvPoint auxiliar;
 stack<CvPoint> pixel_stack;
 image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  Mat final=image.clone();

 fromcanny=imread(argv[2],CV_LOAD_IMAGE_GRAYSCALE);
 for (int i=0;i<fromcanny.rows;i++){
  for(int j=0;j<fromcanny.cols;j++){
    auxiliar.x=j;
    auxiliar.y=i;
    if(fromcanny.at<uchar>(i,j)>0)pixel_stack.push(auxiliar);
  }
 }

int gray;
srand(time(0));
while(!pixel_stack.empty()){

    gray= rand()%255 + 1;
    circle(final,pixel_stack.top(),4,CV_RGB(gray,gray,gray),-1, CV_AA);
    pixel_stack.pop();
}

imshow("original",image);
imshow("final",final);
 
waitKey(0);
return 0;
}


