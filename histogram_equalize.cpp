#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image,greyframe;
  int width, height;
  VideoCapture cap(0);
  Mat histogram, histogram_to_equalize;
  int nbins = 255;
  float range[] = {0, 256};

  long hist[256];
  
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;


  if(!cap.isOpened()){
    cout << "video not loaded";
    return -1;
  }
 
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "width = " << width << endl;
  cout << "height  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImg(histh, histw, CV_8UC1, Scalar(0,0,0));


  while(1){
    cap >> image;
    
    cvtColor(image,greyframe,CV_RGB2GRAY); //coverting color: RGB to Grey
  
    calcHist(&greyframe, 1, 0, Mat(), histogram, 1,
             &nbins, &histrange,
             uniform, acummulate); //built-in function
    
    // calculates the histogram
    for(int i=0; i<256; i++){
      hist[i]=0;
    }
    for(int i=0;i<greyframe.rows;i++){
      for(int j=0;j<greyframe.cols;j++){
        hist[greyframe.at<uchar>(i,j)]++;
      }
    }
    // calculates the accumulate histogram
    for(int i=1; i<256; i++){
      hist[i] += hist[i-1];
    }
    // normalizes the histogram
    for(int i=0; i<256; i++){
      hist[i] = hist[i]*255/hist[255];
    }
    
    calcHist(&greyframe, 1, 0, Mat(), histogram_to_equalize, 1,
             &nbins, &histrange,
             true, true);
    
    normalize(histogram_to_equalize,histogram_to_equalize,0,histImg.rows,NORM_MINMAX,-1,Mat());

    histImg.setTo(Scalar(0));


    for(int i=0; i<nbins; i++){
      line(histImg, Point(i, histh),
           Point(i, cvRound(histogram.at<float>(i))), //bulding the little histogram graphic that appears on image
           Scalar(255), 1, 8, 0);
    }
    
    histImg.copyTo(greyframe(Rect(0, 0,nbins, histh)));//copying the graphic to the image


    Mat equalized_image(greyframe.rows,greyframe.cols,greyframe.type(),Scalar(0)); //empty image
    Mat equalized_image2(greyframe.rows,greyframe.cols,greyframe.type());//empty image

    equalizeHist(greyframe,equalized_image2); //built-in function for equalizing histogram
   
    for(int i=0;i<equalized_image.rows;i++){
      for(int j=0;j<equalized_image.cols;j++){
          equalized_image.at<uchar>(i,j)=(uchar) hist[greyframe.at<uchar>(i,j)];
}
}
      
    //those flippings are for showing the frames properly orientated 
    flip(greyframe,greyframe,1);
    flip(equalized_image,equalized_image,1);
    flip(equalized_image2,equalized_image2,1);
   
    namedWindow("image",0);
    resizeWindow("image",400,600);   
    namedWindow("equalized by me",0);
    resizeWindow("equalized by me",400,600);
    namedWindow("equalized by func",0);
    resizeWindow("equalized by func",400,600);    
    imshow("image", greyframe);
    imshow("equalized by me",equalized_image);
    imshow("equalized by func",equalized_image2);

    if(waitKey(30) >= 0) break;
  }

  
  return 0;

}
