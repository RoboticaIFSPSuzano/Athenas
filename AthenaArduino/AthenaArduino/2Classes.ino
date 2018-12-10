//Declaração de Classes --------------------------------------------------

class Giroscopio : public L3G //Controla o sensor giroscópio
{
public:
	struct Dados
	{
		float x,
			y,
			z;
	}velocidade, grau, velocidadeAnt, ganho, velocidadeBruta, ganhoBruto, maximo, minimo;

	unsigned long tempo; //Tempo da última leitura
	float deltaT; //Tempo entre duas leituras

	//Filtros de Kalman
	FiltroKalman1D filtroX,
		filtroY,
		filtroZ;

	Giroscopio() //Inicia os filtros e os ganhos
	{
		//Suave/Rápido
		filtroX.init(0.0009, 0.15);
		filtroY.init(0.0009, 0.15);
		filtroZ.init(0.0009, 0.15);

		ganho.x = 0;
		ganho.y = 0;
		ganho.z = 0;

		ganhoBruto.x = 0;
		ganhoBruto.y = 0;
		ganhoBruto.z = 0;
	}

	void calibrar() //Calibra os três eixos
	{
		Dados somatorio,
			somatorioBruto;

		somatorio.x = 0;
		somatorio.y = 0;
		somatorio.z = 0;

		somatorioBruto.x = 0;
		somatorioBruto.y = 0;
		somatorioBruto.z = 0;

		for (int i = 0; i < 100; i++)
		{
			leitura();

			somatorio.x += velocidade.x;
			somatorio.y += velocidade.y;
			somatorio.z += velocidade.z;

			somatorioBruto.x += velocidadeBruta.x;
			somatorioBruto.y += velocidadeBruta.y;
			somatorioBruto.z += velocidadeBruta.z;

			if (i == 0)
			{
				maximo.x = velocidade.x;
				maximo.y = velocidade.y;
				maximo.z = velocidade.z;

				minimo.x = velocidade.x;
				minimo.y = velocidade.y;
				minimo.z = velocidade.z;
			}
			else
			{
				if (velocidade.x > maximo.x)
				{
					maximo.x = velocidade.x;
				}
				if (velocidade.x < minimo.x)
				{
					minimo.x = velocidade.x;
				}

				if (velocidade.y > maximo.y)
				{
					maximo.y = velocidade.y;
				}
				if (velocidade.y < minimo.y)
				{
					minimo.y = velocidade.y;
				}

				if (velocidade.z > maximo.z)
				{
					maximo.z = velocidade.z;
				}
				if (velocidade.z < minimo.z)
				{
					minimo.z = velocidade.z;
				}

			}

		}

		ganho.x += somatorio.x / 100;
		ganho.y += somatorio.y / 100;
		ganho.z += somatorio.z / 100;

		ganhoBruto.x += somatorioBruto.x / 100;
		ganhoBruto.y += somatorioBruto.y / 100;
		ganhoBruto.z += somatorioBruto.z / 100;

		maximo.x -= ganho.x;
		maximo.y -= ganho.y;
		maximo.z -= ganho.z;

		minimo.x -= ganho.x;
		minimo.y -= ganho.y;
		minimo.z -= ganho.z;
	}

