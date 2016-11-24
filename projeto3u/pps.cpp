#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

using namespace cv;

int main(int argvc, char** argv){

	Mat image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	if(image.cols>image.rows){	
	flip(image,image,0);
 	transpose(image,image);
	}
	



	namedWindow("teste",0);
 	resizeWindow("teste",1600,900);
	imshow("teste",image);
	waitKey(0);
}