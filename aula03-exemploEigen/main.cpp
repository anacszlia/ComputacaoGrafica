#include <iostream>
using namespace std;

#include "PPM.hpp"
#include <Eigen/Dense>
using namespace Eigen;

#define _PI	3.14159265358979323846

float degree2rad(float ang) {
	float r = ang * (float)(_PI / 180.0);

	return r;
}

Matrix3f getRotacao(float anfGraus){

	float c= cos(degree2rad(anfGraus));
	float s= sin(degree2rad(anfGraus));
	Matrix3f R = Matrix3f::Identity();

	R(0,0)=c;
	R(0,1)=-s;
	R(1,0)=s;
	R(1,1)=c;

	return R;

}

void transf2D(PPM* imgE, PPM* imgS, Matrix3f M){

	if(!imgE->pixels)return;
	if(imgE->alt != imgS->alt || imgE->larg != imgS->larg){
		destruir(imgS);
		criar(imgS,imgE->larg,imgE->alt);
	}

	for(size_t xE=0;xE< imgE->larg;xE++){

		for(size_t yE=0; yE< imgE->alt;yE++){

			Vector3f ps = M* Vector3f(xE,yE,1.0f);
			int xS = round(ps.x());
			int yS = round(ps.y());
			if( coordValida(imgS, xS, yS)){
				setPixel(imgS, xS, yS,getPixel(imgE,xE,yE));
			}
		
		}

	}



}

void transf2DInv(PPM* imgE, PPM* imgS, Matrix3f M){

	if(!imgE->pixels)return;
	if(imgE->alt != imgS->alt || imgE->larg != imgS->larg){
		destruir(imgS);
		criar(imgS,imgE->larg,imgE->alt);
	}

	for(size_t xS=0;xS< imgE->larg;xS++){

		for(size_t yS=0; yS< imgE->alt;yS++){

			Vector3f pE = M* Vector3f(xS,yS,1.0f);
			int xE = round(pE.x());
			int yE = round(pE.y());
			if( coordValida(imgS, xE, yE)){
				setPixel(imgS, xS, yS,getPixel(imgE,xE,yE));
			}
		
		}

	}



}


Matrix3f getTranslacao(float tx, float ty)
{
	Matrix3f T = Matrix3f::Identity();
	T(0,2) = tx;
	T(1,2) = ty;
	return T;
}

int main(void)
{
	setlocale(LC_ALL, "Portuguese");

	//pontos do triângulo em coordenadas homogeneas 
	Vector3f pA = Vector3f(4.0f, 2.0f, 1.0f);
	Vector3f pB = Vector3f(8.0f, 2.0f, 1.0f);
	Vector3f pC = Vector3f(6.0f, 7.0f, 1.0f);


	//definindo uma matriz de translação
	Matrix3f mT = Matrix3f::Identity();
	mT(0,2) = 3; // Tx
	mT(1,2) = 1; // Ty

	//OU, usando uma funcao
	//Matrix3f mT =  getTranslacao(3, 1); 

	// Matriz x ponto
	Vector3f pA_  = mT * pA;
	Vector3f pB_  = mT * pB;
	Vector3f pC_  = mT * pC;


	//imprimindo a matriz
	cout << "Matriz:\n";
	cout << endl << mT << endl;

	//imprimindo os pontos
	cout << "Ponto A: " << pA_.transpose() << endl;
	cout << "Ponto B: " << pB_.transpose() << endl;
	cout << "Ponto C: " << pC_.transpose() << endl;


	//criando um vetor com os pontos
	vector<Vector3f> pontos;
	pontos.push_back(Vector3f(4.0f, 2.0f, 1.0f));
	pontos.push_back(Vector3f(8.0f, 2.0f, 1.0f));
	pontos.push_back(Vector3f(6.0f, 7.0f, 1.0f));

	for (int i = 0; i < pontos.size(); i++)
	{
		Vector3f p  = mT * pontos[i];
		cout << "Ponto " << i+1 << ": " << p.transpose() << endl;
	}
	
	PPM imgE,imgS;
	ler(&imgE,"numeros.ppm");
	float cx =(imgE.larg-1)/ 2.0f;
	float cy =(imgE.alt-1)/ 2.0f;

	Matrix3f T= getTranslacao(cx,cy);
	Matrix3f Tinv= getTranslacao(-cx,-cy);
	Matrix3f R= getRotacao(45.0f);
	Matrix3f M= T*R *Tinv;

	//transf2D(&imgE,&imgS, M);
	transf2DInv(&imgE,&imgS,M.inverse());
	gravar(&imgS,"numeros45.ppm");
	destruir(&imgE);
	destruir(&imgS);

	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}