void pidGiro(int grau)
{
	float kp = 10.13,
		ki = 0.0,
		kd = 39.46,
		erro = 0,
		erroAnt = 0,
		integral = 0;

	int diferenca,
		velocidadeEsq,
		velocidadeDir,
		velocidade,
		resetIntegral = 0;

	giroscopio.reset();

	para(500);

	giroscopio.calibrar(eZ);

	unsigned long tempo = millis();

	do
	{
		giroscopio.medirGrau();

		erroAnt = erro;

		erro = grau - giroscopio.grau.z;

		if (resetIntegral == 200);
		{
			resetIntegral = 0;
			integral = 0;
		}

		integral += erro;

		resetIntegral += 1;

		velocidade = int((kp*erro) + (ki*integral) + (kd*(erro - erroAnt)));

		velocidadeEsq = -velocidade;
		velocidadeDir = +velocidade;

		motores.setSpeeds(velocidadeEsq, velocidadeDir);

	} while (abs(velocidade) > 50 && ((millis() - tempo) < 6000));

}

void pidDeslocamento(float distancia)
{
	float kp = 5,
		ki = 15,
		kd = 0,
		erro = 0,
		erroAnt = 0,
		integral = 0;

	int diferenca,
		velocidadeEsq,
		velocidadeDir,
		velocidade,
		resetIntegral = 0;

	acelerometro.reset();

	para(500);

	acelerometro.calibrar(eX);

	do
	{
		acelerometro.medirDeslocamento();

		erroAnt = erro;

		erro = distancia - acelerometro.deslocamento.x;

		if (resetIntegral == 200);
		{
			resetIntegral = 0;
			integral = 0;
		}

		integral += erro;

		resetIntegral += 1;

		velocidade = int((kp*erro) + (ki*integral) + (kd*(erro - erroAnt)));

		velocidadeEsq = velocidade;
		velocidadeDir = velocidade;

		motores.setSpeeds(velocidadeEsq, velocidadeDir);

	} while (abs(velocidade) > 50);

	para(0);
}

void para(int tempo)
{
	if (tempo == 0)
	{
		motores.setSpeeds(0, 0);
		botao.waitForButton();
	}
	else
	{
		motores.setSpeeds(0, 0);
		delay(tempo);
	}
}

void pidLinha()
{
	//Calcula a posição do robô e executa a resposta de acordo com o algoritimo PID  
	float convertido[6], //Recebe os valores dos sensores escalados/ponderados
		posicao = 0, //Recebe a posição da linha
		diferenca, //Diferença entre as velocidades dos motores
		kp = 0.2,
		ki = 0.0,
		kd = 0.1;

	//Leitura dos sensores
	refletancia.read(sensores);

	//Calcula os dados dos sensores ponderados
	convertido[0] = float(sensores[0]) * -3;
	convertido[1] = float(sensores[1]) * -2;
	convertido[2] = float(sensores[2]) * -1;
	convertido[3] = float(sensores[3]) * 1;
	convertido[4] = float(sensores[4]) * 2;
	convertido[5] = float(sensores[5]) * 3;

	//Calcula a posição
	posicao = 0;

	for (int i = 0; i < 6; i++)
	{
		posicao += convertido[i];
	}
	erroLinha = posicao;

	//Calculo do PID

	integralLinha += erroLinha * ki;

	diferenca = (kp*erroLinha) + (kd*(erroLinha - erroLinhaAnterior)) + integralLinha;

	erroLinhaAnterior = erroLinha;

	//Calcula as velocidades dos motores
	velocidadeesq = velocidade + int(diferenca);
	velocidadedir = velocidade - int(diferenca);

	//Normaliza as velocidades dos motores para que elas não ultrapassem a velocidade máxima
	if (velocidadeesq > velocidade)
	{
		velocidadeesq = velocidade;
	}
	else if (velocidadeesq < -velocidade)
	{
		velocidadeesq = -velocidade;
	}

	if (velocidadedir > velocidade)
	{
		velocidadedir = velocidade;
	}
	else if (velocidadedir < -velocidade)
	{
		velocidadedir = -velocidade;
	}

	//Envia as velocidade aos motores
	motores.setLeftSpeed(velocidadeesq);
	motores.setRightSpeed(velocidadedir);
}