	void calibrar(int eixo) //Calibra apenas o eixo selecionado
	{

		float somatorio = 0,
			somatorioBruto = 0;


		switch (eixo)
		{
		case 1:
			for (int i = 0; i < 100; i++)
			{
				leitura(eixo);

				somatorio += velocidade.x;

				somatorioBruto += velocidadeBruta.x;

				if (i == 0)
				{
					maximo.x = velocidade.x;
					minimo.x = velocidade.x;
				}
				else
				{
					if (velocidade.x > maximo.x)
					{
						maximo.x = velocidade.x;
					}
					if (velocidade.x < minimo.x)
					{
						minimo.x = velocidade.x;
					}
				}
			}

			ganho.x += somatorio / 100;

			ganhoBruto.x += somatorioBruto / 100;

			maximo.x -= ganho.x;
			minimo.x -= ganho.x;
			break;

		case 2:
			for (int i = 0; i < 100; i++)
			{
				leitura(eixo);

				somatorio += velocidade.y;

				somatorioBruto += velocidadeBruta.y;

				if (i == 0)
				{
					maximo.y = velocidade.y;
					minimo.y = velocidade.y;
				}
				else
				{
					if (velocidade.y > maximo.y)
					{
						maximo.y = velocidade.y;
					}
					if (velocidade.y < minimo.y)
					{
						minimo.y = velocidade.y;
					}
				}
			}

			ganho.y += somatorio / 100;

			ganhoBruto.y += somatorioBruto / 100;

			maximo.y -= ganho.y;
			minimo.y -= ganho.y;
			break;

		case 3:
			for (int i = 0; i < 100; i++)
			{
				leitura(eixo);

				somatorio += velocidade.z;

				somatorioBruto += velocidadeBruta.z;

				if (i == 0)
				{
					maximo.z = velocidade.z;
					minimo.z = velocidade.z;
				}
				else
				{
					if (velocidade.z > maximo.z)
					{
						maximo.z = velocidade.z;
					}
					if (velocidade.z < minimo.z)
					{
						minimo.z = velocidade.z;
					}
				}
			}

			ganho.z += somatorio / 100;

			ganhoBruto.z += somatorioBruto / 100;

			maximo.z -= ganho.z;
			minimo.z -= ganho.z;
			break;
		}
	}


	void leitura() //Mensura a aceleração do robô em m/s^2 nos três eixos, filtrando utilizando um filtro de Kalman
	{

		read();

		velocidadeBruta.x = ((g.x * 8.75) / 1000);
		velocidadeBruta.y = ((g.y * 8.75) / 1000);
		velocidadeBruta.z = ((g.z * 8.75) / 1000);

		Vec1f entradaX(velocidadeBruta.x);
		Vec1f entradaY(velocidadeBruta.y);
		Vec1f entradaZ(velocidadeBruta.z);

		filtroX.update(entradaX);
		filtroY.update(entradaY);
		filtroZ.update(entradaZ);

		Vec1f saidaX = filtroX.getEstimation();
		Vec1f saidaY = filtroY.getEstimation();
		Vec1f saidaZ = filtroZ.getEstimation();

		velocidade.x = saidaX[0] - ganho.x;
		velocidade.y = saidaY[0] - ganho.y;
		velocidade.z = saidaZ[0] - ganho.z;

		if (velocidade.x <= maximo.x && velocidade.x >= minimo.x)
		{
			velocidade.x = 0;
		}

		if (velocidade.y <= maximo.y && velocidade.y >= minimo.y)
		{
			velocidade.y = 0;
		}

		if (velocidade.z <= maximo.x && velocidade.z >= minimo.y)
		{
			velocidade.z = 0;
		}

	}


	void leitura(int eixo) //Mensura a aceleração apenas no eixo selecionado, com filtro de Kalman
	{
		read();

		Vec1f entrada,
			saida;


		switch (eixo)
		{
		case 1:
			velocidadeBruta.x = ((g.x * 8.75) / 1000);

			entrada = velocidadeBruta.x;

			filtroX.update(entrada);

			saida = filtroX.getEstimation();

			velocidade.x = saida[0] - ganho.x;

			if (velocidade.x <= maximo.x && velocidade.x > 0.0)
			{
				velocidade.x = 0;
			}

			if (velocidade.x >= minimo.x && velocidade.x < 0.0)
			{
				velocidade.x = 0;
			}
			break;

		case 2:
			velocidadeBruta.y = ((g.y * 8.75) / 1000);

			entrada = velocidadeBruta.y;

			filtroY.update(entrada);

			saida = filtroY.getEstimation();

			velocidade.y = saida[0] - ganho.y;

			if (velocidade.y <= maximo.y && velocidade.y > 0.0)
			{
				velocidade.y = 0;
			}

			if (velocidade.y >= minimo.y && velocidade.y < 0.0)
			{
				velocidade.y = 0;
			}
			break;

		case 3:
			velocidadeBruta.z = ((g.z * 8.75) / 1000);

			entrada = velocidadeBruta.z;

			filtroZ.update(entrada);

			saida = filtroZ.getEstimation();

			velocidade.z = saida[0] - ganho.z;

			if (velocidade.z <= maximo.z && velocidade.z > 0.0)
			{
				velocidade.z = 0;
			}

			if (velocidade.z >= minimo.z && velocidade.z < 0.0)
			{
				velocidade.z = 0;
			}
			break;

		default:
			break;
		}
	}


