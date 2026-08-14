#include <iostream>
#include "PGM.hpp"

using namespace std;

void exercicio1(){
	PGM img;
	criar(&img,100,200,128);
	gravar(&img,"saida_exerc1.pgm");
	destruir(&img);
}
void exercicio2(int linha){
	PGM img;
	criar(&img,100,200);
	setLinha(&img,linha,0);
	gravar(&img,"saida_exerc2.pgm");
	destruir(&img);
}
void exercicio3(){
	PGM img;
	criar(&img,100,200,128);
	cout<<(coordValida(&img,99,100)? "valida":" invalida")<<endl;
	destruir(&img);
}
void exercicio4(){
	PGM img;
	criar(&img,100,200,128);
	preencher_area2(&img,50,50,20,20);
	gravar(&img,"saida_exerc4.pgm");
	destruir(&img);
}


int main(void)
{
	/*
	//#Exemplo1#: criando uma imagem e gravando no disco
	PGM img1;
	criar(&img1, 5, 7);
	gravar(&img1, "exemplo1.pgm");
	destruir(&img1);

	//#Exemplo2#: imprimindo os atributos da imagem 
	PGM img2;
	cout << "#Exemplo 2#\n";
	imprimir(&img2);
	criar(&img2, 700, 500);
	imprimir(&img2);
	gravar(&img2, "exemplo2.pgm");
	destruir(&img2);
	imprimir(&img2);

	//#Exemplo3#: ler imagem, getPixel, setPixel 
	PGM img3;
	unsigned char corP;
	cout << "\n\n#Exemplo 3#\n";
	ler(&img3, "feep.pgm");

	//ler o valor do pixel de coordenadas (x=1, y=1)
	corP = getPixel(&img3, 1,1);
	cout << "A cor do pixel(1,1): " << (int) corP << endl;

	setPixel(&img3, 1, 1, 255); //alterar a cor do pixel(1,1) para branco(255)
	gravar(&img3, "exemplo3.pgm"); //gravar o resultado no disco
	destruir(&img3);

	*/


	exercicio3();
	cout << "Pressione uma tecla para encerrar o programa.\n";
	exercicio4();
	getchar();
	return EXIT_SUCCESS; 
}