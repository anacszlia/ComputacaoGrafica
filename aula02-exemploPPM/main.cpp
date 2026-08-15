#include <iostream>
#include <fstream>
#include <sstream>
#include "PPM.hpp"
#include <vector>
using namespace std;

void exercicio3(){
	PPM img;
	RGB cor (128,0,255);
	criar(&img,500,500,cor);
	gravar(&img, "exercicio3.ppm");
	destruir(&img);
}
void exercicio4(){
	PPM img;
	RGB cor (128,0,255);
	RGB preto (0,0,0);
	criar(&img,500,500,preto);
	setLinha(&img,250,cor);
	gravar(&img, "exercicio4.ppm");
	destruir(&img);
}
void exercicio5(){
	PPM img;
	RGB cor (128,0,255);
	RGB preto (0,0,0);
	criar(&img,500,500,preto);
	cout<<"digite uma cordenada valida:";
	int x,y;
	cin>>x>>y;
	cout<<(coordValida(&img,x,y)? "valida":"invalida");
	cout<<"\n";
	destruir(&img);
}
void exercicio6(){
	PPM img;
	RGB cor (128,0,255);
	RGB preto (0,0,0);
	criar(&img,500,500,preto);
	gravar(&img, "exercicio6.ppm");
	destruir(&img);
}
void exercicio7(){
	PPM img;
	RGB cor (128,0,255);
	RGB preto (0,0,0);
	criar(&img,500,500,preto);
	setLinhasAleatorias(&img,30);
	gravar(&img, "exercicio7.ppm");
	destruir(&img);
}
//202,315 385,543
void exercicio8(){
	PPM img,res;
	RGB cor (128,0,255);
	RGB preto (0,0,0);
	ler(&img,"numeros.ppm");
	criar(&res,500,500,preto);
	copiaArea(&img,&res,202,315,385,543);
	gravar(&res, "exercicio8.ppm");
	destruir(&img);
}

void exercicio9(){
	PPM img,res;
	RGB preto (0,0,0);
	ler(&img,"spider.ppm");
	criar(&res,447,447,preto);
	descolorirImagem(&img,&res);
	gravar(&res, "exercicio9.ppm");
	destruir(&img);
}
void exercicio10(){
	PPM img,res;
	RGB preto (0,0,0);
	ler(&img,"spider.ppm");
	criar(&res,447,447,preto);
	inverterHorizontal(&img,&res);
	gravar(&res, "exercicio10.ppm");
	destruir(&img);
}
void exercicio11(){
	
	int borda;
	cout<<"digite a largura da borda:";
	cin>>borda;
	PPM imgE,res;
	ler(&imgE,"numeros.ppm");
	int x1,y1,x2,y2;
	x1=202;
	y1=315;
	x2=385;
	y2=543;
	copiaArea(&imgE,&res,x1,y1,x2,y2);
	RGB verde(0,255,0);
	setBorda(&res, borda, verde,x1,y1,x2,y2);
	gravar(&res, "exercicio11.ppm");
	destruir(&imgE);


}

void exercicio12(){
	// PPM img1,img2,img3;
	// RGB ciano(0,250,250),rosa(255, 105, 180),vermelho(255,0,0);
	// criar(&img1,500,500,ciano);
	// criar(&img2,500,500,rosa);
	// criar(&img3,500,500,vermelho);
	// gravar(&img1, "img1.ppm");
	// gravar(&img2, "img2.ppm");
	// gravar(&img3, "img3.ppm");

	fstream fs;
	string linha;
    fs.open ("entrada.txt", fstream::in);
	if(!fs.is_open())
	{
		cout << "Erro ao abrir arquivo!\n";
		return;
	}

	linha = "";
	vector<PPM> imagens;
	while(getline(fs, linha))
	{	
		cout << "Linha lida: [" << linha << "]" << endl;

		PPM img;
		if(linha.size()>4 && ler(&img, linha))
		{
			cout << "Imagem carregada: " << linha << endl;
			imagens.push_back(img);
		}
		else
		{
			cout << "Erro ao carregar imagem: " << linha << endl;
		}

		linha= "";
	}
  

	fs.close();

	cout << "Total de imagens: " << imagens.size() << endl;
	int larg = imagens[0].larg, alt= imagens[0].alt;
	for(int i=1; i<imagens.size(); i++)
	{
		if(larg!= imagens[i].larg || alt!= imagens[i].alt)
		{
			cout << "Erro: as imagens de entrada devem possuir o mesmo tamanho!\n";
			return;
		}
	}

	PPM imgS;
	int offset=0;
	criar(&imgS, larg*imagens.size(), alt);
	for(int i=0; i<imagens.size(); i++)
	{
		for(int y=0; y<alt; y++)
		{
			for(int x=0; x<larg; x++)
			{
				setPixel(&imgS, x+offset, y, getPixel(&imagens[i], x, y));
			}
		}
		offset += larg;

	}
	gravar(&imgS, "exercicio12.ppm");


}

void exercicio13(){
	PPM img, imgS;
	//RGB amarelo(255,255,0);
	//criar(&img, 500, 500, amarelo);
	ler(&img, "ifsul.ppm");
	inverterCores(&img,&imgS);
	gravar(&imgS, "exercicio13.ppm");
}

void exercicio14(){
	PPM img, imgS;
	ler(&img, "spider.ppm");
	reduzirQuantizacao(&img,&imgS);
	gravar(&imgS, "exercicio14.ppm");
}

void exercicio15(){
	PPM img;
	ler(&img, "numeros.ppm");
	dda(&img);

	gravar(&img, "exercicio15.ppm");
}

int main(void)
{

	//#Exemplo1#: criando uma imagem e gravando no disco
	// PPM img1;
	// criar(&img1, 5, 7);
	// gravar(&img1, "exemplo1.ppm");
	// destruir(&img1);

	// //#Exemplo2#: imprimindo os atributos da imagem 
	// PPM img2;
	// cout << "#Exemplo 2#\n";
	// imprimir(&img2);
	// criar(&img2, 700, 500);
	// imprimir(&img2);
	// gravar(&img2, "exemplo2.ppm");
	// destruir(&img2);
	// imprimir(&img2);

	//#Exemplo3#: ler imagem, getPixel, setPixel 
	// PPM img3;
	// RGB corP;
	// cout << "\n\n#Exemplo 3#\n";
	// ler(&img3, "exemploPPM.ppm");

	//ler o valor do pixel de coordenadas (x=1, y=1)
	// corP = getPixel(&img3, 0,0);
	// cout << "A cor do pixel(0,0): [" << corP << "] " << endl;

	// setPixel(&img3, 1, 0, RGB(0, 128, 128)); //alterar a cor do pixel(1, 0) para RGB(0, 128, 128)
	// setPixel(&img3, 2, 0, RGB(255, 0, 255)); //alterar a cor do pixel(2, 0) para rosa
	// setPixel(&img3, 2, 4, RGB(128, 0, 128)); //alterar a cor do pixel(2, 4) para roxo
	// gravar(&img3, "exemplo3.ppm"); //gravar o resultado no disco
	// destruir(&img3);


	//exercicio3();
	//exercicio4();
	//exercicio7();
	//exercicio8();
	//exercicio9();
	//exercicio10();
	//exercicio11();
	//exercicio12();
	//exercicio13();
	//exercicio14();
	exercicio15();
	cout << "Pressione uma tecla para encerrar o programa.\n";
	getchar();
	return EXIT_SUCCESS; 
}