	void leituraBruta() //Faz a leitura sem filtragem dos três eixos
	{
		read();

		velocidade.x = ((g.z * 8.75) / 1000) - ganhoBruto.x;
		velocidade.y = ((g.z * 8.75) / 1000) - ganhoBruto.y;
		velocidade.z = ((g.z * 8.75) / 1000) - ganhoBruto.z;
	}

	void reset() //Reseta todas as estrutras de dados
	{
		grau.z = 0;

		velocidadeAnt.z = 0;

		velocidade.z = 0;

		tempo = 0;

		deltaT = 0;
	}

	void medirGrau() //Mensura a quantidade de graus percorridos
	{
		if (tempo == 0)
		{
			tempo = millis();
		}

		//Define todos os pontos anteriores
		velocidadeAnt.z = velocidade.z;

		leitura(eZ);

		//Descobre o tempo percorrido entre as duas leituras
		deltaT = (float(millis() - tempo)) / 1000;

		//Integra a velocidade angular para obter os graus percorridos
		grau.z += ((velocidade.z + velocidadeAnt.z) * deltaT) / 2;

		tempo = millis();
	}
};

class Acelerometro : public LSM303 //Controla o sensor acelerômetro
{
public:
	struct Dados
	{
		float x,
			y,
			z;
	}velocidade, deslocamento, velocidadeAnt, aceleracao, aceleracaoAnt, ganho, aceleracaoBruta, ganhoBruto, maximo, minimo;


	unsigned long tempo; //Tempo da última leitura
	float deltaT; //Diferença de tempo entre duas leituras

	float medidaX[nMedidaAcel], //Para filtro do eixo X
		eixoX; //Leitura filtrada do eixo x 

//Filtros de Kalman
	FiltroKalman1D filtroX, filtroY, filtroZ;


	Acelerometro() //Inicia os filtros e os ganhos
	{
		//Suave/Rápido
		filtroX.init(0.0009, 0.15);
		filtroY.init(0.0009, 0.15);
		filtroZ.init(0.0009, 0.15);

		ganho.x = 0;
		ganho.y = 0;
		ganho.z = 0;

		ganhoBruto.x = 0;
		ganhoBruto.y = 0;
		ganhoBruto.z = 0;
	}

