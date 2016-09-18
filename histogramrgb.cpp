#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  int width, height;
  VideoCapture cap(0); //captures from the "0" (main) connected camera
  vector<Mat> layers;
  Mat histRed, histGreen, histBlue;
  int nbins = 64;
  float range[] = {0, 256};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;

  cap.open(0); //opening the camera for capturing frames

  if(!cap.isOpened()){
    cout << "unavailable cameras";
    return -1;
  }

  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "width = " << width << endl;
  cout << "height  = " << height << endl;

  int histw = nbins, histh = nbins/2; //histw: histogram's graphic width && histh: histogram's graphic height
  Mat histImgRed(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgGreen(histh, histw, CV_8UC3, Scalar(0,0,0));
  Mat histImgBlue(histh, histw, CV_8UC3, Scalar(0,0,0));

  while(1){
    cap >> image;
    split (image, layers); //divides the 3D colored matrix into 3 layers for each color (Red, Green and Blue)
    calcHist(&layers[0], 1, 0, Mat(), histRed, 1,&nbins, &histrange,uniform, acummulate);
    calcHist(&layers[1], 1, 0, Mat(), histGreen, 1, &nbins, &histrange,uniform, acummulate);
    calcHist(&layers[2], 1, 0, Mat(), histBlue, 1, &nbins, &histrange, uniform, acummulate);

    normalize(histRed, histRed, 0, histImgRed.rows, NORM_MINMAX, -1, Mat()); //they all have the same number of rows
    normalize(histGreen, histBlue, 0, histImgRed.rows, NORM_MINMAX, -1, Mat());
    normalize(histBlue, histBlue, 0, histImgRed.rows, NORM_MINMAX, -1, Mat());

    histImgRed.setTo(Scalar(0));
    histImgGreen.setTo(Scalar(0));
    histImgBlue.setTo(Scalar(0)); 

    //plotting histograms for each layer
    for(int i=0; i<nbins; i++){
      line(histImgRed, Point(i, histh),Point(i, cvRound(histRed.at<float>(i))),Scalar(0, 0, 255), 1, 8, 0);
      line(histImgGreen, Point(i, histh),Point(i, cvRound(histGreen.at<float>(i))),Scalar(0, 255, 0), 1, 8, 0);
      line(histImgBlue, Point(i, histh),Point(i, cvRound(histBlue.at<float>(i))),Scalar(255, 0, 0), 1, 8, 0);
    }
    //copying to the main image
    histImgRed.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgGreen.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgBlue.copyTo(image(Rect(0, 2*histh ,nbins, histh)));
    imshow("image", image);
    imwrite("rgbhist.jpg",image);
    if(waitKey(30) >= 0) break;
  }
  return 0;
}