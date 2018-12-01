void pidGiro(int grau) //Implementa um controlador PID para rotacionar o robô
{
	float kp = 10.13, //Constante Kp do PID
		ki = 0.0, //Constante Ki do PID
		kd = 39.46, //Contante Kd do PID
		erro = 0, //Erro
		erroAnt = 0, //Erro anterior
		integral = 0; //Valor da integral do erro

	int velocidadeEsq, //Velocidade a ser comandada no motor esquerdo
		velocidadeDir, //Velocidade a ser comandada no motor direito
		velocidade, //Velocidade saída do controlador
		resetIntegral = 0; //Tempo de reset para integral

	//Calibra o giroscopio
	giroscopio.reset();

	para(500);

	giroscopio.calibrar(eZ);

	unsigned long tempo = millis();

	//Efetua a rotação
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

void pidDeslocamento(float distancia) //Implementa um controle PID para movimentar linearmente o robô
{
	float kp = 5, //Constante Kp do controle PID
		ki = 15, //Constante Ki do controle PID
		kd = 0, //Controle Kd do controle PID
		erro = 0, //Erro
		erroAnt = 0, //Erro anterior
		integral = 0; //Integral do erro

	int velocidadeEsq, //Velocidade a ser comandada ao motor esquerdo
		velocidadeDir, //Velocidade a ser comandada ao motor direito
		velocidade, //Velocidade saída do controlador
		resetIntegral = 0; //Tempo de reset da integral

	//Calibra o sensor acelerômetro
	acelerometro.reset();

	para(500);

	acelerometro.calibrar(eX);

	//Efetua a movimentação
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

void para(int tempo) //Para o robô por um tempo, ou até pressionar o botão (tempo = 0)
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

void pidLinha() //Calcula a posição do robô em relação a linha e executa a resposta de acordo com o algoritimo PID  
{
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

void pidLinha(int velocidade) //Calcula a posição do robô em relação a linha e executa a resposta de acordo com o algoritimo PID, com velocidade dada
{
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

void pidAjuste() //Ajusta o robô a linha (não funciona)
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

void testeGarra() //Efetua um teste da garra
{
	garra.girar(180);
	braco.girar(0);
	ponta.girar(90);
	delay(500);
	para(1);
	ponta.girar(180);
	delay(500);
	braco.girar(180);
	garra.girar(0);
}

void testeUltrassom() //Efetua um teste dos sensores ultrassônicos, enviando as leituras para a porta serial
{
	ultraCima.leitura();
	ultraFrente.leitura();
	//ultraEsq.leitura();
	//ultraDir.leitura();

	if (bluetooth)
	{
		Serial3.print(" Cima: ");
		Serial3.print(ultraCima.distancia);
		Serial3.print("; Frente: ");
		Serial3.print(ultraFrente.distancia);
		/*Serial3.print("; Esq: ");
		Serial3.print(ultraEsq.distancia);
		Serial3.print("; Dir: ");
		Serial3.print(ultraDir.distancia);*/
		Serial3.println();
	}
	else
	{
		Serial.print(" Cima: ");
		Serial.print(ultraCima.distancia);
		Serial.print("; Frente: ");
		Serial.print(ultraFrente.distancia);
		/*Serial.print("; Esq: ");
		Serial.print(ultraEsq.distancia);
		Serial.print("; Dir: ");
		Serial.println(ultraDir.distancia);*/
		Serial.println();
	}
}

void testeLaser() //Efetua um teste dos sensores laser, enviando a leitura para a serial
{
	laserDir.leitura();
	laserEsq.leitura();

	if (bluetooth)
	{
		Serial3.print(" Esq: ");
		Serial3.print(laserEsq.distancia);
		Serial3.print("; Dir: ");
		Serial3.println(laserDir.distancia);
	}
	else
	{
		Serial.print(" Esq: ");
		Serial.print(laserEsq.distancia);
		Serial.print("; Dir: ");
		Serial.println(laserDir.distancia);
	}
}