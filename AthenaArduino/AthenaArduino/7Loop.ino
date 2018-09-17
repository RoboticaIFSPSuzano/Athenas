void loop()
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

	pidLinha();

	acelerometro.medirFiltrado();

	if (acelerometro.eixoX > 2.5)
	{
		rampa();
	}

	ultraCima.leitura();

	if (ultraCima.distancia < 6.0 && ultraCima.distancia >1)
	{
		para(10);
		obstaculo();
		obst = 0;
	}
}