void pidLinha(int velocidade)
{
	//Calcula a posição do robô e executa a resposta de acordo com o algoritimo PID


	float convertido[6], //Recebe os valores dos sensores escalados/ponderados
		posicao = 0, //Recebe a posição da linha
		erroLinha = 0, //erroLinha da posição do motor
		kp = 0.2, //Coeficiente proporcional
		ki = 0.0,
		kd = 0.1, //Coeficiente derivativo
		diferenca; //Diferença entre as velocidades dos motores

		//Leitura dos sensores
	refletancia.read(sensores);

	//Calcula os dados dos sensores ponderados
	convertido[0] = float(sensores[0]) * -3;
	convertido[1] = float(sensores[1]) * -2;
	convertido[2] = float(sensores[2]) * -1;
	convertido[3] = float(sensores[3]) * 1;
	convertido[4] = float(sensores[4]) * 2;
	convertido[5] = float(sensores[5]) * 3;

	//Calcula a posição
	posicao = 0;

	for (int i = 0; i < 6; i++)
	{
		posicao += convertido[i];
	}

	erroLinha = posicao;
	integralLinha += erroLinha * ki;

	//Calculo do PID
	diferenca = (kp*erroLinha) + (kd*(erroLinha - erroLinhaAnterior)) + integralLinha;

	erroLinhaAnterior = erroLinha;

	//Calcula as velocidades dos motores
	velocidadeesq = velocidade + int(diferenca);
	velocidadedir = velocidade - int(diferenca);

	//Normaliza as velocidades dos motores para que elas não ultrapassem a velocidade máxima
	if (velocidadeesq > velocidade)
	{
		velocidadeesq = velocidade;
	}
	else if (velocidadeesq < -velocidade)
	{
		velocidadeesq = -velocidade;
	}
	if (velocidadedir > velocidade)
	{
		velocidadedir = velocidade;
	}
	else if (velocidadedir < -velocidade)
	{
		velocidadedir = -velocidade;
	}

	//Envia as velocidade aos motores
	motores.setLeftSpeed(velocidadeesq);
	motores.setRightSpeed(velocidadedir);
}

void pidAjuste()
{

	float leitura[5];

	MediaMovel filtro(5, leitura);

	while (1)
	{
		//Calcula a posição do robô e executa a resposta de acordo com o algoritimo PID  
		float convertido[6], //Recebe os valores dos sensores escalados/ponderados
			posicao = 0, //Recebe a posição da linha
			diferenca, //Diferença entre as velocidades dos motores
			kp = 0.2,
			ki = 0.0,
			kd = 0.1;

		//Leitura dos sensores
		refletancia.read(sensores);

		//Calcula os dados dos sensores ponderados
		convertido[0] = float(sensores[0]) * -3;
		convertido[1] = float(sensores[1]) * -2;
		convertido[2] = float(sensores[2]) * -1;
		convertido[3] = float(sensores[3]) * 1;
		convertido[4] = float(sensores[4]) * 2;
		convertido[5] = float(sensores[5]) * 3;

		//Calcula a posição
		posicao = 0;

		for (int i = 0; i < 6; i++)
		{
			posicao += convertido[i];
		}
		erroLinha = posicao;

		//Calculo do PID

		integralLinha += erroLinha * ki;

		diferenca = (kp*erroLinha) + (kd*(erroLinha - erroLinhaAnterior)) + integralLinha;

		erroLinhaAnterior = erroLinha;

		//Calcula as velocidades dos motores
		velocidadeesq = +int(diferenca);
		velocidadedir = -int(diferenca);

		//Normaliza as velocidades dos motores para que elas não ultrapassem a velocidade máxima
		if (velocidadeesq > velocidade)
		{
			velocidadeesq = velocidade;
		}
		else if (velocidadeesq < -velocidade)
		{
			velocidadeesq = -velocidade;
		}

		if (velocidadedir > velocidade)
		{
			velocidadedir = velocidade;
		}
		else if (velocidadedir < -velocidade)
		{
			velocidadedir = -velocidade;
		}

		//Envia as velocidade aos motores
		motores.setLeftSpeed(velocidadeesq);
		motores.setRightSpeed(velocidadedir);

		filtro.entrar(erroLinha);

		if (filtro.sair() < 60 && abs(erroLinha) < 250)
		{
			break;
		}
	}

	para(100);

}
