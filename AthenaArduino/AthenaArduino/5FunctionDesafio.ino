void retorno()
{
	pidGiro(180);

	para(100);
	refletancia.read(sensores);
	while (sensores[5] > corte || sensores[0] > corte)
	{
		pidLinha(400);
	}
}

void curvadir()
{
	//Verifica se é uma curva, e executa a resposta prevista

	int curva = 0;

	para(500);
	//Se desloca um pouco para frente para verificar se: é uma curva, é uma curva com fita verde ou é uma encruzilhada.
	motores.setLeftSpeed(150);
	motores.setRightSpeed(150);
	delay(0.7*tempo1);
	para(500);

	//Verfica a configuração da linha
	refletancia.read(sensores);
	if (sensores[1] > corte && sensores[5] > corte)
	{
		motores.setLeftSpeed(150);
		motores.setRightSpeed(150);
		delay(1.5*tempo1);
		para(100);
		//Verfica a configuração da linha
		refletancia.read(sensores);
		if (sensores[0] > corte && sensores[5] > corte && sensores[2] > corte && sensores[3] > corte)
		{
			retorno();
		}
		else
		{
			unsigned long tempo = millis();
			while (millis() - tempo > 75)
			{
				pidLinha();
			}
			/*motores.setLeftSpeed(150);
			motores.setRightSpeed(150);
			delay(75);*/
		}
	}
	else
	{
		motores.setLeftSpeed(150);
		motores.setRightSpeed(150);
		delay(1.5*tempo1);
		para(100);
		//Verfica a configuração da linha
		refletancia.read(sensores);

		if (sensores[4] > corte)
		{
			//Curva com fita verde

			motores.setLeftSpeed(150);
			motores.setRightSpeed(150);
			delay(900);
			pidGiro(-85);
			para(100);
			refletancia.read(sensores);

			unsigned long tempo = millis();

			while (sensores[5] > corte || erroLinha > 2000)
			{
				pidLinha(400);

				if (millis() - tempo > 5000)
				{
					break;
				}
			}
		}
		else
		{
			if ((sensores[2] > corte) || (sensores[3] > corte))
			{
				//Encruzilhada - Segue reto

				/*motores.setLeftSpeed(150);
				motores.setRightSpeed(150);
				delay(75);*/

				unsigned long tempo = millis();
				while ((millis() - tempo) < 75)
				{
					pidLinha();
				}
			}
			else
			{
				//Curva comum

				motores.setLeftSpeed(-150);
				motores.setRightSpeed(-150);
				delay((2.2*tempo1) - 10);
				para(100);

				refletancia.read(sensores);

				unsigned long tempo = millis();
				while (sensores[4] > corte || sensores[1] > corte || (millis() - tempo) < 75)
				{
					pidLinha();
				}
			}
		}
	}

}

