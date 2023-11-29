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
#define SI 17
#define SIA 5
#define SDA 23 
#define SD 19
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
bool obs[4];
//Variables de calibración de fotoresistencias
float maxL = 4070;
//Variables para control de motores
int res = 11, freq = 1000, canalIzq = 0, canalDer = 1;
float velIzq = 100, velDer = 100;
char CM;
bool continua = 1;
//Variables de prueba
int delayPruebas;
//Delays de calibración de giro
//El delay necesario para girar 90 grados en la dirección respectiva
//con la velocidad de los motres a 40%
int dqIzq = 2250, dqDer = 2000;

// void IRAM_ATTR obstaculoEnfrente(){
//   continua = 0;
//   sensado();
// }

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
  //Preparación de sensores infrarojos
  pinMode(SI,INPUT);
  pinMode(SIA,INPUT);
  pinMode(SD,INPUT);
  pinMode(SDA,INPUT);
  //Las interrupciones generaban un error y las tuvimos que omitir
  //attachInterrupt(digitalPinToInterrupt(SIA),obstaculoEnfrente,RISING);
  //attachInterrupt(digitalPinToInterrupt(SDA),obstaculoEnfrente,RISING);

}


//lol
void loop() {
  sensado();
  if(SerialBT.available()){
    recibeRutina();
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
  float velG = 0.40;
  int delaySensado = 400, delayAvanza;
  int lAnterior = 0, l2Anterior = 0;
  lAnterior = l1;
  l2Anterior = l2;
  int velP, distAnterior;
  distAnterior = 15;
  //Se mueve hacia adelante asumiendo que se coloca orientado hacia la fuente de luz
  while(distancia > 15 || distancia == 0){
    //Ajuste de desviaciones
    //Si se detecta un diferencial negativo en alguna dirección se vuelve a determinar la dirección
    if(lAnterior > l1 || l2Anterior < 1 || (distAnterior < distancia && distAnterior != 0))  centrarALuz(delaySensado,velG); 
    //Valor para controlar la velocidad de ambos motores
    //Mientras más cercano esté, menor velocidad para evitar chocar
    //Si se lee una distancia de 0, quiere decir que no hay obstáculos a menos de 1m (o que ya chocamos)
    if (distancia == 0){
      velP = 100;
      delayAvanza = 2000;
    } 
    else{
      velP = distancia * 0.75 + 25;
      delayAvanza = distancia*20;
    }
    //Control de motores
    velIzq = velP;
    velDer = 0.97*velP;
    controlMotor('w');
    delay(delayAvanza);
    controlMotor('0');
    delay(delaySensado);
    lAnterior = l1;
    l2Anterior = l2;
    distAnterior = distancia;
    sensado();
  }
  controlMotor('0');
  centrarALuz(delaySensado,velG);
  sensado();
  if (l1 < 90 && distancia > 15) rutina1();
}

void mothBot(){
  SerialBT.println("MothBot");
  //Parámetros encontrado a prueba y error
  //EDITABLES
  float velG = 0.40;
  int delaySensado = 400, delayAvanza;
  int lAnterior = 0, l2Anterior = 0;
  lAnterior = l1;
  l2Anterior = l2;
  int velP, distAnterior, distMin;
  char ladoMin;
  distAnterior = 15;
  centrarALuz(delaySensado,velG);
  sensado();  
  while((distancia > 15 || distancia == 0) && l1 < 95){
    buscaObstaculos(&distMin,&ladoMin);  
    sensado();  
    if(distMin < 15){
      centraObstaculo(distMin, ladoMin);
      if(ladoMin == 'a' && l1 < 90) flanqueaDer();
      if(ladoMin == 'd' && l1 < 90) flanqueaIzq();
    }
    else{
      if (distancia == 0){
        velP = 100;
        delayAvanza = 2000;
      } 
      else{
        velP = distancia * 0.75 + 25;
        delayAvanza = distancia*20;      
      }
      //Control de motores
      velIzq = velP;
      velDer = 0.97*velP;
      controlMotor('w');
      delay(delayAvanza);
      controlMotor('0');
      delay(delaySensado);
    }
    centrarALuz(delaySensado,velG);
    sensado();
  }
  
}

void buscaObstaculos(int *dM, char* lM){

  SerialBT.println("buscaObstaculo");
  int distMin = 100, t;
  char ladoMin;
  //cuarto de vuelta a la derecha
  velDer = 40;
  velIzq = 40;
  controlMotor('d');
  delay(dqDer);
  controlMotor('0');
  delay(200);
  //Barrido hacia la izquierda
  t = micros() + dqIzq*800;
  controlMotor('a');
  while(t > micros()){
    sensado();
    if (distancia != 0){
      if (distancia < distMin){
        distMin = distancia;
        ladoMin = 'd';
      }
    }
  }
  t = micros() + dqIzq*800;
  while(t > micros()){
    sensado();
    if (distancia != 0){
      if (distancia < distMin){
        distMin = distancia;
        ladoMin = 'a';
      }
    }
  }
  controlMotor('0');
  delay(200);
  //Regreso a inicio
  controlMotor('d');
  delay(dqDer);
  controlMotor('0');
  *dM = distMin;
  *lM = ladoMin; 
}

void centraObstaculo(int dM, char lM){
  SerialBT.println("centraObstaculo");
  sensado();
  velDer = 40;
  velIzq = 40;
  if(lM == 'd'){
    controlMotor('d');
    while(distancia > dM + 3){
      sensado();
    }
    controlMotor('0');
  }
  else{
    controlMotor('a');
    while(distancia > dM + 3){
      sensado();
    }
    controlMotor('0');
  }
  delay(200);
}

//El bot se acerca a un objetivo en frente de el hasta estar a 5cm,
//gira hacia la izquierda o derecha y modula la velocidad de los motores
//para crear un efecto de zigzag a partir de los sensores laterales.
//El objetivo es moverse al lado de un objeto o muro, flanqueandolo sin chocar contra el. 
void flanqueaDer(){
  int t;
  bool f;
  //Acercamiento al obstáculo
  velDer = 35;
  velIzq = 35;
  controlMotor('w');
  while(distancia > 5){
    delay(50);
    sensado();
  }
  controlMotor('0');
  delay(100);
  controlMotor('d');
  delay(dqDer);
  controlMotor('0');
  sensado();
  f = distancia > 15 || distancia == 0;
  while(f){
    t = micros() + 5000000;
    while(obs[1] && obs[0] && t > micros() && (distancia > 15 || distancia == 0)){
      velDer = 45;
      velIzq = 40;
      controlMotor('w');
      sensado();
    }
    f = t > micros();
    while(!obs[0] && f && (distancia > 15 || distancia == 0)){
      velDer = 40;
      velIzq = 45;
      controlMotor('w');
      sensado();
    }
    controlMotor('0');
    sensado();
    f = f && (distancia > 15 || distancia == 0);
  }
}
void flanqueaIzq(){
  int t;
  bool f;
  //Acercamiento al obstáculo
  velDer = 35;
  velIzq = 35;
  controlMotor('w');
  while(distancia > 5){
    delay(50);
    sensado();
  }
  controlMotor('0');
  delay(100);
  controlMotor('a');
  delay(dqIzq);
  controlMotor('0');
  sensado();
  f = distancia > 15 || distancia == 0;
  while(f){
    t = micros() + 5000000;
    while(obs[2] && obs[3] && t > micros() && (distancia > 15 || distancia == 0)){
      velDer = 40;
      velIzq = 45;
      controlMotor('w');
      sensado();
    }
    f = t > micros();
    while(!obs[3] && f && (distancia > 15 || distancia == 0)){
      velDer = 45;
      velIzq = 40;
      controlMotor('w');
      sensado();
    }
    controlMotor('0');
    sensado();
    f = f && (distancia > 15 || distancia == 0);
  }
}

//Captura la información del sensor ultrasónico, y los fotoresistores
//Se almacena en distancia, l1 y l2
//Se despliega la información en el lcd3++
void sensado(){
  distancia = sonar.ping_cm();
  itoa(distancia, prt, 10);
  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println(prt);
  lcd.print(prt);
  l1 = analogRead(SF1);
  l2 = analogRead(SF2);
  obs[0] = digitalRead(SI);
  obs[1] = digitalRead(SIA);
  obs[2] = digitalRead(SDA);
  obs[3] = digitalRead(SD);
  ajustesSF(&l1, &l2);
  lcd.setCursor(0, 1);
  itoa(l1, prt, 10);
  itoa(l2, str, 10);
  strcpy(pls, prt);
  strcat(pls,"-");
  strcat(pls, str);
  lcd.println(pls);
  //SerialBT.println(String(obs[0]) + " " + String(obs[1])+ " " + String(obs[2])+ " " + String(obs[3]));
}

//Método para centrar el bot hacia la fuente de luz
//Voltea en ráfagas de misma duración hacia la derecha e izquierda para sensar
//El bot termina orientado hacia la mitad del umbral de máxima luminosidad 
void centrarALuz(int delaySensado, float velP){
  int lAnterior = 0, contador = 0;
  velDer = 100;
  velIzq = 100;
  velDer *= velP;
  velIzq *= velP;
  //Gira a la derecha hasta que la luz empeora
  while(l1 >= lAnterior){
    lAnterior = l1;
    controlMotor('d');
    delay(dqDer/3);
    controlMotor('0');
    delay(delaySensado);
    sensado();
  }
  //Gira a la izquierda hasta que la luz empeora
  lAnterior = 0;
  while(l1 >= lAnterior){
    lAnterior = l1;
    controlMotor('a');
    delay(dqIzq/3);
    controlMotor('0');
    delay(delaySensado);
    contador++;
    sensado();
  }
  //Gira a la derecha hasta estar centrado hacia la luz
  contador = (contador + 1)/2;
  for(int i = 0; i < contador; i++){
    controlMotor('d');
    delay(dqDer/3);
    controlMotor('0');
    delay(delaySensado);
  }
}

//Override con delayGiro como parámetro
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


//Función de normalización de valores leidos en fotoresistencias
void ajustesSF(int *f1, int *f2){
  float a1, a2;
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
      digitalWrite(M1A,HIGH);
      digitalWrite(M1B,LOW);
      digitalWrite(M2A,HIGH);
      digitalWrite(M2B,LOW);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Izquierda
    case 'a':
      //SerialBT.println("Izquierda prro!!");
      digitalWrite(M1A,HIGH);
      digitalWrite(M1B,LOW);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,HIGH);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Derecha
    case 'd':
      //SerialBT.println("Derecha prro!!");
      digitalWrite(M1A,LOW);
      digitalWrite(M1B,HIGH);
      digitalWrite(M2A,HIGH);
      digitalWrite(M2B,LOW);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Atrás
    case 's':
      //SerialBT.println("Atrás prro!!");
      digitalWrite(M1A,LOW);
      digitalWrite(M1B,HIGH);
      digitalWrite(M2A,LOW);
      digitalWrite(M2B,HIGH);
      ledcWrite(canalDer,int(2023*velDer/100));
      ledcWrite(canalIzq,int(2023*velIzq/100));
      break;
    //Detenido
    case '0':
      //SerialBT.println("Quieto prro!!");
      ledcWrite(canalDer,0);
      ledcWrite(canalIzq,0);
      break;
    default:  
      break;
  }
}

