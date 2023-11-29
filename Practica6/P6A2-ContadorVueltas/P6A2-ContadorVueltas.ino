#define EnM 21
#define MA 23
#define MB 22
#define EncA 19
#define EncB 18
//64 pulsos por revolucion
long pulsos = 0;
long tiempo;
long tiempoPrint;
long tiempoActual;
double vueltas;
void IRAM_ATTR PulsesCounter(){
  if(digitalRead(EncB) == HIGH) pulsos++;
  else pulsos--;
}

void setup() {
  pinMode(EnM,OUTPUT);
  pinMode(MA,OUTPUT);
  pinMode(MB,OUTPUT);
  pinMode(EncA, INPUT);
  pinMode(EncB, INPUT);
  attachInterrupt(digitalPinToInterrupt(EncA),PulsesCounter,RISING);
  Serial.begin(115200);
}

void loop() {
  dextrogiro();
  Serial.println("Detenido");
  delay(2000);
  levogiro();
  Serial.println("Detenido");
  delay(2000);
}

void dextrogiro(){
  pulsos = 0;
  digitalWrite(MA,HIGH);
  digitalWrite(MB,LOW);
  digitalWrite(EnM,HIGH);
  tiempoPrint =micros();
  tiempo = tiempoPrint+3000000;
  tiempoActual = tiempoPrint;
  while(tiempo - tiempoActual > 0){
    if(tiempoActual - tiempoPrint > 300000){
      tiempoPrint = micros();
      Serial.print("Dextrógiro --- Cantidad de Vueltas: ");
      vueltas = 4.0*pulsos/(64.0*30);
      Serial.println(vueltas);
    }
    tiempoActual = micros();
  }
  digitalWrite(EnM,LOW);
  Serial.print("Dextrógiro --- Cantidad de Vueltas: ");
  vueltas = 4.0*pulsos/(64.0*30);
  Serial.println(vueltas);
}
void levogiro(){
  pulsos = 0;
  digitalWrite(MA,LOW);
  digitalWrite(MB,HIGH);
  digitalWrite(EnM,HIGH);
  tiempoPrint = micros();
  tiempoActual = tiempoPrint;
  tiempo = tiempoPrint + 3000000;
  while(tiempo - tiempoActual > 0){
    if(tiempoActual - tiempoPrint > 300000){
      tiempoPrint = micros();
      Serial.print("Levógiro --- Cantidad de Vueltas: ");
      vueltas = 4.0*pulsos/(64.0*30);
      Serial.println(vueltas);
    }
    tiempoActual = micros();
  }
  digitalWrite(EnM,LOW);
  Serial.print("Levógiro --- Cantidad de Vueltas: ");
  vueltas = 4.0*pulsos/(64.0*30);
  Serial.println(vueltas);
}
