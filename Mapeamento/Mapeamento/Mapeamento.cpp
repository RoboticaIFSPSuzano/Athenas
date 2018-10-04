#include "pch.h"
#include <iostream>
#include <math.h>
#include <stdlib.h> 
#include <stdio.h>

#define CE 0
#define CD 1
#define BE 2
#define BD 3

const float distMax = 1.5;

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
			this->y = int(y * 1000)
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

	Reta()
	{
		a = 0;
		b = 0;
		sXY = 0;
		sX = 0;
		sY = 0;
		sX2 = 0;
		sY2 = 0;
		pearson = 0;

		n = 0;
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
	float leitura[360];

	for (int i = 0; i < 360; i++)
	{
		cin >> leitura[i];
	}

	Ponto retangular[360];

	for (int i = 0; i < 360; i++)
	{
		retangular[i].set( leitura[i] * cos(i*0.0174533), leitura[i] * sin(i*0.0174533));
	}

	int nLeitura;

	Ponto retangularF[360];

	for (int i = 0; i < 360; i++)
	{
		int indice = i - 5;

		nLeitura = 0;

		if (indice < 0)
		{
			indice += 360;
		}

		retangularF[i].set(0, 0);

		for (int j = 0; j < 11; j++)
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

	int usado[360];

	Reta reta[4];

	for (int i = 0; i < 360; i++)
	{
		usado[i] = 0;

		if (distancia(retangularF[0], retangularF[i]) <= 5)
		{
			reta[0].inserir(retangularF[i]);

			usado[i] = 1;
		}

		if (distancia(retangularF[180], retangularF[i]) <= 5)
		{
			reta[1].inserir(retangularF[i]);

			usado[i] = 1;
		}

		if (distancia(retangularF[90], retangularF[i]) <= 5)
		{
			reta[2].inserir(retangularF[i]);

			usado[i] = 1;
		}

		if (distancia(retangularF[270], retangularF[i]) <= 5)
		{
			reta[3].inserir(retangularF[i]);

			usado[i] = 1;
		}
	}

	

	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i < 360; i++)
		{
			if (distancia(retangularF[i], reta[0]) < distMax && usado[i] == 0)
			{
				reta[0].inserir(retangularF[i]);
				usado[i] = 1;
				reta[0].calcular();
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

	int retaCima;
	int retaBaixo;
	int retaEsq;
	int retaDir;

	for (int i = 0; i<4; i++)
	{
		reta[i].calcularRaiz();
	}

	for (int i = 0; i < 4; i++)
	{
		if (reta[i].b > 0 && abs(reta[i].a < 2))
		{
			retaCima = i;
		}
		else if (reta[i].b < 0 && abs(reta[i].a < 2))
		{
			retaBaixo = i;
		}
		else if (reta[i].raiz > 0 && abs(reta[i].a)>2)
		{
			retaDir = i;
		}
		else if (reta[i].raiz < 0 && abs(reta[i].a > 2))
		{
			retaEsq = i;
		}
	}

	Ponto cruzamento[4];
	
	cruzamento[CE].setX((reta[retaCima].b-reta[retaEsq].b)/(reta[retaEsq].a-reta[retaCima].a));
	cruzamento[CD].setX((reta[retaCima].b - reta[retaDir].b) / (reta[retaDir].a - reta[retaCima].a));
	cruzamento[BE].setX((reta[retaBaixo].b - reta[retaEsq].b) / (reta[retaEsq].a - reta[retaBaixo].a));
	cruzamento[BD].setX((reta[retaBaixo].b - reta[retaDir].b) / (reta[retaDir].a - reta[retaBaixo].a));

	cruzamento[CE].setY((reta[retaCima].a*cruzamento[CE].getX()) + reta[retaCima].b);
	cruzamento[CD].setY((reta[retaCima].a*cruzamento[CD].getX()) + reta[retaCima].b);
	cruzamento[BE].setY((reta[retaBaixo].a*cruzamento[BE].getX()) + reta[retaBaixo].b);
	cruzamento[BD].setY((reta[retaBaixo].a*cruzamento[BD].getX()) + reta[retaBaixo].b);

	for (int i = 0; i < 360; i++)
	{
		if 
	}

	int cluster[5];

	int nUsado = 0;
	int nCluster = 0;
	int pontoAnterior = -1;

	for (int i = 0; i < 360; i++)
	{
		if (usado[i] == 0)
		{
			nUsado += 1;

			if (pontoAnterior != -1)
			{
				if (distancia(retangularF[i], retangularF[pontoAnterior]) > 5)
				{
					nCluster += 1;
				}
				else
				{
					cluster[nCluster] += 1;
				}
			}
			else
			{
				pontoAnterior = i;
			}
		}
	}

	system("cls");
	return 0;
}