#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv){
	Mat image;
	image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	
	unsigned height,width,N_h,N_v;
	height=image.rows;
	width=image.cols;
	cout<<"Height: "<<height<<" Width: "<<width<<endl;
	cout<<"Choose the number of horizontal parts:"<<endl;
	cin>>N_h;
	cout<<"Choose the number of vertical parts:"<<endl;
	cin>>N_v;
	unsigned regions_order[N_v*N_h],i,j,counter=0;
	Rect regions[N_v*N_h]; 
	
	for(j=0;j<N_v;j++){
		for(i=0;i<N_h;i++){
			Rect R(i*width/N_h,j*height/N_v,width/N_h,height/N_v);
			regions[counter]=R;
			counter++;
		}
	}

	
	for(i=0;i<N_v*N_h;i++){
		cout<<"Region: "<<i<<endl;
		cin>>regions_order[i];
	}
	
	Mat result(height,width,image.type());
	
	counter=0;
	for(j=0;j<height;j+=height/N_v){
		for(i=0;i<width;i+=width/N_h){
			Mat region_of_interest = result(Rect(i,j,width/N_h,height/N_v));
			image(regions[regions_order[counter]]).copyTo(region_of_interest);
			counter++;
		}
	}
	namedWindow("original",WINDOW_AUTOSIZE);
	namedWindow("result",WINDOW_AUTOSIZE);
	imshow("original",image);
	imshow("result",result);
	waitKey(20000);
}