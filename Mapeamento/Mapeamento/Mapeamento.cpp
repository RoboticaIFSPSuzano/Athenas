#include "pch.h"
#include <iostream>
#include <math.h>
#include <stdlib.h> 
#include <stdio.h>
#include "Planilha.h"

#define CE 0
#define CD 1
#define BE 2
#define BD 3

float distMax = 1.5;
float distMaxPonto = 10		;
int nLeituraFiltro = 10;

using namespace std;

class Ponto
{
	private:
		int x,
			  y;
	
	public:

		void set(float x, float y)
		{
			this->x = int(x * 1000);
			this->y = int(y * 1000);
		}

		void setX(float x)
		{
			this->x = int(x * 1000);
		}

		void setY(float y)
		{
			this->y = int(y * 1000);
		}

		float getX()
		{
			return float(x) / 1000;
		}

		float getY()
		{
			return float(y) / 1000;
		}
};

class Reta
{
public:
	float a,
		b,
		sXY,
		sX,
		sY,
		sX2,
		sY2,
		pearson,
		raiz;

	int n;

	void reset()
	{
		a = 0;
		b = 0;
		sXY = 0;
		sX = 0;
		sY = 0;
		sX2 = 0;
		sY2 = 0;
		pearson = 0;

		n = int(0);
	}

	void inserir(Ponto a)
	{
		n += 1;

		sXY += a.getX()* a.getY();
		sX += a.getX();
		sY += a.getY();
		sX2 += pow(a.getX(), 2);
		sY2 += pow(a.getY(), 2);
	}

	void calcular()
	{
		a = ((n*sXY) - (sX*sY)) / ((n*sX2) - (pow(sX, 2)));
		b = (sY - (a*sX)) / n;
		pearson = ((n*sXY) - (sX*sY)) / sqrt((n*sX2 - pow(sX, 2))*(n*sY2 - pow(sY, 2)));
	}

	void calcularRaiz()
	{
		raiz = -b / a;
	}

	Ponto cruzamento(Reta *r)
	{
		Ponto c;

		c.setX((this->b - r->b )/ (r->a - this->a));
		c.setY((a*c.getX()) + b);

		return c;
	}

	/*float custo(Ponto a)
	{
		float tXY = sXY + (a.x*a.y),
			tX = sX + a.x,
			tY = sY + a.y,
			tX2 = sX2 + pow(a.x, 2),
			tY2 = sY2 + pow(a.y, 2);

		int t = n + 1;

		pearson = ((n*sXY) - (sX*sY)) / sqrt((n*sX2 - pow(sX, 2))*(n*sY2 - pow(sY, 2)));

		float tPearson = ((t*tXY) - (tX*tY)) / sqrt((t*tX2 - pow(tX, 2))*(t*tY2 - pow(tY, 2)));

		return tPearson - pearson;
	}*/

};

float distancia(Ponto a, Ponto b)
{
	return sqrt(pow(a.getX()- b.getX(), 2) + pow(a.getY() - b.getY(), 2));
}

float distancia(Ponto ponto, Reta reta)
{
	return abs((reta.a*ponto.getX()) - ponto.getY() + reta.b) / sqrt(pow(reta.a, 2) + 1);
}

