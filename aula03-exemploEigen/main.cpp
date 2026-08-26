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

Matrix3f getCisalhamentoHorizontal(float shx)
{
	Matrix3f S = Matrix3f::Identity();
	S(0,1) = shx;
	return S;
}
Matrix3f getCisalhamentoVertical(float shx)
{
	Matrix3f S = Matrix3f::Identity();
	S(1,0) = shx;
	return S;
}
Matrix3f getReflexaoX()
{
	Matrix3f T = Matrix3f::Identity();
	T(1,1) = -1;
	return T;
}
Matrix3f getReflexaoY()
{
	Matrix3f T = Matrix3f::Identity();
	T(0,0) = -1;
	return T;
}

Matrix3f getEscala(float sx,float sy)
{
	Matrix3f T = Matrix3f::Identity();
	T(1,1) = sy;
	T(0,0) = sx;
	return T;
}

int main(void)
{
	setlocale(LC_ALL, "Portuguese");

	// //pontos do triângulo em coordenadas homogeneas 
	// Vector3f pA = Vector3f(4.0f, 2.0f, 1.0f);
	// Vector3f pB = Vector3f(8.0f, 2.0f, 1.0f);
	// Vector3f pC = Vector3f(6.0f, 7.0f, 1.0f);


	// //definindo uma matriz de translação
	// Matrix3f mT = Matrix3f::Identity();
	// mT(0,2) = 3; // Tx
	// mT(1,2) = 1; // Ty

	// //OU, usando uma funcao
	// //Matrix3f mT =  getTranslacao(3, 1); 

	// // Matriz x ponto
	// Vector3f pA_  = mT * pA;
	// Vector3f pB_  = mT * pB;
	// Vector3f pC_  = mT * pC;


	// //imprimindo a matriz
	// cout << "Matriz:\n";
	// cout << endl << mT << endl;

	// //imprimindo os pontos
	// cout << "Ponto A: " << pA_.transpose() << endl;
	// cout << "Ponto B: " << pB_.transpose() << endl;
	// cout << "Ponto C: " << pC_.transpose() << endl;


	// //criando um vetor com os pontos
	// vector<Vector3f> pontos;
	// pontos.push_back(Vector3f(4.0f, 2.0f, 1.0f));
	// pontos.push_back(Vector3f(8.0f, 2.0f, 1.0f));
	// pontos.push_back(Vector3f(6.0f, 7.0f, 1.0f));

	// for (int i = 0; i < pontos.size(); i++)
	// {
	// 	Vector3f p  = mT * pontos[i];
	// 	cout << "Ponto " << i+1 << ": " << p.transpose() << endl;
	// }
	
	PPM imgE,imgS;
	ler(&imgE,"numeros.ppm");
	float cx =(imgE.larg-1)/ 2.0f;
	float cy =(imgE.alt-1)/ 2.0f;

	// Matrix3f T= getTranslacao(cx,cy);
	// Matrix3f Tinv= getTranslacao(-cx,-cy);
	// Matrix3f R= getRotacao(45.0f);
	// Matrix3f M= T*R *Tinv;

	//transf2D(&imgE,&imgS, M);
	//transf2DInv(&imgE,&imgS,M.inverse());
	//gravar(&imgS,"numeros45.ppm");
	
	//q1 cisalhamento horizontal de 0.9 e reflexao em torno do eixo y
	Matrix3f Tc = getTranslacao(cx, cy);
	Matrix3f Tinv = getTranslacao(-cx, -cy);
	// Matrix3f Cis = getCisalhamentoHorizontal(-0.9);
	// Matrix3f Ref = getReflexaoY();
	// Matrix3f M = Tc * Cis * Ref * Tinv;
	// transf2DInv(&imgE,&imgS,M.inverse());
	// gravar(&imgS,"exer1.ppm");


	//q2 rotacaode 15g, escala de 0.5 para x e para y, rotacao de 20g e 
	//cisalhamnto horizontal de 0.7

	// Matrix3f R1 = getRotacao(15.0f);
	// Matrix3f S = getEscala(0.5f, 0.5f);
	// Matrix3f R2 = getRotacao(-20.0f);
	// Matrix3f Cis = getCisalhamentoVertical(0.7f);
	
	// Matrix3f M = Tc * R1 * S * R2 * Cis * Tinv;
	// transf2DInv(&imgE,&imgS,M.inverse());
	// gravar(&imgS,"exer2.ppm");

	//implemente uma transformação geométrica de rotação
//empregando o conceito de mapeamento inverso 


//q4 programa
	int opcao = 0;
	do {
		cout << "\n--- Menu de Transformacoes ---\n";
		cout << "1. Translacao\n";
		cout << "2. Rotacao\n";
		cout << "3. Escala\n";
		cout << "4. Cisalhamento Horizontal\n";
		cout << "5. Cisalhamento Vertical\n";
		cout << "6. Reflexao em torno do eixo X\n";
		cout << "7. Reflexao em torno do eixo Y\n";
		cout << "0. Aplicar e sair\n";
		cout << "Opcao: ";
		cin >> opcao;

		Matrix3f nova = Matrix3f::Identity();

		if (opcao == 1) {
			float tx, ty;
			cout << "tx ty: "; cin >> tx >> ty;
			nova = getTranslacao(tx, ty);
		} else if (opcao == 2) {
			float ang;
			cout << "Angulo (graus): "; cin >> ang;
			nova = getRotacao(ang);
		} else if (opcao == 3) {
			float sx, sy;
			cout << "sx sy: "; cin >> sx >> sy;
			nova = getEscala(sx, sy);
		} else if (opcao == 4) {
			float shx;
			cout << "shx: "; cin >> shx;
			nova = getCisalhamentoHorizontal(shx);
		} else if (opcao == 5) {
			float shy;
			cout << "shy: "; cin >> shy;
			nova = getCisalhamentoVertical(shy);
		} else if (opcao == 6) {
			nova = getReflexaoX();
		} else if (opcao == 7) {
			nova = getReflexaoY();
		} else if (opcao != 0) {
			cout << "Opcao invalida.\n";
			continue;
		}

		if (opcao != 0)
			M = nova * M;

	} while (opcao != 0);

	Matrix3f Mfinal = Tc * M * Tinv;
	criar(&imgS, imgE.larg, imgE.alt);
	transf2DInv(&imgE, &imgS, Mfinal.inverse());
	gravar(&imgS, nomeSaida);
	cout << "Imagem salva em: " << nomeSaida << endl;
	
	
	//q5 programa
	
	int nTrans = 0;
    string nomeEntrada, nomeSaida, mapeamento;
    vector<Matrix3f> transformacoes;

    string token;
    while (cin >> token) {
        if (token == "NTRANS") {
            cin >> nTrans;
        } else if (token == "IMGE") {
            cin >> nomeEntrada;
        } else if (token == "IMGS") {
            cin >> nomeSaida;
        } else if (token == "MAP") {
            cin >> mapeamento;
        } else if (token == "T") {
            float tx, ty; cin >> tx >> ty;
            transformacoes.push_back(getTranslacao(tx, ty));
        } else if (token == "R") {
            float ang; cin >> ang;
            transformacoes.push_back(getRotacao(ang));
        } else if (token == "S") {
            float sx, sy; cin >> sx >> sy;
            transformacoes.push_back(getEscala(sx, sy));
        } else if (token == "CI") {
            string eixo; cin >> eixo;
            float sh; cin >> sh;
            if (eixo == "H") transformacoes.push_back(getCisH(sh));
            else             transformacoes.push_back(getCisV(sh));
        } else if (token == "RE") {
            string eixo; cin >> eixo;
            if (eixo == "X") transformacoes.push_back(getRefX());
            else             transformacoes.push_back(getRefY());
        }
    }

    PPM imgE, imgS;
    if (!ler(&imgE, nomeEntrada)) return EXIT_FAILURE;

    int L = imgE.larg, A = imgE.alt, N = (int)transformacoes.size();
    criar(&imgS, L * N, A);

    float cx = (L - 1) / 2.0f;
    float cy = (A - 1) / 2.0f;
    Matrix3f Tc   = getTranslacao( cx,  cy);
    Matrix3f Tinv = getTranslacao(-cx, -cy);

    // M acumula as transformações em série (no espaço centrado)
    Matrix3f M = Matrix3f::Identity();

    for (int i = 0; i < N; i++) {
        M = transformacoes[i] * M;
        Matrix3f Mfinal = Tc * M * Tinv;
        int xOffset = i * L;

        if (mapeamento == "DIR") {
            aplicarDireto(&imgE, &imgS, Mfinal, xOffset);
        } else {
            aplicarInverso(&imgE, &imgS, Mfinal.inverse(), xOffset);
        }
    }

    gravar(&imgS, nomeSaida);
    cout << "Imagem salva em: " << nomeSaida << endl;
	
	
	destruir(&imgE);
	destruir(&imgS);

	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}