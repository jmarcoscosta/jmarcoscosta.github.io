#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <string>

using namespace cv;
using namespace std;

int YHslider=0;
int YHslider_max;

int YLslider=0;
int YLslider_max;

int cslider=0;
int cslider_max;

int D0slider=0;
int D0slider_max;

char TrackbarName[50];

Mat image, final;




void shiftsDFT(Mat& image ){ 
  Mat tmp, A, B, C, D;

  image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
  int cx = image.cols/2;
  int cy = image.rows/2;

  // recombines the transforms's quadrants
  // A B   ->  D C
  // C D       B A
  A = image(Rect(0, 0, cx, cy));
  B = image(Rect(cx, 0, cx, cy));
  C = image(Rect(0, cy, cx, cy));
  D = image(Rect(cx, cy, cx, cy));

  A.copyTo(tmp);  D.copyTo(A);  tmp.copyTo(D);

  C.copyTo(tmp);  B.copyTo(C);  tmp.copyTo(B);
}

Mat MakeSpectrum (Mat &image){ //returns image in frequency domain
  vector<Mat> planes;
  planes.clear();
  Mat padded, complexImage;
  Mat_<float> realInput, zeros;
  int dft_M, dft_N;  
  dft_M = getOptimalDFTSize(image.rows);
  dft_N = getOptimalDFTSize(image.cols);
  copyMakeBorder(image, padded, 0,
                 dft_M - image.rows, 0,
                 dft_N - image.cols,
                 BORDER_CONSTANT, Scalar::all(0));

  zeros = Mat_<float>::zeros(padded.size());

  complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));
  realInput = Mat_<float>(padded);
  
  planes.push_back(realInput);
  planes.push_back(zeros);
  merge(planes, complexImage);
  dft(complexImage, complexImage);
  shiftsDFT(complexImage);
  return complexImage;


}
void ShowSpectrum(Mat &Im, string s="Spectrum"){ 
  Mat ImS;
  vector<Mat> planes;
  planes.clear();
  Im.copyTo(ImS);
  split(ImS,planes);
  planes[0] += Scalar::all(1);
  log(planes[0],planes[0]);
  normalize(planes[0], planes[0], 0, 1, CV_MINMAX);
  imshow(s,planes[0]);
  waitKey(0);

}



Mat getBack(Mat &Im){//frequency to space for exhibition
  Mat ImS;
  vector<Mat> planes;
  Im.copyTo(ImS);
  shiftsDFT(ImS);
  idft(ImS,ImS);
  planes.clear();
  split(ImS,planes);
  normalize(planes[0], planes[0], 0, 1, CV_MINMAX);
  return planes[0];
}




Mat MakeHomomorphical(Mat &Im,float GAMMA_H, float GAMMA_L, float c, float RADIUS){
  Mat filter=Im.clone();
  int dft_M = getOptimalDFTSize(Im.rows);
  int dft_N = getOptimalDFTSize(Im.cols); 
  Mat tmp(dft_M,dft_N,CV_32F);
  float D0=RADIUS*RADIUS,  D;
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
        D=(i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2);
        
        tmp.at<float> (i,j)= (GAMMA_H - GAMMA_L)*(1-exp(-c*(D/D0)))+GAMMA_L;
 
    }
  }
  resize(tmp,tmp,filter.size()); //making sure the filter will have the same size of the input image
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);
  return filter;
}

void hmmfilter(Mat &image ){
  Mat ln_image;
  ln_image = Mat_<float>(image);
  normalize(ln_image, ln_image, 0, 1, CV_MINMAX);
  Mat complexImage;
  ln_image+=Scalar::all(1);  
  log(ln_image,ln_image);
  complexImage=MakeSpectrum(ln_image);
  Mat filter=MakeHomomorphical(complexImage,YHslider,YLslider,cslider,D0slider);
  mulSpectrums(complexImage,filter,complexImage,0);
  final=getBack(complexImage);
  exp(final,final);
  normalize(final,final,0,1,CV_MINMAX);
}

void on_trackbar_YH(int, void*){
    hmmfilter(image);
    imshow("Homomorphical",final);

}
void on_trackbar_YL(int, void*){
    hmmfilter(image);
    imshow("Homomorphical",final);
}
void on_trackbar_c(int, void*){
    hmmfilter(image);
    imshow("Homomorphical",final);
}

void on_trackbar_D0(int, void*){
    hmmfilter(image);
    imshow("Homomorphical",final);
}

int main(int argc , char** argv){
  image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  YHslider_max=10;
  YLslider_max=9;
  cslider_max=100;
  D0slider_max=image.rows/2;
  namedWindow("Homomorphical",WINDOW_AUTOSIZE);
    //creating the trackbars
  
  sprintf( TrackbarName, "D0 x %d", D0slider_max );
  createTrackbar( TrackbarName, "Homomorphical",&D0slider, D0slider_max, on_trackbar_D0);
  //showing the trackbars
  on_trackbar_D0(D0slider, 0 );

  //creating the trackbars
  sprintf( TrackbarName, "c x %d", cslider_max );
  createTrackbar( TrackbarName, "Homomorphical",&cslider,cslider_max,on_trackbar_c);
  //showing the trackbars
  on_trackbar_c(cslider, 0 );
  
  //creating the trackbars
  sprintf( TrackbarName, "YH x %d", YHslider_max );
  createTrackbar( TrackbarName, "Homomorphical",&YHslider,YHslider_max,on_trackbar_YH);
  //showing the trackbars
  on_trackbar_YH(YHslider, 0 );

 //creating the trackbars
  sprintf( TrackbarName, "YL x %d", YLslider_max );
  createTrackbar( TrackbarName, "Homomorphical",&YLslider,YLslider_max,on_trackbar_YL);
  //showing the trackbars
  on_trackbar_YL(YLslider, 0 );
  waitKey(0);


}