	void calibrar() //Calibra os três eixos
	{
		Dados somatorio,
			somatorioBruto;

		somatorio.x = 0;
		somatorio.y = 0;
		somatorio.z = 0;

		somatorioBruto.x = 0;
		somatorioBruto.y = 0;
		somatorioBruto.z = 0;

		for (int i = 0; i < 100; i++)
		{
			leitura();

			somatorio.x += aceleracao.x;
			somatorio.y += aceleracao.y;
			somatorio.z += aceleracao.z;

			somatorioBruto.x += aceleracaoBruta.x;
			somatorioBruto.y += aceleracaoBruta.y;
			somatorioBruto.z += aceleracaoBruta.z;

			if (i == 0)
			{
				maximo.x = aceleracao.x;
				maximo.y = aceleracao.y;
				maximo.z = aceleracao.z;

				minimo.x = aceleracao.x;
				minimo.y = aceleracao.y;
				minimo.z = aceleracao.z;
			}
			else
			{
				if (aceleracao.x > maximo.x)
				{
					maximo.x = aceleracao.x;
				}
				if (aceleracao.x < minimo.x)
				{
					minimo.x = aceleracao.x;
				}

				if (aceleracao.y > maximo.y)
				{
					maximo.y = aceleracao.y;
				}
				if (aceleracao.y < minimo.y)
				{
					minimo.y = aceleracao.y;
				}

				if (aceleracao.z > maximo.z)
				{
					maximo.z = aceleracao.z;
				}
				if (aceleracao.z < minimo.z)
				{
					minimo.z = aceleracao.z;
				}

			}
		}

		ganho.x += somatorio.x / 100;
		ganho.y += somatorio.y / 100;
		ganho.z += somatorio.z / 100;

		ganhoBruto.x += somatorioBruto.x / 100;
		ganhoBruto.y += somatorioBruto.y / 100;
		ganhoBruto.z += somatorioBruto.z / 100;

		maximo.x -= ganho.x;
		maximo.y -= ganho.y;
		maximo.z -= ganho.z;

		minimo.x -= ganho.x;
		minimo.y -= ganho.y;
		minimo.z -= ganho.z;
	}


	void calibrar(int eixo) //Calibra apenas o eixo selecionado
	{

		float somatorio = 0,
			somatorioBruto = 0;


		switch (eixo)
		{
		case 1:
			for (int i = 0; i < 100; i++)
			{
				leitura(eixo);

				somatorio += aceleracao.x;

				somatorioBruto += aceleracaoBruta.x;

				if (i == 0)
				{
					maximo.x = aceleracao.x;
					minimo.x = aceleracao.x;
				}
				else
				{
					if (aceleracao.x > maximo.x)
					{
						maximo.x = aceleracao.x;
					}
					if (aceleracao.x < minimo.x)
					{
						minimo.x = aceleracao.x;
					}
				}
			}

			ganho.x += somatorio / 100;

			ganhoBruto.x += somatorioBruto / 100;

			maximo.x -= ganho.x;
			minimo.x -= ganho.x;
			break;

		case 2:
			for (int i = 0; i < 100; i++)
			{
				leitura(eixo);

				somatorio += aceleracao.y;

				somatorioBruto += aceleracaoBruta.y;

				if (i == 0)
				{
					maximo.y = aceleracao.y;
					minimo.y = aceleracao.y;
				}
				else
				{
					if (aceleracao.y > maximo.y)
					{
						maximo.y = aceleracao.y;
					}
					if (aceleracao.y < minimo.y)
					{
						minimo.y = aceleracao.y;
					}
				}
			}

			ganho.y += somatorio / 100;

			ganhoBruto.y += somatorioBruto / 100;

			maximo.y -= ganho.y;
			minimo.y -= ganho.y;
			break;

		case 3:
			for (int i = 0; i < 100; i++)
			{
				leitura(eixo);

				somatorio += aceleracao.z;

				somatorioBruto += aceleracaoBruta.z;

				if (i == 0)
				{
					maximo.z = aceleracao.z;
					minimo.z = aceleracao.z;
				}
				else
				{
					if (aceleracao.z > maximo.z)
					{
						maximo.z = aceleracao.z;
					}
					if (aceleracao.z < minimo.z)
					{
						minimo.z = aceleracao.z;
					}
				}
			}

			ganho.z += somatorio / 100;

			ganhoBruto.z += somatorioBruto / 100;

			maximo.z -= ganho.z;
			minimo.z -= ganho.z;
			break;
		}
	}

