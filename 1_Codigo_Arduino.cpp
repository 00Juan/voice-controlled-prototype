
#include<Wire.h>


#include <EtherCard.h>
#include <Modbus.h>
#include <ModbusIP_ENC28J60.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial (2, 3);
#include <Wire.h>
const int VOZ = 50;

ModbusIP mb;

//VARIABLES COMMS
int paso, divisor;
int t = 25;
int t1 = 5;
int offset = 2;

unsigned long int tiempoA, tiempoT, tiempoA1, tiempoT1;
bool salidaBin[9];
bool entradaBin[9];
int entradaDec;
int contBit = 0;

int pin_comunicacion = 2;

int size_paquete = 9;
unsigned long int p,p1;



//FUNCIONES

void transformar(int input);
int transformar2();
bool leer();
bool leerBot();
void escribir(bool a);
int leerPot();
void transformar(int input);
void mostrarDec(int in);

void setup()
{
  Serial.begin(9600);
  pinMode(pin_comunicacion, INPUT);
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  //dirección IP
  byte ip [] = {192, 168, 1, 25};

  mb.config (mac, ip); // se confirma las direcciones
  mb.addIreg(VOZ);
  // Wire.begin();
}
void loop()
{
  
  //Serial.println(paso);
 
  if(paso<3)
  {
  mb.task();
  mb.Ireg(VOZ, transformar2());
  }
  
  
  switch (paso)
  {
    case 0:
      pinMode(pin_comunicacion, INPUT);
      if (!leer()) paso++;
      break;

    case 1:
      if (leer()) paso++;
      break;


    case 2:
      if (!leer())
      {
        Serial.println("Recibiendo: ");
        paso++;
        //delay(t1);
      }
      break;




    case 3:

      delay(t);
      entradaBin[contBit] = leer();
      //Serial.print(entradaBin[contBit]);
      contBit++;
      if (contBit > (size_paquete - 1))
      {
        //delay(t);
        //Serial.println("");
        contBit = 0;
        paso++;

      }

      break;

    case 4:
      if (!leer()) paso++;
      break;

    case 5:
      if (leer()) paso++;
      break;

    case 6:
      if (!leer())
      {
        pinMode(pin_comunicacion, OUTPUT);
        escribir(1);
        delay(t);
        paso=0;
        mostrarDec(transformar2());
      }
      break;

  

  }



}



void transformar(int input) {
  divisor = input;
  for (int j = 0; j < 8; j++)
  {
    salidaBin[7 - j] = divisor % 2;
    divisor = divisor / 2;
  }
}

int transformar2()
{
  entradaDec =
    entradaBin[0] * 128 +
    entradaBin[1] * 64 +
    entradaBin[2] * 32 +
    entradaBin[3] * 16 +
    entradaBin[4] * 8 +
    entradaBin[5] * 4 +
    entradaBin[6] * 2 +
    entradaBin[7];
  //Serial.println("");
  //Serial.println("El numero es:");
  //Serial.print(entradaDec);

  //Serial.println();
  return (entradaDec);
}



bool leer()
{
  return (digitalRead(pin_comunicacion));
}

bool leerBot()
{
  return (digitalRead(13));
}

void escribir(bool a)
{
  digitalWrite(pin_comunicacion, a);
}



void mostrarDec(int in)
{
  Serial.println(in);
}