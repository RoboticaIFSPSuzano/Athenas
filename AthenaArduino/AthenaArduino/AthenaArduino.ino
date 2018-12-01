//Inclusão de Bibliotecas ----------------------------------------------------
#include <QTRSensors.h> //Sensor de refletância
#include <ZumoReflectanceSensorArray.h> //Sensor de refletância
#include <ZumoMotors.h> //Motores
#include <Pushbutton.h> //Botão
#include <Wire.h> //Comunicação I2C
#include <LSM303.h> //Acelerômetro
#include <L3G.h> //Giroscópio
#include <Ultrasonic.h>//Ultrassônico
#include "SoftwareSerial.h"
#include <Servo.h>//Motores da garra
#include <FiltroKalman.h> //Filtro de Kalman
#include <VL53L0X.h> //Laser
#include <Filters.h> //Filtro

//Fim da inclusão de bibliotecas------------------------------------------

//Constantes
#define nMedidaAcel 20 //Medidas filtro média móvel do acelerômetro
#define eX 1
#define eY 2
#define eZ 3
#define bluetooth 1 //Utilizar (1) ou não (0) o módulo bluetooth para comunicação serial


//Declaração de Variáveis ------------------------------------------------

int velocidadeesq, //Velocidade para o motor da esquerda
velocidadedir, //Velocidade para o motor da direita
velocidade = 300, //Velocidade máxima do robô
corte = 325, //Valor de corte para os sensores de refletância - acima = preto/abaixo = branco
tempo1 = 175; //Tempo para o robô ir para frente e verificar a configuração da linha em curvas

unsigned int sensores[6]; //Recebe os valores dos sensores


int medidaAcel[nMedidaAcel], //Medidas do filtro de média móvel do sensor acelerômetro
eixoX = 0; //Valor final do filtro de média móvel do acelerômetro

//Variáveis para o pidLinha
float erroLinha = 0, 
integralLinha,
erroLinhaAnterior = 0;


//Fim da declaração de variáveis------------------------------------------