	void leitura() //Mensura a aceleração do robô em m/s^2 nos três eixos, filtrando utilizando um filtro de Kalman
	{

		readAcc();

		aceleracaoBruta.x = (((float(a.x) * 980665) / 100000) / 16000) * 100;
		aceleracaoBruta.y = (((float(a.y) * 980665) / 100000) / 16000) * 100;
		aceleracaoBruta.z = (((float(a.z) * 980665) / 100000) / 16000) * 100;

		Vec1f entradaX(aceleracaoBruta.x);
		Vec1f entradaY(aceleracaoBruta.y);
		Vec1f entradaZ(aceleracaoBruta.z);

		filtroX.update(entradaX);
		filtroY.update(entradaY);
		filtroZ.update(entradaZ);

		Vec1f saidaX = filtroX.getEstimation();
		Vec1f saidaY = filtroY.getEstimation();
		Vec1f saidaZ = filtroZ.getEstimation();

		aceleracao.x = saidaX[0] - ganho.x;
		aceleracao.y = saidaY[0] - ganho.y;
		aceleracao.z = saidaZ[0] - ganho.z;

		if (aceleracao.x <= maximo.x && aceleracao.x >= minimo.x)
		{
			aceleracao.x = 0;
		}

		if (aceleracao.y <= maximo.y && aceleracao.y >= minimo.y)
		{
			aceleracao.y = 0;
		}

		if (aceleracao.z <= maximo.z && aceleracao.z >= minimo.y)
		{
			aceleracao.z = 0;
		}


	}


	void leitura(int eixo) //Mensura a aceleração apenas no eixo selecionado, com filtro de Kalman
	{
		readAcc();

		Vec1f entrada;
		Vec1f saida;

		switch (eixo)
		{
		case 1:
			aceleracaoBruta.x = (((float(a.x) * 980665) / 100000) / 16000) * 100;

			entrada = aceleracaoBruta.x;

			filtroX.update(entrada);

			saida = filtroX.getEstimation();

			aceleracao.x = saida[0] - ganho.x;

			if (aceleracao.x <= maximo.x && aceleracao.x > 0.0)
			{
				aceleracao.x = 0;
			}

			if (aceleracao.x >= minimo.x && aceleracao.x < 0.0)
			{
				aceleracao.x = 0;
			}
			break;

		case 2:
			aceleracaoBruta.y = (((float(a.y) * 980665) / 100000) / 16000) * 100;

			entrada = aceleracaoBruta.y;

			filtroY.update(entrada);

			saida = filtroY.getEstimation();

			aceleracao.y = saida[0] - ganho.y;

			if (aceleracao.y <= maximo.y && aceleracao.y > 0.0)
			{
				aceleracao.y = 0;
			}

			if (aceleracao.y >= minimo.y && aceleracao.y < 0.0)
			{
				aceleracao.y = 0;
			}
			break;

		case 3:
			aceleracaoBruta.z = (((float(a.z) * 980665) / 100000) / 16000) * 100;

			entrada = aceleracaoBruta.z;

			filtroZ.update(entrada);

			saida = filtroZ.getEstimation();

			aceleracao.z = saida[0] - ganho.z;

			if (aceleracao.z <= maximo.z && aceleracao.z > 0.0)
			{
				aceleracao.z = 0;
			}

			if (aceleracao.z >= minimo.z && aceleracao.z < 0.0)
			{
				aceleracao.z = 0;
			}
			break;

		default:
			break;
		}
	}

	void leituraBruta() //Faz a leitura sem filtragem dos três eixos
	{
		readAcc();

		aceleracaoBruta.x = (((float(a.x) * 980665) / 100000) / 16000) - ganhoBruto.x;
		aceleracaoBruta.y = (((float(a.y) * 980665) / 100000) / 16000) - ganhoBruto.y;
		aceleracaoBruta.z = (((float(a.z) * 980665) / 100000) / 16000) - ganhoBruto.z;
	}

