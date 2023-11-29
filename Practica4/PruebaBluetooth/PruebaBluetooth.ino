#include "BluetoothSerial.h"
#include <NewPing.h>
#include <LiquidCrystal_I2C.h>

#if !defined(CONFIG_BT_ENABLED) || !defined( CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run 'make menuconfig' to and enable it
#endif

//Motor 1 - derecho. Motor 2 - izquierdo
//Viendo el robot por atrás
#define En1 14
#define M1A 27
#define M1B 26
#define M2A 25
#define M2B 33
#define En2 32

//SF1 es la derecha
#define SF1 34
#define SF2 35
#define SI
#define SIA
#define SDA
#define SD
#define SOT 13
#define SOE 12
#define SDA 21
#define SCL 22

//Librería de control del lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);
//Librería para sensado ultrasónico fácil
NewPing sonar(SOT,SOE,100);
//Librería de comunicación bluetooth
BluetoothSerial SerialBT;
char r;
//Arreglos para impresión en lcd 
char prt[16],str[16], pls[16];
//Variables de registro de sensado
int distancia;
int l1,l2;
//Variables de calibración de fotoresistencias
float maxL = 3700;
//Variables para control de motores
int res = 11, freq = 1000, canalIzq = 0, canalDer = 1;
float velIzq = 100, velDer = 100;
char cM;
bool continua = 1;

void setup() {
  //Prearación del lcd
  lcd.init();
  lcd.backlight();
  //Preparación de los motores
  pinMode(M1A,OUTPUT);
  pinMode(M1B,OUTPUT);
  pinMode(M2A,OUTPUT);
  pinMode(M2B,OUTPUT);
  ledcSetup(canalIzq, freq, res);
  ledcAttachPin(En2, canalIzq);
  ledcSetup(canalDer, freq, res);
  ledcAttachPin(En1, canalDer);
  //Preparación del bluetooth 
  SerialBT.begin("MothBot");
  Serial.begin(115200);
  //Motores deshabilitados
  digitalWrite(M1A,LOW);
  digitalWrite(M1B,LOW);
  digitalWrite(M2A,LOW);
  digitalWrite(M2B,LOW);
}

void loop() {
  sensado();
  if(SerialBT.available()){
    r = SerialBT.read();
    if(r =='1'){
      SerialBT.println("Rutina 1");
      rutina1();
    } 
    if(r =='2'){
      SerialBT.println("Rutina 2");
      
    } 
    if(r =='3'){
      SerialBT.println("Rutina 3");
      
    } 
  }
  delay(500);
}



//RUTINA 1
//Se mueve hacia adelante sin considerar obstáculos
//Determina la velocidad de los motores considerando 
//distancia, SF1 y SF2. Ajusta la velocidad para corregir
//desviaciones hacia la derecha o izquierda de la fuente de luz.
//Se detiene al encontrarse a 5cm de la fuente de luz
void rutina1(){
  //Parámetros encontrado a prueba y error
  //EDITABLES
  float velM = 0.5;
  int delaySensado = 700, delayGiro= 300, delayAvanza;
  int lAnterior = 0, l2Anterior = 0;
  lAnterior = l1;
  l2Anterior = l2;
  int velP;
  //Se mueve hacia adelante asumiendo que se coloca orientado hacia la fuente de luz
  while(distancia > 15 || distancia == 0){
    //Valor para controlar la velocidad de ambos motores
    //Mientras más cercano esté, menor velocidad para evitar chocar
    if (distancia == 0){
      velP = 100;
      delayAvanza = 500;
    } 
    else{
      velP = distancia * 0.4 + 10;
      delayAvanza = 200;
    }
    //Ajuste de desviaciones
    //Si se detecta un diferencial negativo en alguna dirección,
    //se ajusta la velocidad de algún motor para cambiar de dirección ligeramente
    if(lAnterior > l1) velIzq = 40; 
    else velIzq = 100;
    if(l2Anterior > l2) velDer = 40; 
    else velDer = 100;
    //Control de motores
    velIzq *= velP;
    velDer *= velP;
    controlMotor('w');
    delay(delayAvanza);
    controlMotor('0');
    delay(delaySensado);
    sensado();
  }
  controlMotor('0');
  centrarALuz(delaySensado,delayGiro,velM);
  sensado();
  if (l1 < 90) rutina1();
  SerialBT.println("Rutina 1 terminada!");
}


//Captura la información del sensor ultrasónico, y los fotoresistores
//Se almacena en distancia, l1 y l2
//Se despliega la información en el lcd
void sensado(){
  distancia = sonar.ping_cm();
  itoa(distancia, prt, 10);
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println(prt);
  lcd.print(prt);
  l1 = analogRead(SF1);
  l2 = analogRead(SF2);
  ajustesSF(&l1, &l2);
  lcd.setCursor(0, 1);
  itoa(l2, prt, 10);
  itoa(l1, str, 10);
  strcpy(pls, prt);
  strcat(pls,"-");
  strcat(pls, str);
  lcd.println(pls);
  //SerialBT.println(distancia);
}

void centrarALuz(int delaySensado, int delayGiro, float velP){
  int lAnterior = 0, contador = 0;
  velDer = 100;
  velIzq = 100;
  velDer *= velP;
  velIzq *= velP;
  //Gira a la derecha hasta que la luz empeora
  while(l1 >= lAnterior){
    lAnterior = l1;
    controlMotor('d');
    delay(delayGiro);
    controlMotor('0');
    delay(delaySensado);
    sensado();
  }
  //Gira a la izquierda hasta que la luz empeora
  lAnterior = 0;
  while(l1 >= lAnterior){
    lAnterior = l1;
    controlMotor('a');
    delay(delayGiro);
    controlMotor('0');
    delay(delaySensado);
    contador++;
    sensado();
  }
  //Gira a la derecha hasta estar centrado hacia la luz
  contador = contador/2;
  for(int i = 0; i < contador; i++){
    controlMotor('d');
    delay(delayGiro);
    controlMotor('0');
    delay(delaySensado);
  }
}

void ajustesSF(int *f1, int *f2){
  float a1, a2;
  int s1, s2;
  a1 = (*f1)*100.0/maxL;
  a2 = (*f2)*100.0/maxL;
  *f1 = (int)a1;
  *f2 = (int)a2;
}

//Recibe un caracter, control
//Switch sobre control, se ajusta el sentido de giro
//de los motores. Depende de las variables globales
//velIzq y VelDer para ajustar la velocidad de giro a través de pwm. 
void controlMotor(char control){
  switch(control){
    //Adelante
    case 'w':
      SerialBT.println("Vamos prro!!");
      digitalWrite(M1A,HIGH);
      digitalWrite(M1B,LOW);
      digitalWrite(M2A,HIGH);
      digitalWrite(M2B,LOW);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Izquierda
    case 'a':
      SerialBT.println("Izquierda prro!!");
      digitalWrite(M1A,HIGH);
      digitalWrite(M1B,LOW);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,HIGH);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Derecha
    case 'd':
      SerialBT.println("Derecha prro!!");
      digitalWrite(M1A,LOW);
      digitalWrite(M1B,HIGH);
      digitalWrite(M2A,HIGH);
      digitalWrite(M2B,LOW);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Atrás
    case 's':
      SerialBT.println("Atrás prro!!");
      digitalWrite(M1A,LOW);
      digitalWrite(M1B,HIGH);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,HIGH);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Detenido
    case '0':
      SerialBT.println("Quieto prro!!");
      ledcWrite(canalDer,0);
      ledcWrite(canalIzq,0);
      break;
    default:  
      break;
  }
}