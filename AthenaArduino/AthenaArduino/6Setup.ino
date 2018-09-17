void setup()
{
	//Inverte o motor esquerdo (invertido mecânicamente)
	motores.flipLeftMotor(true);

	//Inicia os motores
	braco.attach(44);
	garra.attach(46);
	ponta.attach(45);

	garra.posicao = 9;

	garra.girar(10);

	braco.posicao = 179;
	braco.girar(180);

	ponta.posicao = 79;
	ponta.girar(80);

	refletancia.init();

	Wire.begin();

	laserDir.setShutDown(32);

	laserDir.desabilitar();

	laserEsq.setAddress(0x23);
	delay(10);

	laserDir.habilitar();

	laserDir.iniciar();
	laserEsq.iniciar();
	//Inicia o acelerometro
	acelerometro.init();
	acelerometro.enableDefault();

	//Inicia o giroscópio
	giroscopio.init();
	giroscopio.enableDefault();

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