	void reset() //Reseta todos os dados
	{
		velocidade.x = 0;
		velocidade.y = 0;
		velocidade.z = 0;

		deslocamento.x = 0;
		deslocamento.y = 0;
		deslocamento.z = 0;

		velocidadeAnt.x = 0;
		velocidadeAnt.y = 0;
		velocidadeAnt.z = 0;

		aceleracaoAnt.x = 0;
		aceleracaoAnt.y = 0;
		aceleracaoAnt.z = 0;

		aceleracao.x = 0;
		aceleracao.y = 0;
		aceleracao.z = 0;

		tempo = 0;

		deltaT = 0;
	}

	void medirDeslocamento() //Mensura o deslocamento do robô
	{
		if (tempo == 0)
		{
			tempo = millis();
		}

		//Define a aceleração do momento anterior
		aceleracaoAnt.x = aceleracao.x;

		//Define a velocidade do momento anterior
		velocidadeAnt.x = velocidade.x;

		//Mensura as novas acelerações
		leitura(eX);
		//Calcula a diferença de tempo entre as leituras (em segundos)
		deltaT = (float(millis() - tempo)) / 1000;

		//Integra a aceleração para descobrir a velocidade
		velocidade.x += ((aceleracao.x + aceleracaoAnt.x) * deltaT) / 2;

		//Integra a velocidade para descobrir o deslocamento
		deslocamento.x += ((velocidade.x + velocidadeAnt.x) * deltaT) / 2;

		//Define o tempo da leitura
		tempo = millis();
	}

	void medirFiltrado() //Lê e filtra as leituras do eixo X, utilizando média móvel
	{

		float soma = 0;//Valor de soma de todos os valores

		//Lê o sensor acelerômetro
		leituraBruta();

		//Movimenta os valores atuais para os antigos
		for (int k = nMedidaAcel; k > 0; k--)
		{
			medidaX[k] = medidaX[k - 1]; //Valor antigo = valor recente
			soma += medidaX[k]; //Soma os valores das leituras
		}

		//Coloca o valor mais atual como a última leitura
		medidaX[0] = aceleracaoBruta.x - -0.09646234;

		//Soma o valor mais atual
		soma += medidaX[0];

		//Calcula a média
		eixoX = soma / nMedidaAcel;
	}
};

class Cor
{
public:
	const int s0 = 40,
		s1 = 48,
		s2 = 27,
		s3 = 26,
		out = 28;

	int red = 0,
		green = 0,
		blue = 0,
		cor,
		leiturasCor[5];

	void leitura() //Lê o sensor de cor e efetua uma filtragem por média móvel
	{
		for (int i = 0; i < 5; i++)
		{
			digitalWrite(s2, LOW);
			digitalWrite(s3, LOW);
			red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
			digitalWrite(s3, HIGH);
			blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
			digitalWrite(s2, HIGH);
			green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

			float leituraCor = (red + blue + green) / 3;

			//Média móvel
			float soma = 0;

			for (int j = 0; i < 4; j++)
			{
				leiturasCor[j + 1] = leiturasCor[j];
				soma += leiturasCor[j];
			}

			leiturasCor[0] = leituraCor;

			soma += leiturasCor[0];

			cor = int(soma / 5);
		}
	}
};

class ServoMotor : public Servo
{
public:
	//Servo *motor;

	int posicao;

	/*ServoMotor(Servo *motor)
	  {
		this -> motor = motor;

		posicao = 0;
	}*/

	ServoMotor() : Servo()
	{
	}

	void girar(int posicao)
	{
		if (posicao > this->posicao)
		{
			for (int localAtual = this->posicao; localAtual <= posicao; localAtual++)
			{
				write(localAtual);
				delay(15);
			}
		}
		else
		{
			for (int localAtual = this->posicao; localAtual >= posicao; localAtual--)
			{
				write(localAtual);
				delay(15);
			}
		}

		this->posicao = posicao;
	}
};

class SensorUltrassonico : public Ultrasonic
{
public:
	//Ultrasonic *ultrassonico;

