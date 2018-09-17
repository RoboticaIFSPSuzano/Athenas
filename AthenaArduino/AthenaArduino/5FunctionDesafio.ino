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

void bolinha()
{
	para(0);
}

void sala3()
{

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
