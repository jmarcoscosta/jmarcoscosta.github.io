#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;
/*
#define STEP 3
#define JITTER 3
#define RAIO 3*/

int STEP_SLIDER=1;
int STEP_SLIDER_MAX;

int JITTER_SLIDER=1;
int JITTER_SLIDER_MAX;

int RAIO_SLIDER=1;
int RAIO_SLIDER_MAX;

Mat image,points;
char TrackbarName[50];

void pointillism(Mat &im,int STEP, int RAIO, int JITTER){
  Mat frame;
  vector<int> yrange;
  vector<int> xrange;
  int width, height, gray;
  int x, y;
  srand(time(0));

  width=im.size().width;
  height=im.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = im.at<uchar>(x,y);
      circle(points,
             cv::Point(y,x),
             RAIO,
             CV_RGB(gray,gray,gray),
             -1,
             CV_AA);
    }
  }

}

void on_trackbar_STEP(int, void*){
    pointillism(image,STEP_SLIDER,RAIO_SLIDER,JITTER_SLIDER);
    imshow("points",points);

}
void on_trackbar_JITTER(int, void*){
    pointillism(image,STEP_SLIDER,RAIO_SLIDER,JITTER_SLIDER);
    imshow("points",points);
}
void on_trackbar_RAIO(int, void*){
    pointillism(image,STEP_SLIDER,RAIO_SLIDER,JITTER_SLIDER);
    imshow("points",points);
}


int main(int argc, char** argv){
  
  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

  STEP_SLIDER_MAX=15;
  JITTER_SLIDER_MAX=15;
  RAIO_SLIDER_MAX=15;
  namedWindow("points",WINDOW_AUTOSIZE);
    //creating the trackbars
  
  sprintf( TrackbarName, "STEP x %d", STEP_SLIDER_MAX );
  createTrackbar( TrackbarName, "points",&STEP_SLIDER, STEP_SLIDER_MAX, on_trackbar_STEP);
  //showing the trackbars
  on_trackbar_STEP(STEP_SLIDER, 0 );

  //creating the trackbars
  sprintf( TrackbarName, "JITTER x %d", JITTER_SLIDER_MAX );
  createTrackbar( TrackbarName, "points",&JITTER_SLIDER,JITTER_SLIDER_MAX,on_trackbar_JITTER);
  //showing the trackbars
  on_trackbar_JITTER(JITTER_SLIDER, 0 );
  
  //creating the trackbars
  sprintf( TrackbarName, "RAIO x %d", RAIO_SLIDER_MAX );
  createTrackbar( TrackbarName, "points",&RAIO_SLIDER,RAIO_SLIDER_MAX,on_trackbar_RAIO);
  //showing the trackbars
  on_trackbar_RAIO(RAIO_SLIDER, 0 );

  waitKey(0);
  bool save;
  cout<<"Save: For Yes, press 1, for No, press 0"<<endl;
  cin>>save;
  if(save)imwrite("pointillism.jpg",points);
}