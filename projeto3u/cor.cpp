#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>
using namespace std;
using namespace cv;



int main(int argvc, char** argv){
	Mat image=imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
	
	if(image.cols>image.rows){	
	flip(image,image,0);
 	transpose(image,image);
	}
	
	for(int i=0;i<3;i++){
	blur(image,image,Size(2,2));
 	threshold(image,image,0,255,CV_THRESH_BINARY | CV_THRESH_OTSU);

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
 	lado_quadrado[0]=int(sqrt(area_quadrado[0])); //cout<<lado_quadrado[0];
 	
 	centros_quadrados_teoricos[0].x=origem[0].x+lado_quadrado[0]/2;
 	centros_quadrados_teoricos[0].y=origem[0].y+lado_quadrado[0]/2;
 	centros_quadrados_reais[0]=centros_quadrados_teoricos[0];
	int corte_horizontal_y=29.5*lado_quadrado[0];

	Rect corte_horizontal=Rect(0,0,image.cols,corte_horizontal_y);
	image=image(corte_horizontal);

  	regiao_quadrado[1]=Rect(4*image.cols/6,0, image.cols/3 , image.rows/6);
	regiao_quadrado[2]=Rect(0,5*image.rows/6,image.cols/6,image.rows/6);
  	regiao_quadrado[3]=Rect(5*image.cols/6,5*image.rows/6,image.cols/6 ,image.rows/6);


	centros_quadrados_teoricos[2].x=centros_quadrados_teoricos[0].x;
	centros_quadrados_teoricos[2].y=centros_quadrados_teoricos[0].y+26*lado_quadrado[0];
	centros_quadrados_teoricos[1].x=centros_quadrados_teoricos[0].x+lado_quadrado[0]*17.5;
	centros_quadrados_teoricos[1].y=centros_quadrados_teoricos[0].x;
	centros_quadrados_teoricos[3].y=centros_quadrados_teoricos[2].y;
	centros_quadrados_teoricos[3].x=centros_quadrados_teoricos[2].x+17.5*lado_quadrado[0];



	for(int k=1; k<4 ; k++){// achando os tamanhos dos lados e as áreas
 	
 	quadrado=image(regiao_quadrado[k]);
	

 	for(int i=0;i<quadrado.rows;i++){
 		for(int j=0;j<quadrado.cols;j++)if(quadrado.at<uchar>(i,j)==0)area_quadrado[k]++;
 	}
    lado_quadrado[k]=int(sqrt(area_quadrado[k]));
    }

	    
    for(int k=1; k<4 ; k++){ //achando os centros reais com a fórmula do centro de massa
 	
 	quadrado=image(regiao_quadrado[k]);
	int x_centro,y_centro;
	unsigned int somaM=0;
	for(int i=0; i<quadrado.rows;i++){
		for(int j=0;j<quadrado.cols;j++){
			quadrado.at<uchar>(i,j)=255-quadrado.at<uchar>(i,j); //deixando a imagem negativa
			somaM+=quadrado.at<uchar>(i,j); //calculando a "massa total"
		}
	}
	int soma_x=0,soma_y=0;
	for(int i=0; i<quadrado.rows;i++){
		for(int j=0;j<quadrado.cols;j++){
			soma_x+=quadrado.at<uchar>(i,j)*i;
			soma_y+=quadrado.at<uchar>(i,j)*j;

		}
	}
	x_centro=soma_x/somaM;
	y_centro=soma_y/somaM;
	centros_quadrados_reais[k].x=y_centro;
	centros_quadrados_reais[k].y=x_centro;
	for(int i=0; i<quadrado.rows;i++){
		for(int j=0;j<quadrado.cols;j++){
			quadrado.at<uchar>(i,j)=255-quadrado.at<uchar>(i,j); //"desnegativando"
		}
	}

 	}
centros_quadrados_reais[1].x+=4*image.cols/6;
centros_quadrados_reais[2].y+=5*image.rows/6;
centros_quadrados_reais[3].y+=5*image.rows/6;
centros_quadrados_reais[3].x+=5*image.cols/6;

/*circle(image,centros_quadrados_reais[2],6,CV_RGB(60,60,60),-1,CV_AA);
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

	/*for(int k=0;k<4;k++){

		cout<<k<<"input x e y :"<<teoricos[k].x<<" "<<teoricos[k].y<<endl;
		cout<<k<<"output x e y:"<<reais[k].x<<" "<<reais[k].y<<endl;
	}*/


	Mat corrigida;

	Mat matriz_transformacao_perspectiva=getPerspectiveTransform(reais,teoricos);
	warpPerspective(image,corrigida,matriz_transformacao_perspectiva,image.size());

	unsigned int matricula[10]={0},x_inicial,y_inicial,lin,col;
	unsigned long long int soma_local;
	unsigned long long int soma_total[10]={0};
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			soma_local = 0;
			x_inicial = centros_quadrados_reais[0].y + 4.5 * lado_quadrado[0] + j *lado_quadrado[0];
			y_inicial = centros_quadrados_reais[0].x + 4.5 * lado_quadrado[0] + i *lado_quadrado[0];
			for (col = y_inicial + lado_quadrado[0]/3; col < y_inicial + 2*lado_quadrado[0]/3; col++){
				for (lin = x_inicial + lado_quadrado[0]/3; lin < x_inicial + 2*lado_quadrado[0]/3; lin++){
					soma_local += corrigida.at<uchar>(lin,col);
					corrigida.at<uchar>(col,lin)=100;
				}
			}
			soma_total[j]=soma_local;
		}
		unsigned long long int min=area_quadrado[0]*255/9; 
		int min_pos=-1;//botei -1 só pra parar de aparecer warninr unitilized
		for(int i=0;i<10;i++){
			if(soma_total[i]<min){
				min=soma_total[i];
				min_pos=i;
			}
		}
		cout<<min_pos;

	}
		
	//for(int k=0;k<10;k++)cout<<matricula[k];
	cout<<endl;
	unsigned int respostas[10]={0};
	char resp_letras[10];
	//Achando o primeiro quadrado (totalmente branco) da tabela de respostas
	//servirá como comparativo pra saber se tá preenchido ou n

	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 5; j++){
			soma_local = 0;
			x_inicial = centros_quadrados_reais[0].y + 16.5 * lado_quadrado[0] + i*lado_quadrado[0];
			y_inicial = centros_quadrados_reais[0].x + 6.5 * lado_quadrado[0] + j*lado_quadrado[0];
			for (lin = x_inicial + lado_quadrado[0]/3; lin < x_inicial + 2 * lado_quadrado[0]/3; lin++){
				for (col = y_inicial + lado_quadrado[0]/3; col < y_inicial + 2*lado_quadrado[0]/3; col++){
					soma_local += corrigida.at<uchar>(lin,col);
					corrigida.at<uchar>(lin,col)=100;
				}
			}
			respostas[j] = soma_local;
		}
		unsigned long long int min=area_quadrado[0]*255/9;
		int min_pos=-1;
		for(int i=0;i<5;i++){
			if(respostas[i]<min){
				min=respostas[i];
				min_pos=i;
			}
		}
		switch(min_pos){
			case 0:
				resp_letras[i] = 'A';
			break;
			case 1:
				resp_letras[i] = 'B';
			break;
			case 2:
				resp_letras[i] = 'C';
			break;
			case 3:
				resp_letras[i] = 'D';
			break;
			case 4:
				resp_letras[i] = 'E';
			break;
		}
		cout<<resp_letras[i];

	}
	cout<<endl;





 /*	namedWindow("corrigida",0);
 	resizeWindow("corrigida",1600,900); 	
 	namedWindow("original",0);
 	resizeWindow("original",1600,900);*/
 
 	imwrite(argv[2],corrigida);
	//imwrite("original.jpg",image);
	//waitKey(0);
}