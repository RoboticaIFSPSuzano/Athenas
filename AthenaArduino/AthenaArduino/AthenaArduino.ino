#include <QTRSensors.h> //Sensor de refletância
#include <ZumoReflectanceSensorArray.h> //Sensor de refletância
#include <ZumoMotors.h> //Motores
#include <Pushbutton.h> //Botão
#include <Wire.h>
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
#define nMedidaAcel 20
#define eX 1
#define eY 2
#define eZ 3
#define bluetooth 0


//Declaração de Variáveis ------------------------------------------------

int velocidadeesq, //Velocidade para o motor da esquerda
velocidadedir, //Velocidade para o motor da direita
velocidade = 200, //Velocidade máxima do robô
corte = 325, //Valor de corte para os sensores de refletância - acima = preto/abaixo = branco
tempo1 = 175; //Tempo para o robô ir para frente e verificar a configuração da linha em curvas

unsigned int sensores[6]; //Recebe os valores dos sensores

int obst = 1;

int medidaAcel[nMedidaAcel],
eixoX = 0;

float erroLinha = 0,
integralLinha,
erroLinhaAnterior = 0;


const int prata = 138, preto = 316, branco = 40;

//Fim da declaração de variáveis------------------------------------------