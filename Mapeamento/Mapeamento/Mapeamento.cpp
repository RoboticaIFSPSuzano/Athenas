#include <iostream>
#include <math.h>
#include <stdlib.h> 
#include <stdio.h>

const float distMax = 1.5;

using namespace std;

struct Ponto
{
	float x,
		y;
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
		pearson;

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

		sXY += a.x * a.y;
		sX += a.x;
		sY += a.y;
		sX2 += pow(a.x, 2);
		sY2 += pow(a.y, 2);
	}

	void calcular()
	{
		a = ((n*sXY) - (sX*sY)) / ((n*sX2) - (pow(sX, 2)));
		b = (sY - (a*sX)) / n;
		pearson = ((n*sXY) - (sX*sY)) / sqrt((n*sX2 - pow(sX, 2))*(n*sY2 - pow(sY, 2)));
	}

	float custo(Ponto a)
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
	}

};

float distancia(Ponto a, Ponto b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float distancia(Ponto ponto, Reta reta)
{
	return abs((reta.a*ponto.x) - ponto.y + reta.b) / sqrt(pow(reta.a, 2) + 1);
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
		retangular[i].x = leitura[i] * cos(i*0.0174533);
		retangular[i].y = leitura[i] * sin(i*0.0174533);
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

		retangularF[i].x = 0;
		retangularF[i].y = 0;

		for (int j = 0; j < 11; j++)
		{
			if (retangular[indice].x != 0 && retangular[indice].y != 0)
			{
				retangularF[i].x += retangular[indice].x;
				retangularF[i].y += retangular[indice].y;

				nLeitura += 1;
			}


			indice += 1;

			if (indice > 359)
			{
				indice = 0;
			}
		}

		retangularF[i].x /= nLeitura;
		retangularF[i].y /= nLeitura;
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

	system("cls");

	for (int i = 0; i < 4; i++)
	{
		reta[i].calcular();

		cout << reta[i].a << ";" << reta[i].b << ";" << reta[i].n << ";" << reta[i].pearson << "\n";
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




	for (int i = 0; i < 4; i++)
	{
		reta[i].calcular();

		cout << reta[i].a << ";" << reta[i].b << ";" << reta[i].n << ";" << reta[i].pearson << "\n";
	}

	cout << "\n\n";

	for (int i = 0; i < 360; i++)
	{
		if (usado[i] == 0)
		{
			cout << retangularF[i].x << ";" << retangularF[i].y << "\n";
		}
	}

	return 0;
}
