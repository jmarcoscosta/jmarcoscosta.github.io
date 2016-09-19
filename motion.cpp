#include <iostream>
#include <stack>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


double compare_hist(Mat h1o, Mat h2o){
  Mat h1,h2;
  h1o.copyTo(h1);
  h2o.copyTo(h2);
  double difference=0;
  if(h1.rows != h1.rows){
    cout<<"different size"<<endl;
    return -1;
  }
  if(h1.cols != h1.cols){
    cout<<"different size"<<endl;
    return -1;
  }

  for(int i=0;i<max(h1.cols,h1.rows);i++){
    difference+=abs(h1.at<float>(i)-h2.at<float>(i));
  }

  return difference/=max(h1.cols,h1.rows);



}
int main(int argc, char** argv){

  Mat image,greyframe,greyframe2;
  int width, height;
  VideoCapture cap(argv[1]);
  Mat histogram1,histogram2;
  int nbins = 64;
  float range[] = {0, 256};
  stack<Mat> mstack;
  const float *histrange = { range };
  bool uniform = true;
  bool accumulate = false;
  int counter=0;
  double difference;
  double limit;
  cout<<"What is the limit for detecting a movement?"
  cin>>limit;

  if(!cap.isOpened()){
    cout << "video not loaded";
    return -1;
  }
 
  width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImg(histh, histw, CV_8UC1, Scalar(0,0,0));

 while(1){
    
    counter++;
    
    if(mstack.empty()){
         cap >> image;
         cvtColor(image,greyframe,CV_RGB2GRAY);
         color_reduce(greyframe,greyframe,32);
         mstack.push(greyframe);
    }
   
     
    if(counter%60==0){//compares a k-histogram with a (k+60)-histogram
      
      cap >> image;
      cvtColor(image,greyframe2,CV_RGB2GRAY);
      calcHist(&greyframe2, 1, 0, Mat(), histogram2, 1,&nbins, &histrange,uniform, accumulate);
      calcHist(&mstack.top(), 1, 0, Mat(), histogram1, 1,&nbins, &histrange,uniform, accumulate);  


      mstack.pop();
      
      difference=compare_hist(histogram1,histogram2);
      if(difference>=limit){
        cout<<"CALL THE COPS!"<<endl;
        return -1;
      }
      
      
    }
  
    namedWindow("motion",0);
    resizeWindow("motion",400,600);  
    imshow("motion",image);
    if(waitKey(1) >= 0) break;
  }

  
  return 0;

}