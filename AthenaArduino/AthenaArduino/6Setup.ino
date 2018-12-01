void setup()
{
	//Inverte o motor esquerdo (invertido mecânicamente)
	motores.flipLeftMotor(true);

	//Inicia os motores
	braco.attach(44);
	garra.attach(46);
	ponta.attach(45);

	//Movimenta os motores para as posições corretas
	garra.posicao = 1;

	garra.girar(0);

	braco.posicao = 179;
	braco.girar(180);

	ponta.posicao = 179;
	ponta.girar(180);

	//Inicia os sensores de refletância
	refletancia.init();

	//Inicia a comunicação I2C
	Wire.begin();

	//Configura os endereços dos sensores laser
	laserDir.setShutDown(32); //Configura o pino de desativação do sensor direito

	laserDir.desabilitar(); //Desabilita o sensor direito

	laserEsq.setAddress(0x23); //Altera o endereço do sensor esquerdo
	delay(10);

	laserDir.habilitar(); //Habilita o sensor esquerdo

	//Inicia os sensores laser
	laserDir.iniciar();
	laserEsq.iniciar();
	//Inicia o acelerometro
	acelerometro.init();
	acelerometro.enableDefault();

	//Inicia o giroscópio
	giroscopio.init();
	giroscopio.enableDefault();

	//Inicia o filtro média móvel do acelerômetro
	for (int i = 0; i < nMedidaAcel; i++)
	{
		medidaAcel[i] = 0;
		acelerometro.readAcc();
	}

	if (bluetooth == 1)
	{
		Serial3.begin(9600);
	}
	else
	{
		Serial.begin(250000);
	}
	para(100);
}
