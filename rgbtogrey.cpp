#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
	Mat colored_image;
	colored_image=imread(argv[1],CV_LOAD_IMAGE_COLOR);
	Mat greyscale_image(colored_image.rows,colored_image.cols,0);
	typedef Vec<uchar,3> Vec3b;
	Vec3b color;
	for(int i=0;i<colored_image.rows;i++){
		for(int j=0;j<colored_image.cols;j++){
			color=colored_image.at<Vec3b>(i,j);
			greyscale_image.at<uchar>(i,j)=color[0]*0.11 + color[1]*.59 +color[2]*0.30;
		}
	}
	imshow("Greyscale Image",greyscale_image);
	imwrite("rgbtogrey.jpg",greyscale_image);
	waitKey(5000);
}