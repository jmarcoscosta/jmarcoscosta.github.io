#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <string>
//#define RADIUS 20
#include <typeinfo>
using namespace cv;
using namespace std;

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

Mat InsertNoise(Mat &Im, float freq, float gain){//returns image in frequency domain
  vector<Mat> planes;
  planes.clear();
  Mat ImS;
  Mat padded;
  int dft_M = getOptimalDFTSize(Im.rows);
  int dft_N = getOptimalDFTSize(Im.cols); 
  copyMakeBorder(Im, padded, 0,
                 dft_M - Im.rows, 0,
                 dft_N - Im.cols,
                 BORDER_CONSTANT, Scalar::all(0));
  ImS = Mat(padded.size(), CV_32FC2, Scalar(0));  
  MakeSpectrum(Im).copyTo(ImS);
  split(ImS,planes);
  float mean;
  mean = abs(planes[0].at<float>(dft_M/2,dft_N/2));

  planes[0].at<float>(dft_M/2 +freq, dft_N/2 +freq) +=gain*mean;
  planes[1].at<float>(dft_M/2 +freq, dft_N/2 +freq) +=gain*mean;
  planes[0].at<float>(dft_M/2 -freq, dft_N/2 -freq) =planes[0].at<float>(dft_M/2 +freq, dft_N/2 +freq);
  planes[1].at<float>(dft_M/2 -freq, dft_N/2 -freq) =-planes[1].at<float>(dft_M/2 +freq, dft_N/2 +freq);

  merge(planes,ImS);
  return ImS;

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

Mat MakeLowPass(Mat &Im, float RADIUS){
  Mat filter=Im.clone();
  int dft_M = getOptimalDFTSize(Im.rows);
  int dft_N = getOptimalDFTSize(Im.cols); 
  Mat tmp(dft_M,dft_N,CV_32F);
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      if((i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2) < RADIUS*RADIUS){
        tmp.at<float> (i,j) = 1.0;
      }
      else         tmp.at<float> (i,j) = 0.0;
 
    }
  }
  resize(tmp,tmp,filter.size()); //making sure the filter will have the same size of the input image
  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);
  return filter;
}

Mat MakeHighPass(Mat &Im, float RADIUS){
  Mat filter=Im.clone();
  int dft_M = getOptimalDFTSize(Im.rows);
  int dft_N = getOptimalDFTSize(Im.cols); 
  Mat tmp(dft_M,dft_N,CV_32F);
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      if((i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2) < RADIUS*RADIUS){
        tmp.at<float> (i,j) = 0.0;
      }
      else         tmp.at<float> (i,j) = 1.0;
 
    }
  }
  resize(tmp,tmp,filter.size()); //making sure the filter will have the same size of the input image

  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);
  return filter;
}

Mat MakeBandPass(Mat &Im, float MAX_RADIUS, float MIN_RADIUS){
  Mat filter=Im.clone();
  int dft_M = getOptimalDFTSize(Im.rows);
  int dft_N = getOptimalDFTSize(Im.cols); 
  Mat tmp(dft_M,dft_N,CV_32F);
  for(int i=0; i<dft_M; i++){
    for(int j=0; j<dft_N; j++){
      if((i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2) < MAX_RADIUS*MAX_RADIUS
        &&(i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2) > MIN_RADIUS*MIN_RADIUS){
        tmp.at<float> (i,j) = 1.0;
      }
      else         tmp.at<float> (i,j) = 0.0;  
    }
  }
  resize(tmp,tmp,filter.size()); //making sure the filter will have the same size of the input image

  Mat comps[]= {tmp, tmp};
  merge(comps, 2, filter);
  return filter;
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


int main(int argc , char** argv){
  Mat complexImage;
  Mat padded, filter;
  Mat image, tmp;
  Mat_<float> realInput, zeros;
  vector<Mat> planes;
  image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
 /* Mat ln_image;
  ln_image = Mat_<float>(image);
  normalize(ln_image, ln_image, 0, 1, CV_MINMAX);

  ln_image+=Scalar::all(1);  
  log(ln_image,ln_image);*/
  complexImage=MakeSpectrum(image);
  float FC;
  int option;
  cout<<"choose option"<<endl;
  cin>>option;
  switch(option){
    case 0:
      cout<<"Low Pass:"<<endl<<"FC:"<<endl;
      cin>>FC;
      filter=MakeLowPass(complexImage,FC);
      break;
    case 1:
      cout<<"High Pass:"<<endl<<"FC:"<<endl;
      cin>>FC;
      filter=MakeHighPass(complexImage,FC);
      break;
    case 2:
      cout<<"Band Pass:"<<endl<<"FC:"<<endl;
      float FC2;
      cin>>FC;
      cout<<"FC2:"<<endl;
      cin>>FC2;
      filter=MakeBandPass(complexImage,FC,FC2);
      break;
    case 3:
      cout<<"Homomorphical"<<endl<<"YH,YL,c,D0:"<<endl;
      float YH,YL,c,D0;
      cin>>YH>>YL>>c>>D0;
            //filter=MakeHomomorphical(complexImage,2,0.25,1,60);

      filter=MakeHomomorphical(complexImage,YH,YL,c,D0);
      break;
  }
 /* mulSpectrums(complexImage,filter,complexImage,0);
  Mat final=getBack(complexImage);
  exp(final,final);
  normalize(final,final,0,1,CV_MINMAX);*/
  imshow("original",image);
  mulSpectrums(complexImage,filter,complexImage,0);
  imshow("filtered",getBack(complexImage));
  waitKey(0);
 
}