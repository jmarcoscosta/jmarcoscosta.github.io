#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;
void iterative_threshold(Mat &image){
	int image_mean=mean(image)[0];
  	double minval,maxval;

  	minMaxIdx(image,&minval,&maxval);
  	double arith_mean=0.5*(minval+maxval);
  	double thresh= 0.5*(arith_mean+image_mean);
	threshold(image,image,int(thresh),255,THRESH_BINARY);

}


int main(){
	Mat image=imread("gab.jpg",CV_LOAD_IMAGE_GRAYSCALE);
	if(image.cols>image.rows){	
	flip(image,image,0);
 	transpose(image,image);
	}
	for(int i=0;i<5;i++){
	blur(image,image,Size(2,2));
	iterative_threshold(image);
 	}
  	Rect regiao_quadrado[4];
  	regiao_quadrado[0]=Rect(0,0,image.rows/6,image.cols/6);
  	
 	Mat quadrado=image(regiao_quadrado[0]);
	
 	
	int area_quadrado[4]={0};
 	CvPoint origem[4];
 	for(int i=0;i<4;i++){
 		origem[i].x=0;
 		origem[i].y=0;
 	}
 	for(int i=0;i<quadrado.rows;i++){
 		for(int j=0;j<quadrado.cols;j++){
 			if(quadrado.at<uchar>(i,j)==0)area_quadrado[0]++;
 			if(area_quadrado[0]==1){
 				origem[0].x=j;
 				origem[0].y=i;
 			}
 		}
 	}
 	CvPoint	centros_quadrados_teoricos[4];
 	CvPoint centros_quadrados_reais[4];
 	int lado_quadrado[4];
 	lado_quadrado[0]=int(sqrt(area_quadrado[0]));
 	centros_quadrados_teoricos[0].x=origem[0].x+lado_quadrado[0]/2;
 	centros_quadrados_teoricos[0].y=origem[0].y+lado_quadrado[0]/2;
   	
	centros_quadrados_teoricos[2].x=centros_quadrados_teoricos[0].x;
	centros_quadrados_teoricos[2].y=centros_quadrados_teoricos[0].y+24*lado_quadrado[0];
	centros_quadrados_teoricos[1].x=centros_quadrados_teoricos[0].x+lado_quadrado[0]*17;
	centros_quadrados_teoricos[1].y=centros_quadrados_teoricos[0].x;
	centros_quadrados_teoricos[3].y=centros_quadrados_teoricos[2].y;
	centros_quadrados_teoricos[3].x=centros_quadrados_teoricos[2].x+16*lado_quadrado[0];


 	
    //cortando a imagem
	int corte_horizontal_x=centros_quadrados_teoricos[0].x+18*lado_quadrado[0];
	int corte_horizontal_y=centros_quadrados_teoricos[0].y+26*lado_quadrado[0];
	Rect corte_horizontal=Rect(0,0,corte_horizontal_x,corte_horizontal_y);
	image=image(corte_horizontal);
	regiao_quadrado[1]=Rect(4*image.cols/6,0, image.cols/3 , image.rows/6);
	regiao_quadrado[2]=Rect(0,5*image.rows/6,image.cols/6,image.rows/6);
  	regiao_quadrado[3]=Rect(5*image.cols/6,5*image.rows/6,image.cols/6 ,image.rows/6);




  
    for(int k=1; k<4 ; k++){
 	
 	quadrado=image(regiao_quadrado[k]);
	
	for(int i=0;i<3;i++){
	blur(quadrado,quadrado,Size(2,2));
	iterative_threshold(quadrado);
 	}

 	for(int i=0;i<quadrado.rows;i++){
 		for(int j=0;j<quadrado.cols;j++)if(quadrado.at<uchar>(i,j)==0)area_quadrado[k]++;
 	}
    lado_quadrado[k]=int(sqrt(area_quadrado[k]));
    }

    
    for(int k=1; k<4 ; k++){ //tentando achar o centro real
 	
 	quadrado=image(regiao_quadrado[k]);
	int x_prov=0,y_prov=0;

 	for(int i=0;i<quadrado.rows;i++){
 		for(int j=0;j<quadrado.cols;j++){
 			if(quadrado.at<uchar>(i,j)==0){
 				y_prov=i;
 				break;
 			}
 			}
 		}

  	for(int i=0;i<quadrado.rows;i++){
 		for(int j=0;j<quadrado.cols;j++){
 			if(quadrado.at<uchar>(j,i)==0){
 				x_prov=i;
 				break;
 			}
 			}
 		}	
 	centros_quadrados_reais[k].x=x_prov-lado_quadrado[k]/2;
 	centros_quadrados_reais[k].y=y_prov-lado_quadrado[k]/2;
 	}
    centros_quadrados_reais[0]=centros_quadrados_teoricos[0];
    centros_quadrados_reais[1].x+=4*image.cols/6;
    centros_quadrados_reais[2].y+=5*image.rows/6;
    centros_quadrados_reais[3].y+=5*image.rows/6;
    centros_quadrados_reais[3].x+=5*image.cols/6;
   
/* DESENHANDO CÍRCULOS PARA VER OS CENTROS
    circle(image,centros_quadrados_reais[2],6,CV_RGB(60,60,60),-1,CV_AA);
	circle(image,centros_quadrados_reais[0],6,CV_RGB(60,60,60),-1,CV_AA);
	circle(image,centros_quadrados_reais[1],6,CV_RGB(60,60,60),-1,CV_AA);
	circle(image,centros_quadrados_reais[3],6,CV_RGB(60,60,60),-1,CV_AA);   

	circle(image,centros_quadrados_teoricos[2],6,CV_RGB(120,120,120),-1,CV_AA);
	circle(image,centros_quadrados_teoricos[0],6,CV_RGB(120,120,120),-1,CV_AA);
	circle(image,centros_quadrados_teoricos[1],6,CV_RGB(120,120,120),-1,CV_AA);
	circle(image,centros_quadrados_teoricos[3],6,CV_RGB(120,120,120),-1,CV_AA); */
	

	Point2f teoricos[4]; //a função getPerspective só aceita Point2f
	Point2f reais[4];
	for(int k=0;k<4;k++){
		teoricos[k].x=centros_quadrados_teoricos[k].x;
		teoricos[k].y=centros_quadrados_teoricos[k].y;
		reais[k].x=centros_quadrados_reais[k].x;
		reais[k].y=centros_quadrados_reais[k].y;
	}
	Point2f auxiliar;
	auxiliar=teoricos[2];
	teoricos[2]=teoricos[3];
	teoricos[3]=auxiliar;	
	auxiliar=reais[2];
	reais[2]=reais[3];
	reais[3]=auxiliar;

	for(int k=0;k<4;k++){

		cout<<k<<"input x e y :"<<teoricos[k].x<<" "<<teoricos[k].y<<endl;
		cout<<k<<"output x e y:"<<reais[k].x<<" "<<reais[k].y<<endl;
	}



	Mat corrigida;

	Mat matriz_transformacao_perspectiva=getPerspectiveTransform(reais,teoricos);
	warpPerspective(image,corrigida,matriz_transformacao_perspectiva,image.size());



 /*	namedWindow("corrigida",0);
 	resizeWindow("corrigida",1600,900); 	
 	namedWindow("original",0);
 	resizeWindow("original",1600,900);*/
 	imwrite("corrigida.jpg",corrigida);
	imwrite("original.jpg",image);
	//waitKey(0);
}