// Primero que todo incluimos la libreria del LCD
#include <LiquidCrystal.h>
// incluimos la libreria del sensor DHT
#include "DHT.h"

// Declaramos la variables
int temperatura1;   //temperatura del colchon que se va a imprimir
int temperatura2;   //temperatura del ambiente que se va a imprimir
int sensorAmbiente; //Variable lectura sensor de ambiente
int sensorValRuido; //Variable lectura sensor de ruido
double ruido;       //ruido que se va a imprimir
double flujo;       //flujo que se va a imprimir
int humedad;        //Humedad que se va a imprimir
//puerto serial virtual
char tx[40]; //arreglo char de 40 caracteres
String Stemperatura1; 
String Stemperatura2; 
String Sruido; 
String Sflujo; 
String Shumedad; 

// Declaracion de pines
int T1 = A0;    //temperatura del colchol, Sensor PT100
int T2 = A1;    //temperatura del ambiente, Sensor 
int sensorRuido = A2;   //pin del sensor de ruido
int sensorFlujo = A3;   //pin del sensor de Flujo
int sensorHumedad = 2;  //pin del sensor de humedad
const int ledB = 9;   //Pin led ruido bajo
const int ledA = 8;   //Pin led ruido alto
// pines de la LCD
int rs=12;
int en=11;
int d4=6;
int d5=5;
int d6=4;
int d7=3;

// Inicializa la pantalla
LiquidCrystal lcd (rs, en, d4, d5, d6, d7); // Inicializa la pantalla
DHT dht(sensorHumedad, DHT11);   // Inicializamos la libreria DHT

// el simbolo de ° en bits para que la pantalla lo reconozca y para el dinosaurio
byte simGrados[8] = {B00110,B01001,B01001,B00110,B00000,B00000,B00000,B00000};
byte chardino1[8] = {B00000,B00000,B00000,B00000,B10000,B10000,B11000,B11100};
byte chardino2[8] = {B00000,B00001,B00001,B00001,B00001,B00011,B01111,B11111};
byte chardino3[8] = {B11111,B10111,B11111,B11111,B11100,B11111,B11100,B11100};
byte chardino4[8] = {B10000,B11000,B11000,B11000,B00000,B10000,B00000,B00000};
byte chardino5[8] = {B11111,B11111,B01111,B00111,B00011,B00011,B00010,B00011};
byte chardino6[8] = {B11111,B11111,B11111,B11111,B10110,B00010,B00010,B00011};
byte chardino7[8] = {B11111,B11001,B10000,B00000,B00000,B00000,B00000,B00000};

void setup () 
{
  Serial.begin(9600); //Comunicacion serial 9600 (bits por segundo)
  // Darle el tamaño al LCD 20x4
  lcd.begin (20, 4);
  dht.begin();
  
  // Declaramos pines de entrada
  pinMode(INPUT,T1);
  pinMode(INPUT,T2);
  pinMode(INPUT,sensorRuido);
  pinMode(INPUT,sensorFlujo);
  
  // Declaramos pines de salida
  pinMode(ledB,OUTPUT);
  pinMode(ledA,OUTPUT);

  // Configurar por codigo el ADC; es decir en ves de trabajar a 5V
  analogReference(EXTERNAL);

  //lipiamos la LCD
  lcd.clear();
  // imprimimos los dinos
  dino1();
  dino2();
  // imprimimos presentacion
  lcd.setCursor(5, 0);
  lcd.print("Analizador");
  lcd.setCursor(9, 1);
  lcd.print("de");
  lcd.setCursor( 5, 2);
  lcd.print("Incubadora");
  lcd.setCursor( 5, 3);
  lcd.print("GALLIMIMUS");
  // damos una espera para que el mensaje se quede en la pantalla 
  delay (100);
  //lipiamos la LCD
  lcd.clear();
}

int calcularTemperaturaColchon()
{
  // leemos el valor del sensor PT100
  int temperaturaPT100=analogRead(T1);
  // se convierte el valor del ADC al rango de trabajo
  temperaturaPT100=map(temperaturaPT100, 0,1023,0,50);
  // retornamos la temperatura calculada
  return temperaturaPT100;
}

int calcularTemperaturaAmbiente()
{
  // Leer Temperatura T2
  sensorAmbiente= analogRead(T2);
  int temperaturaAmbiente;
  // convertimos el valor leido en tension
  float voltaje =((sensorAmbiente/1023.0)*5);
  // convertimos la tension en temperatura
  temperaturaAmbiente=((voltaje-0.7)/0.04);
  // retornamos la temepratura del colchon
  return temperaturaAmbiente;
}

double calcularRuido()
{
  sensorValRuido = analogRead(sensorRuido);    //Lectura sensor
  
  if(sensorValRuido>0)
  {
    //Convertir los datos a dB
    long x=10*sensorValRuido;
    long y=100*x;
    double z=log10(y);
    double DB=10*z;

    //Si el ruido es menor a 50 dB encender led verde y apagar led rojo
    if(DB<50)
    {
      digitalWrite(ledB,HIGH);
      digitalWrite(ledA,LOW);
    }
    //Si el ruido es mayor o igual a 50 dB encender led rojo y apagar led verde
    if(DB>=50)
    {
      digitalWrite(ledB,LOW);
      digitalWrite(ledA,HIGH);
    }
    // retornamos el valor del ruido
    return DB;
  }
}

