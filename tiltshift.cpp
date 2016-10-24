#include <iostream>
#include <opencv2/opencv.hpp>
#include <cmath>

using namespace cv;
using namespace std;

double d;

int l1_slider=0;
int l1_slider_max;

int l2_slider=0;
int l2_slider_max;

int d_slider=0;
int d_slider_max=100;

Mat original,auxiliar, final;

char TrackbarName[50];

double calc_alpha(double x,double l1,double l2,double d1){
    return 0.5*(tanh((x-l1)/d)-tanh((x-l2)/d)); //returns alpha for weighing
}

void implement_addweight(Mat& m1, Mat& m2){ //makes weighing
  for(int i=2;i<m1.rows;i++){
    double a0 = calc_alpha(i,l1_slider,l2_slider,d);
    addWeighted(m1.row(i),a0, m2.row(i),1-a0,0.0,final.row(i));
  }
}

void on_trackbar_d(int, void*){
  d = (double) d_slider;
  implement_addweight(original,auxiliar);
  final.convertTo(final, CV_8UC3);
  imshow("tiltshift",final);
}

void on_trackbar_l1(int, void*){
  implement_addweight(original,auxiliar);  
  final.convertTo(final, CV_8UC3);
  imshow("tiltshift",final);
}

void on_trackbar_l2(int, void*){
  implement_addweight(original,auxiliar);
  final.convertTo(final, CV_8UC3);
  imshow("tiltshift",final);
}

void change_saturation(Mat& im, double s){
  vector<Mat> layers;
  cvtColor(im,im,CV_RGB2HSV);
  split(im,layers);
  scaleAdd(layers[1],s,Mat::zeros(im.rows,im.cols,layers[1].type()),layers[1]);
  cvtColor(im,im,CV_HSV2RGB);
/*
  equalizeHist(layers[0],layers[0]);
  equalizeHist(layers[1],layers[1]);
  equalizeHist(layers[2],layers[2]);*/

}



int main(int argvc, char** argv){

  original = imread(argv[1],CV_LOAD_IMAGE_COLOR);
  double s;
  bool saturate;
  cout<<"Do you want to change image saturation? Yes: 1 ; No: 0"<<endl;
  cin>>saturate;
  if(saturate){
  cout<<"Choose the rate (between 0.0 and 1.0 to decrease, and >1.0 to increase)"<<endl;
  cin>>s;
  change_saturation(original,s);
  }
  namedWindow("tiltshift", WINDOW_NORMAL); //creating the window
  auxiliar=original.clone();
  float media[] = {1,1,1,
                   1,1,1,
                   1,1,1};
  Mat mask = Mat(3,3,CV_32F,media);
  scaleAdd(mask,1/9.0,Mat::zeros(3,3,CV_32F), mask); //making the media kernel for the filter2D

  int filter_times;
  cout<<"You can apply the media filter for blurring X times. Define X:"<<endl;
  cin>>filter_times;  

  l1_slider_max=original.size().height;

  l2_slider_max=original.size().height;

  for(int i=0;i<filter_times;i++){
      //applying the filter for blurring
      filter2D(auxiliar, auxiliar, original.depth(), mask, Point(1,1), 0);
  }

  original.convertTo(original,CV_32FC3);
  auxiliar.convertTo(auxiliar,CV_32FC3);
  final = original.clone();

  
  //creating the trackbars
  sprintf( TrackbarName, "D x %d", d_slider_max );
  createTrackbar( TrackbarName, "tiltshift",&d_slider, d_slider_max, on_trackbar_d);
  //showing the trackbars
  on_trackbar_d(d_slider, 0 );

  //creating the trackbars
  sprintf( TrackbarName, "L1 x %d", l1_slider_max );
  createTrackbar( TrackbarName, "tiltshift",&l1_slider,l1_slider_max,on_trackbar_l1);
  //showing the trackbars
  on_trackbar_l1(l1_slider, 0 );
  
  //creating the trackbars
  sprintf( TrackbarName, "L2 x %d", l2_slider_max );
  createTrackbar( TrackbarName, "tiltshift",&l2_slider,l2_slider_max,on_trackbar_l2);
  //showing the trackbars
  on_trackbar_l2(l2_slider, 0 );

  waitKey(0);
  bool save;
  cout<<"Save: For Yes, press 1, for No, press 0"<<endl;
  cin>>save;
  if(save)imwrite("tiltshifted_image_equalized.jpeg",final);

  return 0;
}