void curvaesq()
{
	//Verifica se é uma curva, e executa a resposta prevista

	int curva = 0;

	para(500);
	//Se desloca um pouco para frente para verificar se: é uma curva, é uma curva com fita verde ou é uma encruzilhada.
	motores.setLeftSpeed(150);
	motores.setRightSpeed(150);
	delay(0.7*tempo1);
	para(500);
	//Verfica a configuração da linha
	refletancia.read(sensores);
	if (sensores[1] > corte && sensores[5] > corte)
	{
		motores.setLeftSpeed(150);
		motores.setRightSpeed(150);
		delay(1.5*tempo1);
		para(100);
		//Verfica a configuração da linha
		refletancia.read(sensores);
		if (sensores[0] > corte && sensores[5] > corte && sensores[2] > corte && sensores[3] > corte)
		{
			retorno();
		}
		else
		{
			/*motores.setLeftSpeed(150);
			motores.setRightSpeed(150);
			delay(75);*/
			unsigned long tempo = millis();
			while ((millis() - tempo) < 75)
			{
				pidLinha();
			}
		}

	}
	else
	{
		motores.setLeftSpeed(150);
		motores.setRightSpeed(150);
		delay(1.5*tempo1);
		para(100);
		//Verfica a configuração da linha
		refletancia.read(sensores);

		if (sensores[1] > corte)
		{
			//Curva com fita verde

			motores.setLeftSpeed(150);
			motores.setRightSpeed(150);
			delay(950);
			pidGiro(85);
			para(100);

			refletancia.read(sensores);

			unsigned long tempo = millis();

			while (sensores[0] > corte || erroLinha > 2000)
			{
				pidLinha(400);

				if (millis() - tempo > 5000)
				{
					break;
				}
			}
		}
		else
		{
			if ((sensores[2] > corte) || (sensores[3] > corte))
			{
				//Encruzilhada - Segue reto

				/*motores.setLeftSpeed(150);
				motores.setRightSpeed(150);
				delay(75);*/

				unsigned long tempo = millis();
				while (millis() - tempo > 75)
				{
					pidLinha();
				}
			}
			else
			{
				//Curva comum

				motores.setLeftSpeed(-150);
				motores.setRightSpeed(-150);
				delay((2.2*tempo1) - 10);
				para(100);

				refletancia.read(sensores);

				unsigned long tempo = millis();
				while (sensores[1] > corte || sensores[4] > corte || ((millis() - tempo) < 75))
				{
					pidLinha();
				}
			}
		}
	}

}

void obstaculo()
{
	para(100);

	//Ajustar para sair da linha
	do
	{
		motores.setSpeeds(-200, -200);
		ultraCima.leitura();
	} while (ultraCima.distancia < 6.0);

	do
	{
		motores.setSpeeds(200, 200);
		ultraCima.leitura();
	} while (ultraCima.distancia > 6.0);


	//Sair da linha

	pidGiro(90);


	//Andando ao lado do obstáculo

	motores.setSpeeds(200, 200);
	do
	{
		ultraDir.leitura();
	} while (ultraDir.distancia < 15.0);

	delay(200);
	para(100);

	//90º
	pidGiro(-90);

	//Andando ao lado
	para(500);

	motores.setSpeeds(200, 200);

	do
	{
		do
		{
			ultraDir.leitura();
		} while (ultraDir.distancia > 15.0 || ultraDir.distancia < 5.0);
		ultraDir.leitura();
	} while (ultraDir.distancia > 15.0 || ultraDir.distancia < 5.0);

	delay(200);
	motores.setSpeeds(200, 200);

	do
	{
		do
		{
			ultraDir.leitura();
		} while (ultraDir.distancia<40.0 && ultraDir.distancia > 2.0);
		ultraDir.leitura();
	} while (ultraDir.distancia<40.0 && ultraDir.distancia > 2.0);

	delay(400);


	para(500);

	//90º
	pidGiro(-90);

	motores.setSpeeds(200, 200);
	do
	{
		refletancia.read(sensores);
	} while (sensores[2] < corte || sensores[3] < corte);
	delay(200);

	motores.setSpeeds(-200, 300);
	delay(1200);

	para(100);
	refletancia.read(sensores);
	while (sensores[4] > corte)// || sensores[1]>corte)
	{
		pidLinha();
	}
}

bool vitima()
{
	para(0);

	return false;
}

void pegaVitima()
{

}

void sala3()
{
	while (!vitima())
	{
		ultraFrente.leitura();
		ultraCima.leitura();

		if (abs(ultraFrente.distancia - ultraCima.distancia) > 5.0)
		{
			pegaVitima();
			break;
		}

		pidGiro(90);
	}
	
	for (int i = 0; i; i++)
	{
		do
		{
			ultraCima.leitura();
			ultraTras.leitura();
			if (ultraCima.distancia > ultraTras.distancia)
			{
				motores.setSpeeds(200, 200);
			}
			else
			{
				motores.setSpeeds(-200, -200);
			}
		} while (abs(ultraCima.distancia - ultraTras.distancia) > 1.0);

		pidGiro(90);
	}

	zonaResgate();
}

