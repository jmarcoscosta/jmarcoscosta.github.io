#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
	Mat image;
	CvPoint point1, point2;
	int height,width,negative,i,j; 
	
	image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	height=image.rows;
	width=image.cols;
	
	cout<<"Height:"<<height<<" Width:"<<width<<endl;
	
	cin>>point1.x;
	cin>>point1.y;
	cin>>point2.x;
	cin>>point2.y;
	
	for(i=point1.y;i<=point2.y+1;i++){
		for(j=point1.x;j<=point2.x+1;j++){
			negative=255-int(image.at<uchar>(i,j));
			image.at<uchar>(i,j)=negative;
		}
	}
	
	imshow("Negative",image);
	waitKey(5000);
	return 0;
}