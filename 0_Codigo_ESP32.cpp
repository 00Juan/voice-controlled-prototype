#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth no OK
#endif

BluetoothSerial SerialBT;
// PROGRAMA EMISOR////////////////////

// DECLARACIÓN DE FUNCIONES
void transformar(int input);
int transformar2();
bool leer();
bool leerBot();
void escribir(bool a);
int leerPot();
void mostrarDec(int in);
void mostrarSalidaBin();
bool activar_comunicacion(int dato_envio_voz);

bool buscar_varias(String entrada, String vector_entrada[], int tamano);
bool buscar(String entrada, String parte);
int salida(int dd1, int dd2, int dd3);

/////VARIABLES COM

int pin_comunicacion = 12;
int paso, divisor, potAux;
int t = 25;
int t1 = 5;
int offset = 2;


unsigned long int tiempoA, tiempoT, tiempoA1, tiempoT1;
bool salidaBin[9];
bool entradaBin[9];
int entradaDec;
int contBit = 0;


int size_paquete = 9;

/////VARIABLES VOZ

String voz;
String parcial;
String manual[6] = {"alime", "ete", "inz", "int", "oto", "sviado"};
String modos[6] = {"manual", "programa", "proceso", "pausa", "ejecu", "cance"};
int contador;
int pos_espacio;
int modo, objeto, estado;

bool voz_ok;
int n;

/////

void setup()
{

  Serial.begin(9600);
  SerialBT.begin("ESP32test"); // Bluetooth device name
  Serial.println("BT OK");
  pinMode(pin_comunicacion, INPUT);
  delay(1000);
}

void loop()
{

//Serial.println(paso);
 


  while (SerialBT.available()) 
  {
    voz_ok=0;
    delay(10);
    char c = SerialBT.read();


    if (c == '#') {
      break;
    
    }

    voz += c;

  }

  modo = 0;

  if(voz_ok)
  {
   
     activar_comunicacion(n);
  }
    


  if (voz.length() > 0)
    //Serial.print(voz);
  {
    modo = 0;
    objeto = 0;
    estado = 0;

    objeto = 0;
    if (buscar(voz, "poso"))
      estado = 1;
    else if (buscar(voz, "nua"))
      estado = 2;
    else if (buscar(voz, "tom"))
      estado = 3;
    else if (buscar(voz, "jec"))
      estado = 4;
    else if (buscar(voz, "aus"))
      estado = 5;
    else if (buscar(voz, "ance"))
      estado = 6;
    else if (buscar(voz, "allo"))
      estado = 7;
    else if (buscar(voz, "stad"))
      estado = 8;
    else if (buscar(voz, "rad"))
      estado = 90;
    else if (buscar(voz, "chico"))
      estado = 9;

    //01-alim 02-abrir pinza 03-bajar pinza, 04-girar pinza
    else if (buscar_varias(voz, manual, 6))

    {
      modo = 0;


      //EV02
      if (buscar(voz, "pi"))
      {
        if (buscar(voz, "abr"))
        {
          objeto = 2;
          estado = 2;
        }
        else if (buscar(voz, "cer"))
        {
          objeto = 2;
          estado = 1;
        }

        //EV03
        else if (buscar(voz, "subir"))
        {
          objeto = 3;
          estado = 1;
        }
        else if (buscar(voz, "jar"))
        {
          objeto = 3;
          estado = 2;
        }

        //EV04

        if (buscar(voz, "dida"))
        {
          objeto = 4;
          estado = 1;
        }
        else if (buscar(voz, "inta"))
        {
          objeto = 4;
          estado = 2;
        }
      }
      //M01
      else if (buscar(voz, "otor"))
      {

        objeto = 7;
        if (buscar(voz, "ara"))
        {
          estado = 1;
        }
        else if (buscar(voz, "irect"))
        {
          estado = 2;
        }
        else if (buscar(voz, "vers"))
        {
          estado = 3;
        }
      }

      else
      {
        if (buscar(voz, "aca"))
        {
          estado = 2;
        }
        else if (buscar(voz, "ete"))
        {
          estado = 1;
        }
        //EV01
        if (buscar(voz, "liment"))
        {
          objeto = 1;
        }
        //EV05
        else if (buscar(voz, "esvia"))
        {
          objeto = 5;
        }
        //EV06
        else if (buscar(voz, "reten"))
        {
          objeto = 6;
        }
      }







    }
    
  
   
    n =   salida(modo, objeto, estado);


    modo = 0;
    objeto = 0;
    estado = 0;

    voz_ok=1;
      Serial.print(voz);
    Serial.print("--->");
     Serial.println(n);
    voz = ""; //Reset the variable after initiating
  }

}




bool buscar(String entrada, String parte)
{
  String aux1;
  entrada.toLowerCase();
  if (entrada.indexOf(parte) < 0)
  {
    return false;
  }
  else
  {
    return true;
  }
}


bool buscar_varias(String entrada, String vector_entrada[], int tamano)
{
  bool ok;
  ok = 0;
  for (int i = 0; i < tamano; i++)
  {
    if (buscar(entrada, vector_entrada[i]))
    {
      ok = 1;
    }
  }
  return ok;
}


int salida(int dd1, int dd2, int dd3)
{
  return (dd1 * 100 + dd2 * 10 + dd3);
}



bool activar_comunicacion(int dato_envio_voz)
{
  
  switch (paso)
  {
  case 0:
  
    contBit = 0;
    pinMode(pin_comunicacion, OUTPUT);
    escribir(0);
    delay(t1);
    paso++;
    break;

  case 1:
  
    escribir(1);
    delay(t1);
    paso++;
    break;

  case 2:
    Serial.println("Enviando: ");
 
    
    paso++;
    escribir(0);

   

    break;

  case 3:

    delay(t - offset);
    transformar(dato_envio_voz);
   
    escribir(salidaBin[contBit]);
    

    contBit++;
    if (contBit > (size_paquete - 1))
    {
       escribir(0);
      // Serial.println("Enviado");
      delay(t-offset);
      contBit = 0;
      paso++;
      escribir(0);
      delay(t1);
      escribir(1);
      delay(t1);
      escribir(0);
      delay(t1);
      pinMode(pin_comunicacion, INPUT);
       //delay(t);
    }

    break;

  

  case 4:
    if (leer())
      paso=0;
      voz_ok=0;
      Serial.println(dato_envio_voz);
    break;

  
  
    
  }
}

void transformar(int input)
{
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

int leerPot()
{
  return (map(analogRead(34), 0, 4090, 0, 250));
}

void mostrarDec(int in)
{
  Serial.println(in);
}

void mostrarSalidaBin()
{
  for (int t = 0; t < 8; t++)
  {
    Serial.print(salidaBin[t]);
  }
  while (true)
    ;
}