double calcularFlujo()
{
  // leemos el valor de flujo y lo convertimos el valor del ADC de 0 a 1023 en uno de 1 a 7
  double valFlujo=(analogRead(sensorFlujo))/146.28;
  // retornamos el valor del flujo
  return valFlujo;
}

int calcularHumedad()
{
  // leemos la humedad con ayuda de la libreria DHT
  int leerHumedad = dht.readHumidity();
  // retornamos la humedad
  return leerHumedad;
}

void dino1()
 {
  // creamos los simbolos para formar al dino
  lcd.createChar(1,chardino1);
  lcd.createChar(2,chardino2);
  lcd.createChar(3,chardino3);
  lcd.createChar(4,chardino4);
  lcd.createChar(5,chardino5);
  lcd.createChar(6,chardino6);
  lcd.createChar(7,chardino7);
  //  imprimimos el dino
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.setCursor(1,1);
  lcd.write(2);
  lcd.setCursor(2,1);
  lcd.write(3);
  lcd.setCursor(3,1);
  lcd.write(4);
  lcd.setCursor(0,2);
  lcd.write(5);
  lcd.setCursor(1,2);
  lcd.write(6);
  lcd.setCursor(2,2);
  lcd.write(7);
}

void dino2()
 {
  // creamos los simbolos para formar al dino
  lcd.createChar(1,chardino1);
  lcd.createChar(2,chardino2);
  lcd.createChar(3,chardino3);
  lcd.createChar(4,chardino4);
  lcd.createChar(5,chardino5);
  lcd.createChar(6,chardino6);
  lcd.createChar(7,chardino7);
  //  imprimimos el dino
  lcd.setCursor(16,1);
  lcd.write(1);
  lcd.setCursor(17,1);
  lcd.write(2);
  lcd.setCursor(18,1);
  lcd.write(3);
  lcd.setCursor(19,1);
  lcd.write(4);
  lcd.setCursor(16,2);
  lcd.write(5);
  lcd.setCursor(17,2);
  lcd.write(6);
  lcd.setCursor(18,2);
  lcd.write(7);
}


void loop() 
{
  // Comando para crear el simbolo ° para la pantalla que se llama 1, tiene que ser un numero entreto entre 1 y 8 
  lcd.createChar(1, simGrados);
  
  // Calcular Temperatura PT100
  temperatura1=calcularTemperaturaColchon();
  //Puerto serial virtual
  Serial.print(" T1: ");
  Serial.print(temperatura1);
  Serial.print( 1);
  Serial.print(" C");
  Serial.print('\r');
  
  // Calcular Temperatura Ambiente  
  temperatura2=calcularTemperaturaAmbiente();
  Serial.print(" T2: ");
  Serial.print(temperatura2);
  Serial.print( 1);
  Serial.print(" C");
  Serial.print('\r');

  // Calcula el nivel de Ruido en dB
  ruido=calcularRuido();
  //Puerto serial virtual
  Serial.print(" R: ");
  Serial.print(ruido);
  Serial.print(" dB");
  Serial.print('\r');
  
  // Calculo del Flujo en m/s
  flujo=calcularFlujo();
  flujo=flujo/10;
  Serial.print(" F: ");
  Serial.print(flujo);
  Serial.print(" m/s");
  Serial.print('\r');

  // Calculo de la Humedad
  humedad=calcularHumedad();
  Serial.print(" H: ");
  Serial.print(humedad);
  Serial.print(" %");
  Serial.print('\r');

  // Imprimimos Informacion
  lcd.setCursor(0, 0);
  lcd.print("Temperatura Colchon:");
  lcd.setCursor(8, 1);
  lcd.print(temperatura1);
  //imprimimos el simbolo de ° que se llama 1
  lcd.write(1);
  lcd.print("C");

  lcd.setCursor(0, 2);
  lcd.print("Temperatura Ambiente:");
  lcd.setCursor(8, 3);
  lcd.print(temperatura2);
  //imprimimos el simbolo de ° que se llama 1
  lcd.write(1);
  lcd.print("C");

  // damos una espera para mostrar las temperaturas 
  delay(200);
  //lipiamos la LCD
  lcd.clear();
  // imprimimos el ruido, flujo y la humedad
  lcd.setCursor(0,0);
  lcd.print("Intensidad: ");
  lcd.print(ruido);
  lcd.print(" dB");

  lcd.setCursor(0,1);
  lcd.print("Flujo: ");
  lcd.print(flujo);
  lcd.print(" m/s");

  lcd.setCursor(0, 2);
  lcd.print("Humedad: ");
  lcd.print(humedad);
  lcd.print(" %");
  // damos una espera para mostrar el ruido, flujo y la humedad
  delay(200);
  //lipiamos la LCD
  lcd.clear();
}
