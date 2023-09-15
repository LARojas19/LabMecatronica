#include <NewPing.h>
//Definicion de los pines de los LEDs
#define v1 23
#define v2 22
#define a1 21
#define a2 19
#define r1 18
#define r2 5
#define obstaculoPin 36
#define trigPin 32
#define echoPin 34
#define fotoresistor 14

NewPing sonar(trigPin,echoPin,100);
//int duracion;
int distancia;
//int velSonido = 0.0343; //cm/microsegundo

bool obstaculo;
bool temporizador;

bool d1;
bool d2;
bool d3;
bool d4;
bool d5;
bool d6;


int luminosidad;


int lecturaTouch;
bool bandera1;
bool bandera2;
bool bandera3;
bool error;


void setup() {
  pinMode(v1, OUTPUT);
  pinMode(v2, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(obstaculoPin, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  lecturaTouch = touchRead(T0);
  bandera1 = lecturaTouch < 30;
  lecturaTouch = touchRead(T4);
  bandera2 = lecturaTouch < 30;
  lecturaTouch = touchRead(T3);
  bandera3 = lecturaTouch < 30;
  errorTouch();
  setZeros();
  if(bandera1) detectaObstaculo();
  if(bandera2) ultrasonico();
  if(bandera3) detectaLuminosidad();
  //Imprime resultados de rutinas
  imprime();
  delay(300);
}


void setZeros(){
  obstaculo = 0;
  d1 = 0;
  d2 = 0;
  d3 = 0;
  d4 = 0;
  d5 = 0;
  d6 = 0;
  luminosidad = 0;
  apagaLuces();
}

void imprime(){
  Serial.print("OBST: ");
  Serial.print(obstaculo);
  Serial.print(" -- DIST: ");
  Serial.print(d1);
  Serial.print(d2);
  Serial.print(d3);
  Serial.print(d4);
  Serial.print(d5);
  Serial.print(d6);
  Serial.print(" -- LUM: ");
  Serial.print(luminosidad);
  Serial.println("%");
}


void errorTouch(){
  error = (bandera1&&bandera2)||(bandera2&&bandera3)||(bandera1&&bandera3);
  if (error){
    bandera1 = false;
    bandera2 = false;
    bandera3 = false;
  }
}

void detectaLuminosidad(){
  luminosidad = analogRead(fotoresistor);
  //Min 0 - Max 4095
}

void detectaObstaculo(){
  temporizador = 0; 
  while(bandera1){
    obstaculo = not(digitalRead(obstaculoPin));
    imprime();
    if(obstaculo && temporizador){
      prendeLuces();
    }
    else{
      apagaLuces();
    }
    delay(100);
    temporizador = !temporizador;
    lecturaTouch = touchRead(T0);
    bandera1 = lecturaTouch < 30;
  }
}


void ultrasonico(){
  //Se reemplazo gran parte del código gracias a la librería
  //digitalWrite(trigPin,LOW);
  //delayMicroseconds(2);
  //digitalWrite(trigPin,HIGH);
  //delayMicroseconds(10);
  //digitalWrite(trigPin,LOW);
  //duracion = pulseIn(echoPin,HIGH);
  //Serial.println(duracion);
  //distancia = velSonido * duracion * 0.5;
  distancia = sonar.ping_cm();
  if(distancia > 30){
    digitalWrite(v1,HIGH);
    d1 = 1;
  }
  if(distancia > 25){
    digitalWrite(v2,HIGH);
    d2 = 1;
  }
  if(distancia > 20){
    digitalWrite(a1,HIGH);
    d3 = 1;
  }
  if(distancia > 15){
    digitalWrite(a2,HIGH);
    d4 = 1;
  }
  if(distancia > 10){
    digitalWrite(r1,HIGH);
    d5 = 1;
  }
  if(distancia > 5){
    digitalWrite(r2,HIGH);
    d6 = 1;
  }
}


void prendeLuces(){
  digitalWrite(v1,HIGH);
  digitalWrite(v2,HIGH);
  digitalWrite(a1,HIGH);
  digitalWrite(a2,HIGH);
  digitalWrite(r1,HIGH);
  digitalWrite(r2,HIGH);
}
void apagaLuces(){
  digitalWrite(v1,LOW);
  digitalWrite(v2,LOW);
  digitalWrite(a1,LOW);
  digitalWrite(a2,LOW);
  digitalWrite(r1,LOW);
  digitalWrite(r2,LOW);
}
