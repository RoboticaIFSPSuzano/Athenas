void loop()
{
	para(0);
	para(100);

	giroscopio.reset();
	giroscopio.calibrar();

	int nLeitura[360];
	float leituraRadar[360];

	for (int i = 0; i < 360; i++)
	{
		nLeitura[i] = 0;
		leituraRadar[i] = 0;
	}

	unsigned long tempo = millis();

	motores.setSpeeds(-400, 400);

	while (millis() - tempo < 30000)
	{
		laserDir.leitura();
		laserEsq.leitura();
		giroscopio.medirGrau();

		if (giroscopio.grau.z >= 360)
		{
			giroscopio.grau.z -= 360;
		}

		int indice = (int)giroscopio.grau.z;

		leituraRadar[indice] += laserDir.distancia;
		nLeitura[indice] += 1;

		indice += 180;

		if (indice >= 360)
		{
			indice -= 360;
		}

		leituraRadar[indice] += laserEsq.distancia;
		nLeitura[indice] += 1;


	}
	para(1);

	for (int i = 0; i < 360; i++)
	{
		if (nLeitura[i] > 0)
		{
			leituraRadar[i] = leituraRadar[i] / nLeitura[i];
		}
	}

	float variacao[360];
	float variacaoFiltrado[360];

	for (int i = 0; i < 360; i++)
	{
		if (i == 0)
		{
			variacao[i] = leituraRadar[0] - leituraRadar[359];
		}
		else
		{
			variacao[i] = leituraRadar[i] - leituraRadar[i - 1];
		}
	}

	for (int i = 0; i < 360; i++)
	{
		int indice = i - 5;

		if (indice < 0)
		{
			indice += 360;
		}

		for (int j = 0; j < 11; j++)
		{
			variacaoFiltrado[i] += variacao[indice];
			indice += 1;
			if (indice > 359)
			{
				indice -= 359;
			}
		}

		variacaoFiltrado[i] /= 11;
	}

	int pico[4];
	int grau[4];
	int indice = 0;

	for (int i = 0; i < 360; i++)
	{
		if (i == 0)
		{
			if (variacao[i] < 0 && variacao[359] >= 0)
			{
				pico[indice] = leituraRadar[i];
				grau[indice] = i;
				indice += 1;
			}
		}
		else if (variacao[i] < 0 && variacao[i - 1] >= 0 && indice == 0)
		{
			pico[indice] = leituraRadar[i];
			grau[indice] = i;
			indice += 1;
		}
		else if (variacao[i] < 0 && variacao[i - 1] >= 0 && i - grau[indice-1] > 45)
		{
			pico[indice] = leituraRadar[i];
			grau[indice] = i;
			indice += 1;
		}
	}

	if (pico[0] < pico[1] && pico[0] < pico[2] && pico[0] < pico[3])
	{
		indice = grau[0];
	}
	else if (pico[1] < pico[2] && pico[1] < pico[3])
	{
		indice = grau[1];
	}
	else if (pico[2] < pico[3])
	{
		indice = grau[2];
	}
	else
	{
		indice = grau[3];
	}

	pidGiroSemReset(indice);

	para(0);
}