int main()
{
	char nome[10];
	nome[0] = 'Z';
	nome[1] =	'.';
	nome[2] = 'c';
	nome[3] = 's';
	nome[4] = 'v';
	Planilha planilha(nome);
	
	float leitura[360]; //Armazena as leituras do laser

	//Recebe as leituras
	for (int i = 0; i < 360; i++)
	{
		cin >> leitura[i];
	}

	Ponto retangular[360]; //Armazena os pontos cartesianos do mapa

	//Calcula os pontos (polar -> retangular)
	for (int i = 0; i < 360; i++)
	{
		retangular[i].set( leitura[i] * cos(i*0.0174533), leitura[i] * sin(i*0.0174533));
	}

	int nLeitura;

	Ponto retangularF[360]; //Armazena os pontos filtrados da sala

	//Filtra o mapa utilizando média móvel
	for (int i = 0; i < 360; i++)
	{
		int indice = i - (nLeituraFiltro / 2);

		nLeitura = 0;

		if (indice < 0)
		{
			indice += 360;
		}

		retangularF[i].set(0, 0);

		for (int j = 0; j < nLeituraFiltro+1; j++)
		{
			if (retangular[indice].getX() != 0 && retangular[indice].getY() != 0)
			{
				retangularF[i].set(retangular[indice].getX() + retangularF[i].getX(), retangular[indice].getY() + retangularF[i].getY());

				nLeitura += 1;
			}


			indice += 1;

			if (indice > 359)
			{
				indice = 0;
			}
		}

		retangularF[i].set(retangularF[i].getX() / nLeitura, retangularF[i].getY() / nLeitura);
	}

	int usado[360]; //Indica se o ponto já foi utilizado para alguma reta

	Reta reta[4]; //Armazena as 4 retas

	//Inicia as retas
	reta[0].reset();
	reta[1].reset();
	reta[2].reset();
	reta[3].reset();

	//Verifica quais pontos pertencem as retas
	for (int i = 0; i < 360; i++)
	{
		usado[i] = 0;

		if (distancia(retangularF[0], retangularF[i]) <= distMaxPonto) //Verifica se o ponto está próximo do ponto 0º
		{
			reta[0].inserir(retangularF[i]);

			usado[i] = 1;
		}

		if (distancia(retangularF[180], retangularF[i]) <= distMaxPonto) //Verifica se o ponto está próximo do ponto 180º
		{
			reta[1].inserir(retangularF[i]);

			usado[i] = 1;
		}

		if (distancia(retangularF[90], retangularF[i]) <= distMaxPonto) //Verifica se o ponto está próximo do ponto 90º
		{
			reta[2].inserir(retangularF[i]);

			usado[i] = 1;
		}

		if (distancia(retangularF[270], retangularF[i]) <= distMaxPonto) //Verifica se o ponto está próximo do ponto 270º
		{
			reta[3].inserir(retangularF[i]);

			usado[i] = 1;
		}
	}

	//Calcula os coeficientes das retas
	for (int i = 0.; i < 4; i++)
	{
		reta[i].calcular();
	}

	//Reajusta as retas adicionando os pontos próximos a elas
	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 360; i++)
		{
			if (distancia(retangularF[i], reta[0]) < distMax && usado[i] == 0) //Verifica se o ponto está próximo da reta
			{
				reta[0].inserir(retangularF[i]); //Insere o ponto na reta
				usado[i] = 1; //Indica que o ponto foi utilizado
				reta[0].calcular(); //Reajusta a reta
			}
			if (distancia(retangularF[i], reta[1]) < distMax && usado[i] == 0)
			{
				reta[1].inserir(retangularF[i]);
				usado[i] = 1;
				reta[1].calcular();
			}
			if (distancia(retangularF[i], reta[2]) < distMax && usado[i] == 0)
			{
				reta[2].inserir(retangularF[i]);
				usado[i] = 1;
				reta[2].calcular();
			}
			if (distancia(retangularF[i], reta[3]) < distMax && usado[i] == 0)
			{
				reta[3].inserir(retangularF[i]);
				usado[i] = 1;
				reta[3].calcular();
			}

		}
	}

	int cima, baixo, esq, dir; //Armazenam quais retas estão em quais lados do quadrilátero

	dir = 0;
	esq = 1;
	cima = 2;
	baixo = 3;

	//Recalcula as retas e calcula as raizes
	for (int i = 0; i<4; i++)
	{
		reta[i].calcular();
		reta[i].calcularRaiz();
	}
	

	Ponto cruzamento[4]; //Armazenam os cruzamentos entre as retas
	
	//Calcula os cruzamentos entre as retas
	cruzamento[CE] = reta[cima].cruzamento(&reta[esq]);
	cruzamento[CD] = reta[cima].cruzamento(&reta[dir]);
	cruzamento[BE] = reta[baixo].cruzamento(&reta[esq]);
	cruzamento[BD] = reta[baixo].cruzamento(&reta[dir]);

	Reta r; //segmento de reta auxiliar

	//Inicia o segmento de reta auxiliar
	r.a = 0;


	//Remove os pontos fora do quadrilátero
	for (int i = 0; i < 360; i++)
	{
		if (usado[i] == 0)
		{

			//Insere no segmento de reta o coeficiente do ponto analisado
			r.b = retangularF[i].getY();

			//Segmento de reta que parte do ponto e segue ao infinito para a esquerda OU direita

			//Pontos de encontro entre as retas e o segmento
			Ponto encontro[4];

			//Calcula os cruzamentos entre as retas e o segmento
			for (int j = 0; j < 4; j++)
			{
				encontro[j] = reta[j].cruzamento(&r);
			}

			int vezes = 0; //Armazena quantas vezes o segmento cruza o quadrilátero
			
			//Verifica quantas vezes o segmento de reta cruza o quadrilátero

			//Verifica se cruza com a reta de cima
			if (encontro[cima].getX() > cruzamento[CE].getX() && encontro[cima].getX() < cruzamento[CD].getX())
			{
				vezes += 1;
			}	

			//Verifica se cruza com a reta esquerda
			if (encontro[esq].getX() >	 retangularF[i].getX())
			{
				if (cruzamento[CE].getX() > cruzamento[BE].getX())
				{
					if (encontro[esq].getX() > cruzamento[BE].getX() && encontro[esq].getX() < cruzamento[CE].getX())
					{
						vezes += 1;
					}
				}
				else
				{
					if (encontro[esq].getX() > cruzamento[CE].getX() && encontro[esq].getX() < cruzamento[BE].getX())
					{
						vezes += 1;
					}
				}
			}
			
			//Verifica se cruza com a reta de baixo
			if (encontro[baixo].getX() > cruzamento[BE].getX() && encontro[baixo].getX() < cruzamento[BD].getX())
			{
				vezes += 1;
			}
			
			//Verifica se cruza com a reta direita
			if (encontro[dir].getX() >= retangularF[i].getX())
			{
				if (cruzamento[CD].getX() > cruzamento[BD].getX())
				{
					if (encontro[dir].getX() < cruzamento[BD].getX() && encontro[dir].getX() > cruzamento[CD].getX())
					{
						vezes += 1;
					}
				}
				else
				{
					if (encontro[dir].getX() > cruzamento[CD].getX() && encontro[dir].getX() < cruzamento[BD].getX())
					{
						vezes += 1;
					}
				}
			}
			//Se não cruzar uma vez e apenas uma = fora do quadrilátero
			if (vezes != 1)
			{
				usado[i] = 1;
			}
		}
	}

	int cluster[5]; //Angulo médio dos conjuntos de pontos
	int nPonto[5]; //N de pontos nos conjuntos
	int anterior	 = -1; //Indice do ponto anterior

	int indice = 0; //Indice do conjunto de pontos atual

	//Reseta os conjuntos
	for (int i = 0; i < 5; i++)
	{
		nPonto[i] = 0;
		cluster[i] = 0;
	}

	//Forma os conjuntos
	for (int i = 0; i < 360; i++)
	{
		if (usado[i] == 0) //Verifica se o ponto deve pertencer a algum conjunto
		{
			if (i - anterior < 2 || anterior == -1) //Verifica se o ponto pertence ao conjunto anterior
			{
				cluster[indice] += i;

				nPonto[indice] += 1;

				anterior = i;
			}
			else //Incia um novo conjunto
			{
				anterior = i;
				indice += 1;
			}
		}
	}

	int maior = 0; //Armazena o indice do conjunto com mais pontos
	
	//Calcula os angulos médios
	for (int i = 0; i < 5; i++)
	{
		if (nPonto[i] != 0)
		{
			cluster[i] /= nPonto[i];

			//cout << cluster[i] << " " << nPonto[i];
		}
	}

	//Verifica qual o maior conjunto
	for (int i = 0; i < 5; i++)
	{
		if (nPonto[i] > nPonto[maior])
		{
			maior = i;
		}
	}

	for (int i = 0; i < 360; i++)
	{
		planilha.escrever(retangular[i].getX());
		planilha.escrever(retangular[i].getY());
		planilha.escrever(retangularF[i].getX());
		planilha.escrever(retangularF[i].getY());

		if (i <= 3)
		{
			planilha.escrever(reta[i].a);
			planilha.escrever(reta[i].b);
		}
		else
		{
			planilha.novaColuna();
			planilha.novaColuna();
		}

		if (usado[i] == 0)
		{
			planilha.escrever(retangularF[i].getX());
			planilha.escrever(retangularF[i].getY());
		}
		else
		{
			planilha.novaColuna();
			planilha.novaColuna();
		}

		if (i == 0)
		{
			planilha.escrever(cluster[maior]);
		}

		planilha.novaLinha();
	}

	cout << cluster[maior]; //Imprime o angulo do maior conjuto = conjunto da zona de resgate
	return 0;
}