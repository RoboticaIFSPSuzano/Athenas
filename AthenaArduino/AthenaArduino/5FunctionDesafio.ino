void retorno() //Executa a encruzilhada em que o robô deve retornar
{
	pidGiro(180); //Gira o robô 180º

	//Ajusta o robô a linha
	para(100);
	refletancia.read(sensores);
	while (sensores[5] > corte || sensores[0] > corte)
	{
		pidLinha(400);
	}
}

void curvadir()//Verifica qual o tipo de curva ou redutor e executa a resposta
{
	para(500);
	//Se desloca um pouco para frente para verificar se: é uma curva, é uma curva com fita verde ou é uma encruzilhada.
	motores.setLeftSpeed(150);
	motores.setRightSpeed(150);
	delay(0.7*tempo1);
	para(500);

	for (int i = 0; i < 25; i++) //Faz as leituras para verificar se é um redutor
	{
		acelerometro.medirFiltrado();
	}

	if (acelerometro.eixoX > 1) //Verifica se é um redutor e supera-o
	{
		motores.setSpeeds(400, 400);

		delay(250);

		para(100);
		return;
	}

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

void curvaesq() //Verifica qual o tipo de curva ou redutor e executa a resposta
{

	para(500);
	//Se desloca um pouco para frente para verificar se: é uma curva, é uma curva com fita verde ou é uma encruzilhada.
	motores.setLeftSpeed(150);
	motores.setRightSpeed(150);
	delay(0.7*tempo1);
	para(500);

	for (int i = 0; i < 25; i++) //Faz as leituras para verificar se é um redutor
	{
		acelerometro.medirFiltrado();
	}

	if (acelerometro.eixoX > 1) //Verifica se é um redutor e supera-o
	{
		motores.setSpeeds(400, 400);

		delay(250);

		para(100);
		return;
	}

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
		ultraFrente.leitura();
	} while (ultraFrente.distancia < 6.0);

	do
	{
		motores.setSpeeds(200, 200);
		ultraFrente.leitura();
	} while (ultraFrente.distancia > 6.0);


	//Sair da linha

	pidGiro(90);


	//Andando ao lado do obstáculo

	motores.setSpeeds(200, 200);
	do
	{
		laserDir.leitura();
	} while (laserDir.distancia < 15.0);

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
			laserDir.leitura();
		} while (laserDir.distancia > 15.0 || laserDir.distancia < 5.0);
		laserDir.leitura();
	} while (laserDir.distancia > 15.0 || laserDir.distancia < 5.0);

	delay(200);
	motores.setSpeeds(200, 200);

	do
	{
		do
		{
			laserDir.leitura();
		} while (laserDir.distancia<40.0 && laserDir.distancia > 2.0);
		laserDir.leitura();
	} while (laserDir.distancia<40.0 && laserDir.distancia > 2.0);

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

void bolinha() //Captura uma bolinha?
{
	garra.girar(180);
	ponta.girar(90);
	delay(200);
	motores.setSpeeds(200, 200);
	delay(5000);
	para(1);
	ponta.girar(180);
	delay(200);
	garra.girar(0);
}

void sala3() //Efetua a programação da sala 3
{

}

void rampa() //Passa pela rampa
{
	//Desce a garra
	para(1000);
	garra.girar(180);

	para(1000);
	/*while(1)//acelerometro.eixoX>1)
	  {
		//acelerometro.medirFiltrado();

		pidLinha(200);
	  }*/

	//Anda pela rampa
	do
	{
		pidLinha(300);
		acelerometro.medirFiltrado();
	} while (acelerometro.eixoX > 1.0); //Verfica se ela terminou


	//Anda mais um pouco
	motores.setSpeeds(300, 300);
	delay(500);

	para(100);

	refletancia.read(sensores);
	
	//Sobe a garra
	garra.girar(10);

	sala3(); //Efetua a programação para a sala 3
}

void obr()
{
	//Leitura do sensor de refletancia 
	refletancia.read(sensores);

	//Verifica e executa curvas
	if (sensores[0] > corte || sensores[5] > corte)
	{
		para(10);

		if (sensores[0] > corte && sensores[5] < corte)
		{
			//Curva para a esquerda
			curvaesq();
		}
		else if (sensores[5] > corte && sensores[0] < corte)
		{
			//Curva para a direita
			curvadir();
		}
		else
		{
			//Encruzilhada/Redutor
			motores.setLeftSpeed(150);
			motores.setRightSpeed(150);
			delay(75);
		}
	}

	pidLinha(); //Segue linha

	acelerometro.medirFiltrado(); //Efetua a leitura do acelerômetro

	if (acelerometro.eixoX > 2.5) //Verifica se o robô está na rampa
	{
		rampa();
	}

	ultraFrente.leitura(); //Efetua a leitura do sensor ultrassônico

	if (ultraFrente.distancia < 6.0 && ultraFrente.distancia >1) //Verifica se há um obstáculo
	{
		para(10);
		obstaculo();
	}
}