//Declara��o de objetos---------------------------------------------------

//Motores da garra
/*Servo motor1;
Servo motor2;

ServoMotor braco(&motor1); //Pin�a - ponta da garra
ServoMotor garra(&motor2); //Garra em si*/

ServoMotor braco; //Pin�a - ponta da garra
ServoMotor garra; //Garra em si
ServoMotor ponta;

//Buzzer
Buzzer buzzer(36);

//Sensores ultrass�nicos
/*Ultrasonic sensorU1(53, 52);
Ultrasonic sensorU2(23, 22);
Ultrasonic sensorU3(37, 35);
Ultrasonic sensorU4(25, 24);

SensorUltrassonico ultraEsq(&sensorU1);
SensorUltrassonico ultraDir(&sensorU2);
SensorUltrassonico ultraFrente(&sensorU3);
SensorUltrassonico ultraCima(&sensorU4);*/

SensorUltrassonico ultraEsq(53, 52);
SensorUltrassonico ultraDir(23, 22);
SensorUltrassonico ultraFrente(31, 33);
SensorUltrassonico ultraCima(25, 24);
SensorUltrassonico ultraTras;

//Laser
Laser laserDir;
Laser laserEsq;

//Sensores inerciais
Giroscopio giroscopio;
Acelerometro acelerometro;

//Classes do rob� zumo
ZumoReflectanceSensorArray refletancia; //Sensor de Reflet�ncia
ZumoMotors motores; //Motores
Pushbutton botao(ZUMO_BUTTON); //Bot�o

//Fim da declara��o de objetos--------------------------------------------
