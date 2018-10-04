void loop()
{
	float leitura[360];

	for (int i = 0; i < 360; i++)
	{
		leitura[i] = 1;
	}

	Ponto retangular[360];

	for (int i = 0; i < 360; i++)
	{
		retangular[i].set(leitura[i] * cos(i*0.0174533), leitura[i] * sin(i*0.0174533));
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

	free(retangular);
	free(nLeitura);

	bool usado[360];

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

	for (int i = 0; i < 4; i++)
	{
		reta[i].calcular();
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
	}

	for (int i = 0; i < 360; i++)
	{
		if (usado[i] == 0)
		{
		}
	}

}