	float distancia, a, b;

	int trigger,
		echo;

	/*SensorUltrassonico(Ultrasonic *ultrassonico)
	  {
		this-> ultrassonico = ultrassonico;

		distancia = 0;
	  }*/

	SensorUltrassonico(int trigger, int echo) : Ultrasonic(trigger, echo)
	{
		this->trigger = trigger;
		this->echo = echo;
	}

	long timing(long tempo)
	{
		digitalWrite(trigger, LOW);
		delayMicroseconds(2);
		digitalWrite(trigger, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigger, LOW);
		return pulseIn(echo, HIGH, tempo);
	}

	void leitura()
	{
		unsigned long tempo = timing(10000);
		distancia = convert(tempo, 1);
	}
	/*void leitura()
	  {
		unsigned long tempo = ultrassonico->timing(10000);
		distancia = ultrassonico->convert(tempo, 1);
	  }*/
};

class Laser : public VL53L0X //Controla os sensores laser
{
public:

	float distancia; //Leitura do sensor
	int pinShutDown; //Pino de shutdown

	void setShutDown(int pin) //Define o pino de shutdown
	{
		pinShutDown = pin;

		pinMode(pinShutDown, OUTPUT);
	}

	void iniciar() //Inicia o sensor em leitura contínua
	{
		init();
		setTimeout(500);
		startContinuous();
	}

	void leitura() //Efetua uma leitura e converte para cm
	{
		distancia = readRangeContinuousMillimeters();
		distancia /= 10;
	}

	void habilitar() //Habilita o sensor
	{
		digitalWrite(pinShutDown, HIGH);
	}

	void desabilitar() //Desabilita o sensor
	{
		digitalWrite(pinShutDown, LOW);
	}
};

class Buzzer
{
public:

	int porta, //Porta do buzzer
		estado; //Estado (ligado 1, desligado 0)

	Buzzer(int porta) //Inicia o buzzer
	{
		this->porta = porta;

		//Define o estado inicial como desligado
		estado = 0; 

		digitalWrite(porta, estado);
	}

	void pulsar(int vezes) //Pulsa n vezes
	{
		if (estado)
		{
			digitalWrite(porta, LOW);
			delay(1000);

			estado = 0;
		}

		for (int i = 0; i < vezes; i++)
		{
			digitalWrite(porta, HIGH);
			delay(1000);
			digitalWrite(porta, LOW);
			delay(1000);
		}
	}

	void ligar() //Liga ou desliga o buzzer
	{
		if (estado)
		{
			digitalWrite(porta, LOW);
			estado = 0;
		}
		else
		{
			digitalWrite(porta, HIGH);
			estado = 1;
		}
	}
};

class MediaMovel //Implementa um filtro do tipo média móvel
{
public:
	float *leitura, //Vetor para armazenar as leituras
		valor; //Valor de saída

	int periodo; //Período para filtragem

	MediaMovel(int periodo, float *leitura) //Recebe vetor e o período
	{
		this->periodo = periodo;
		this->leitura = leitura;

		for (int k = 0; k < periodo; k++) //Inicia o vetor de leituras
		{
			leitura[k] = 0.0;
		}
	}

	void entrar(float entrada) //Entra uma nova leitura
	{
		//Movimenta os valores atuais para os antigos
		float soma = 0;

		for (int k = periodo; k > 0; k--) //Movimenta os valores uma varíavel para cima
		{
			leitura[k] = leitura[k - 1]; //Valor antigo = valor recente
			soma += leitura[k]; //Soma os valores das leituras
		}

		leitura[0] = entrada; //Insere o novo valor no vetor
		valor += leitura[0]; //Soma o novo valor

		valor = soma / periodo; //Calcula o valor de saída
	}

	float sair() //Retorna o valor filtrado
	{
		return valor;
	}

};
//Fim da declaração de classes--------------------------------------------
