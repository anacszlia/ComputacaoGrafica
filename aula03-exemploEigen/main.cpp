#include <iostream>
using namespace std;

#include "PPM.hpp"
#include "PGM.hpp"
#include <Eigen/Dense>
#include <vector>
#include <algorithm>
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
template <typename T>
void transf2D(T* imgE, T* imgS, Matrix3f M){

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

template <typename T>
void transf2DInv(T* imgE, T* imgS, Matrix3f M){

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

template <typename T>
vector<int> getVizinhos(T* img, int x, int y){
	int d[8][2] = {{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{-1,1},{0,1},{1,1}};
	vector<int> vizinhos;
	for(int i=0;i<8;i++){
		int nx = x + d[i][0];
		int ny = y + d[i][1];
		if(coordValida(img,nx,ny)){
			vizinhos.push_back(getPixel(img,nx,ny));
		}
	}
	sort(vizinhos.begin(), vizinhos.end());
	return vizinhos;
}

void exercicio7(){
	PGM img;
	ler(&img, "picotepgm.pgm");
	//para ler uma imagem PGM e preencher os buracos/artefatos da imagem com o
	//valor da mediana, o qual deve ser calculado com base na intensidade de cor de pixels vizinhos.
	PGM imgS;
	criar(&imgS,img.larg,img.alt);
	float cx =(img.larg-1)/ 2.0f;
	float cy =(img.alt-1)/ 2.0f;
	Matrix3f Tc = getTranslacao(cx, cy);
	Matrix3f Tinv = getTranslacao(-cx, -cy);
	for(int i = 0; i < img.larg; i++){
		for(int j = 0; j < img.alt; j++){
			if(getPixel(&img, i, j) == 0){
				vector<int> vizinhos = getVizinhos(&img, i, j);
				if(vizinhos.size() > 0 && vizinhos.size() % 2 == 1){
					setPixel(&imgS, i, j, vizinhos[vizinhos.size()/2]);
				} else if(vizinhos.size() > 0 && vizinhos.size() % 2 == 0){
					setPixel(&imgS, i, j, (vizinhos[vizinhos.size()/2-1] + vizinhos[vizinhos.size()/2])/2);
				} else {
					setPixel(&imgS, i, j, 0);
				}
			} else {
				setPixel(&imgS, i, j, getPixel(&img, i, j));
			}
		}
	}
	gravar(&imgS,"picotePreenchido.pgm");


		
	int buracos = 0, preenchidos = 0;
	for(int i = 0; i < img.larg; i++)
		for(int j = 0; j < img.alt; j++)
			if(getPixel(&img, i, j) == 0) buracos++;

	cout << "Buracos encontrados: " << buracos << endl;


	for(int i = 0; i < imgS.larg; i++)
		for(int j = 0; j < imgS.alt; j++)
			if(getPixel(&imgS, i, j) == 0) preenchidos++;

	cout << "Preenchidos: " << preenchidos << endl;

}
// Q1: rotacao 30g + escala 1.5x com mapeamento inverso
void questao1(){
    PPM imgE, imgS;
    ler(&imgE, "numeros.ppm");
    criar(&imgS, imgE.larg, imgE.alt);
    float cx = (imgE.larg-1)/2.0f, cy = (imgE.alt-1)/2.0f;
    Matrix3f Tc = getTranslacao(cx,cy), Tinv = getTranslacao(-cx,-cy);
    Matrix3f M = Tc * getEscala(1.5f,1.5f) * getRotacao(30.0f) * Tinv;
    transf2DInv(&imgE, &imgS, M.inverse());
    gravar(&imgS, "q1.ppm");
	destruir(&imgE);
	destruir(&imgS);
}

// Q2: filtro de mediana 3x3 em PGM com ruido sal-e-pimenta
void questao2(){
    PGM imgE, imgS;
    ler(&imgE, "picotepgm.pgm");
    criar(&imgS, imgE.larg, imgE.alt);
    for(int x=0; x<imgE.larg; x++){
        for(int y=0; y<imgE.alt; y++){
            int v = getPixel(&imgE,x,y);
            if(v==0 || v==255){
                vector<int> viz;
                for(int dx=-1;dx<=1;dx++)
                    for(int dy=-1;dy<=1;dy++)
                        if(dx!=0||dy!=0)
                            if(coordValida(&imgE,x+dx,y+dy))
                                viz.push_back(getPixel(&imgE,x+dx,y+dy));
                sort(viz.begin(),viz.end());
                setPixel(&imgS,x,y, viz.empty()?v : viz[viz.size()/2]);
            } else {
                setPixel(&imgS,x,y,v);
            }
        }
    }
    gravar(&imgS, "q2.pgm");
	destruir(&imgE);
	destruir(&imgS);
}

// Q3: recorte (50,50)-(200,200), cisalhamento H 0.5 + reflexao X, cola de volta
void questao3(){
    PPM imgE, recorte, aux;
    ler(&imgE, "numeros.ppm");
    int x1=50,y1=50,x2=200,y2=200;
    int larg=x2-x1, alt=y2-y1;
    criar(&recorte,larg,alt);
    criar(&aux,larg,alt);
    for(int y=0;y<alt;y++)
        for(int x=0;x<larg;x++)
            setPixel(&recorte,x,y,getPixel(&imgE,x1+x,y1+y));
    float cx=(larg-1)/2.0f, cy=(alt-1)/2.0f;
    Matrix3f Tc=getTranslacao(cx,cy), Tinv=getTranslacao(-cx,-cy);
    Matrix3f M = Tc * getCisalhamentoHorizontal(0.5f) * getReflexaoX() * Tinv;
    transf2DInv(&recorte,&aux,M.inverse());
    for(int y=0;y<alt;y++)
        for(int x=0;x<larg;x++)
            setPixel(&imgE,x1+x,y1+y,getPixel(&aux,x,y));
    gravar(&imgE, "q3.ppm");
	destruir(&imgE);
	destruir(&recorte);
	destruir(&aux);
}

// Q4: imagem original na metade esquerda, reflexao Y (mapeamento direto) na direita
void questao4(){
    PPM imgE, imgS;
    ler(&imgE, "numeros.ppm");
    criar(&imgS, imgE.larg*2, imgE.alt);
    for(int y=0;y<imgE.alt;y++)
        for(int x=0;x<imgE.larg;x++)
            setPixel(&imgS,x,y,getPixel(&imgE,x,y));
    float cx=(imgE.larg-1)/2.0f, cy=(imgE.alt-1)/2.0f;
    Matrix3f Tc=getTranslacao(cx,cy), Tinv=getTranslacao(-cx,-cy);
    Matrix3f M = Tc * getReflexaoY() * Tinv;
    for(int y=0;y<imgE.alt;y++){
        for(int x=0;x<imgE.larg;x++){
            Vector3f ps = M * Vector3f(x,y,1.0f);
            int xS=round(ps.x()), yS=round(ps.y());
            if(coordValida(&imgE,xS,yS))
                setPixel(&imgS, imgE.larg+xS, yS, getPixel(&imgE,x,y));
        }
    }
    gravar(&imgS, "q4.ppm");
	destruir(&imgS);
}

// Q5: negativo na regiao (x1,y1)-(x2,y2), depois rotacao 90g com mapeamento inverso
void questao5(){
    PGM imgE, imgS, imgRot;
    ler(&imgE, "picotepgm.pgm");
    criar(&imgS, imgE.larg, imgE.alt);
    int x1=50,y1=50,x2=200,y2=200;
    for(int y=0;y<imgE.alt;y++)
        for(int x=0;x<imgE.larg;x++){
            int v = getPixel(&imgE,x,y);
            if(x>=x1&&x<x2&&y>=y1&&y<y2)
                setPixel(&imgS,x,y,255-v);
            else
                setPixel(&imgS,x,y,v);
        }
    criar(&imgRot, imgS.larg, imgS.alt);
    float cx=(imgS.larg-1)/2.0f, cy=(imgS.alt-1)/2.0f;
    Matrix3f Tc=getTranslacao(cx,cy), Tinv=getTranslacao(-cx,-cy);
    Matrix3f M = Tc * getRotacao(90.0f) * Tinv;
    transf2DInv(&imgS, &imgRot, M.inverse());
    gravar(&imgRot, "q5.pgm");
	destruir(&imgS);
	destruir(&imgRot);
}

void preencher_area3(PPM *pgm,PPM *pgm2,int x1,int y1,int x2,int y2){
    int i=0, j=0;
    for(int y=y1;y<=y2 && i<pgm2->alt;y++,i++){
		int j = 0;
       for(int x=x1;x<=x2 && j<pgm2->larg;x++,j++){
            if(coordValida(pgm2,i,j))
                setPixel(pgm,x,y,getPixel(pgm2,i,j));
       }
    }
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
	
	// Matrix3f T= getTranslacao(cx,cy);
	// Matrix3f Tinv= getTranslacao(-cx,-cy);
	// Matrix3f R= getRotacao(45.0f);
	// Matrix3f M= T*R *Tinv;

	//transf2D(&imgE,&imgS, M);
	//transf2DInv(&imgE,&imgS,M.inverse());
	//gravar(&imgS,"numeros45.ppm");
	
	//q1 cisalhamento horizontal de 0.9 e reflexao em torno do eixo y
	PPM imgE,imgS,aux;

	ler(&imgE,"numeros.ppm");

	int x1=18,y1=626,x2=174,y2=866;

	int larg=x2-x1;
	int alt=y2-y1;

	float cx=larg/2.0f;
	float cy=alt/2.0f;

	criar(&imgS,larg,alt);
	criar(&aux,larg,alt);

	for(int y=0;y<alt;y++){
		for(int x=0;x<larg;x++){
			setPixel(&imgS,x,y,getPixel(&imgE,x1+x,y1+y));
		}
	}

	RGB branco=RGB(255,255,255);

	for(int y=0;y<alt;y++){
		for(int x=0;x<larg;x++){
			setPixel(&aux,x,y,branco);
		}
	}

	Matrix3f Tc=getTranslacao(cx,cy);
	Matrix3f Tinv=getTranslacao(-cx,-cy);
	Matrix3f R=getRotacao(45.0f);

	Matrix3f M=Tc*R*Tinv;

	transf2D(&imgS,&aux,M);

	gravar(&aux,"exer91.ppm");

	preencher_area3(&imgE,&aux,x1,y1,x2,y2);

	gravar(&imgE,"exer9.ppm");

	// Matrix3f Esc = getEscala(2.0f, 2.0f);
	// Matrix3f M = Tc * Esc * Tinv;
	// transf2D(&imgE, &imgS, M);
	// gravar(&imgS, "picotepgm.pgm");

	
	//volta a imagem para a posicao original com a TInv
	// Matrix3f M2 = Tc * Cis * Ref * Tinv;
	// transf2D(&imgE,&imgS, M2);
	// gravar(&imgS,"testepicote2.ppm");
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
//comando para rodar o programa para reconhecer o eigen: g++ -I /usr/include/eigen3 main.cpp PPM.cpp -o main
	// int opcao = 0;
	// do {
	// 	cout << "\n--- Menu de Transformacoes ---\n";
	// 	cout << "1. Translacao\n";
	// 	cout << "2. Rotacao\n";
	// 	cout << "3. Escala\n";
	// 	cout << "4. Cisalhamento Horizontal\n";
	// 	cout << "5. Cisalhamento Vertical\n";
	// 	cout << "6. Reflexao em torno do eixo X\n";
	// 	cout << "7. Reflexao em torno do eixo Y\n";
	// 	cout << "0. Aplicar e sair\n";
	// 	cout << "Opcao: ";
	// 	cin >> opcao;

	// 	Matrix3f nova = Matrix3f::Identity();

	// 	if (opcao == 1) {
	// 		float tx, ty;
	// 		cout << "tx ty: "; cin >> tx >> ty;
	// 		nova = getTranslacao(tx, ty);
	// 	} else if (opcao == 2) {
	// 		float ang;
	// 		cout << "Angulo (graus): "; cin >> ang;
	// 		nova = getRotacao(ang);
	// 	} else if (opcao == 3) {
	// 		float sx, sy;
	// 		cout << "sx sy: "; cin >> sx >> sy;
	// 		nova = getEscala(sx, sy);
	// 	} else if (opcao == 4) {
	// 		float shx;
	// 		cout << "shx: "; cin >> shx;
	// 		nova = getCisalhamentoHorizontal(shx);
	// 	} else if (opcao == 5) {
	// 		float shy;
	// 		cout << "shy: "; cin >> shy;
	// 		nova = getCisalhamentoVertical(shy);
	// 	} else if (opcao == 6) {
	// 		nova = getReflexaoX();
	// 	} else if (opcao == 7) {
	// 		nova = getReflexaoY();
	// 	} else if (opcao != 0) {
	// 		cout << "Opcao invalida.\n";
	// 		continue;
	// 	}

	// 	if (opcao != 0)
	// 		M = nova * M;

	// } while (opcao != 0);

	// Matrix3f Mfinal = Tc * M * Tinv;
	// criar(&imgS, imgE.larg, imgE.alt);
	// transf2DInv(&imgE, &imgS, Mfinal.inverse());
	// gravar(&imgS, nomeSaida);
	// cout << "Imagem salva em: " << nomeSaida << endl;
	
	
	// q5 programa
	
	// int nTrans = 0;
    // string nomeEntrada, nomeSaida, mapeamento;
    // vector<Matrix3f> transformacoes;

    // string token;
    // while (cin >> token) {
    //     if (token == "NTRANS") {
    //         cin >> nTrans;
    //     } else if (token == "IMGE") {
    //         cin >> nomeEntrada;
    //     } else if (token == "IMGS") {
    //         cin >> nomeSaida;
    //     } else if (token == "MAP") {
    //         cin >> mapeamento;
    //     } else if (token == "T") {
    //         float tx, ty; cin >> tx >> ty;
    //         transformacoes.push_back(getTranslacao(tx, ty));
    //     } else if (token == "R") {
    //         float ang; cin >> ang;
    //         transformacoes.push_back(getRotacao(ang));
    //     } else if (token == "S") {
    //         float sx, sy; cin >> sx >> sy;
    //         transformacoes.push_back(getEscala(sx, sy));
    //     } else if (token == "CI") {
    //         string eixo; cin >> eixo;
    //         float sh; cin >> sh;
    //         if (eixo == "H") transformacoes.push_back(getCisH(sh));
    //         else             transformacoes.push_back(getCisV(sh));
    //     } else if (token == "RE") {
    //         string eixo; cin >> eixo;
    //         if (eixo == "X") transformacoes.push_back(getRefX());
    //         else             transformacoes.push_back(getRefY());
    //     }
    // }

    // PPM imgE, imgS;
    // if (!ler(&imgE, nomeEntrada)) return EXIT_FAILURE;

    // int L = imgE.larg, A = imgE.alt, N = (int)transformacoes.size();
    // criar(&imgS, L * N, A);

    // float cx = (L - 1) / 2.0f;
    // float cy = (A - 1) / 2.0f;
    // Matrix3f Tc   = getTranslacao( cx,  cy);
    // Matrix3f Tinv = getTranslacao(-cx, -cy);

    // M acumula as transformações em série (no espaço centrado)
    // Matrix3f M = Matrix3f::Identity();

    // for (int i = 0; i < N; i++) {
    //     M = transformacoes[i] * M;
    //     Matrix3f Mfinal = Tc * M * Tinv;
    //     int xOffset = i * L;

    //     if (mapeamento == "DIR") {
    //         aplicarDireto(&imgE, &imgS, Mfinal, xOffset);
    //     } else {
    //         aplicarInverso(&imgE, &imgS, Mfinal.inverse(), xOffset);
    //     }
    // }

    // gravar(&imgS, nomeSaida);
    // cout << "Imagem salva em: " << nomeSaida << endl;
	
	
	// destruir(&imgE);
	// destruir(&imgS);

	questao1();
	questao2();
	questao3();
	questao4();
	questao5();

	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}