//Control del sistema via bluetooth 
void recibeRutina(){
  int dM = 100;
  char lM;
  r = SerialBT.read();
  switch(r){
    case '1':
      SerialBT.println("Rutina 1");
      rutina1();
      SerialBT.println("Rutina Terminada");
      break;
    case 'p':
      pruebas();
      break;
    case '0':
      while(true){
        controlMotor('0');
        SerialBT.println('help!');
        delay(1000);
      }
      break;
    case 'q':
      delayPruebas = recibeNumero();
      break;
    case 'i':
      velIzq = recibeNumero();
      break;
    case 'd':
      velDer = recibeNumero();
      break;
    case 'c':
      CM = SerialBT.read();
      controlMotor(CM);
      break;
    case 'f':
      r = SerialBT.read();
      if (r == 'd') flanqueaDer();
      if( r == 'a') flanqueaIzq();
      break;
    case 'm':
      mothBot();
      break;
    case 's':
      buscaObstaculos(&dM,&lM);
      SerialBT.println(String(dM) + " - " + String(lM));
      break;
    default:
      break;
  } 
}


void pruebas(){
  //SerialBT.println("CM: " + String(CM) + " - Delay: " + String(delayPruebas) + " - VelDer: " + String(velDer)+ " - VelIzq: " + String(velIzq));
  controlMotor(CM);
}

//Funcion tomada de internet
//Limpia el buffer de comunicación BT, regresa un entero si lo lee
int recibeNumero(){
  SerialBT.println("Adentro");
  char input[32] = "" ;
  int input_index = 0 , k;
  while (SerialBT.available())
  {
      char c = SerialBT.read() ;
      if( c != '\r' && c != '\n' )
      {
          input[input_index] = c ;
          input_index = (input_index + 1) % (sizeof(input) - 1) ;
      }
      else if( input_index > 0 )
      {
          k = atoi( input ) ;
          SerialBT.println( k ) ;
          input_index = 0 ;
      }

      input[input_index] = '\0' ;         
  }
  return k;
}