void rampa()
{
	para(1000);
	garra.girar(180);

	para(1000);
	/*while(1)//acelerometro.eixoX>1)
	  {
		//acelerometro.medirFiltrado();

		pidLinha(200);
	  }*/

	do
	{
		pidLinha(300);
		acelerometro.medirFiltrado();
	} while (acelerometro.eixoX > 1.0);


	motores.setSpeeds(300, 300);
	delay(500);

	para(100);

	refletancia.read(sensores);
	garra.girar(10);

	sala3();
}

int mapeamento()
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
		reta[i].calcularRaiz();
	}

	int cima;
	int baixo;
	int esq;
	int dir;

	for (int i = 0; i < 4; i++)
	{
		if (reta[i].b > 0 && abs(reta[i].a) < 2)
		{
			cima = i;
		}
		else if (reta[i].b < 0 && abs(reta[i].a) < 2)
		{
			baixo = i;
		}
		else if (reta[i].raiz > 0 && abs(reta[i].a) > 2)
		{
			esq = i;
		}
		else if (reta[i].raiz < 0 && abs(reta[i].a) > 2)
		{
			dir = i;
		}
	}

	Ponto cruzamento[4];

	cruzamento[CE] = reta[cima].cruzamento(&reta[esq]);
	cruzamento[CD] = reta[cima].cruzamento(&reta[dir]);
	cruzamento[BE] = reta[baixo].cruzamento(&reta[esq]);
	cruzamento[BD] = reta[baixo].cruzamento(&reta[dir]);

	Reta r;

	r.a = 0;


	for (int i = 0; i < 360; i++)
	{
		if (usado[i] == 0)
		{

			r.b = retangularF[i].getY();

			Ponto encontro[4];

			for (int j = 0; j < 4; j++)
			{
				encontro[j] = reta[j].cruzamento(&r);
			}

			int vezes = 0;

			//Verifica quantas vezes a reta cruza o quadrilátero

			//Verifica se cruza com a reta de cima
			if (encontro[cima].getX() > cruzamento[CE].getX() && encontro[cima].getX() < cruzamento[CD].getX())
			{
				vezes += 1;
			}

			//Verifica se cruza com a reta esquerda
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

			//Verifica se cruza com a reta de baixo
			if (encontro[baixo].getX() > cruzamento[BE].getX() && encontro[baixo].getX() < cruzamento[BD].getX())
			{
				vezes += 1;
			}


			//Verifica se cruza com a reta direita
			if (cruzamento[CD].getX() > cruzamento[BE].getX())
			{
				if (encontro[dir].getX() > cruzamento[BD].getX() && encontro[esq].getX() < cruzamento[CD].getX())
				{
					vezes += 1;
				}
			}
			else
			{
				if (encontro[esq].getX() > cruzamento[CD].getX() && encontro[esq].getX() < cruzamento[BD].getX())
				{
					vezes += 1;
				}
			}

			if (vezes != 1)
			{
				usado[i] = 1;
			}
		}
	}

	free(retangularF);

	int cluster[4];
	int nPonto[4];
	int anterior = -1;

	int indice = 0;

	for (int i = 0; i < 4; i++)
	{
		nPonto[i] = 0;
		cluster[i] = 0;
	}

	for (int i = 0; i < 360; i++)
	{
		if (usado[i] == 0)
		{
			if (i - anterior < 2 || anterior == -1)
			{
				cluster[indice] += i;

				nPonto[indice] += 1;

				anterior = i;
			}
			else
			{
				anterior = i;
				indice += 1;
			}
		}
	}

	int maior = 0;

	for (int i = 0; i < 4; i++)
	{
		if (nPonto[i] != 0)
		{
			cluster[i] /= nPonto[i];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (nPonto[i] > nPonto[maior])
		{
			maior = i;
		}
	}

	return cluster[maior];
}

void soltaVitima()
{
	para(0);
}

void zonaResgate()
{
	int grau = mapeamento();

	pidGiroSemReset(grau);

	motores.setSpeeds(200, 200);
	
	do {
		ultraFrente.leitura();
	} while (ultraFrente.distancia > 5.0);

	para(100);

	soltaVitima();
}