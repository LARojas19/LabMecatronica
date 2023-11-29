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
int res = 11;
int freq = 1000;
int channel = 0; 
float anguloActual;
float anguloAnterior;
float rpm, rps, rads;

void IRAM_ATTR PulsesCounter(){
  if(digitalRead(EncB) == HIGH) pulsos++;
  else pulsos--;
}

void setup() {
  //pinMode(EnM,OUTPUT);
  ledcSetup(channel, freq, res);
  ledcAttachPin(EnM, channel);
  pinMode(MA,OUTPUT);
  pinMode(MB,OUTPUT);
  pinMode(EncA, INPUT);
  pinMode(EncB, INPUT);
  attachInterrupt(digitalPinToInterrupt(EncA),PulsesCounter,RISING);
  Serial.begin(115200);
}

void loop() {
  levogiroControlado(0.1);
  levogiroControlado(0.3);
  levogiroControlado(0.5);
  levogiroControlado(1.0);
  levogiroControlado(0.0);
}


void levogiroControlado(float porcentaje){
  pulsos = 0;
  digitalWrite(MA,LOW);
  digitalWrite(MB,HIGH);
  Serial.println(porcentaje);
  Serial.println(int(2023*porcentaje));
  ledcWrite(channel,int(2023*porcentaje));
  tiempoPrint = micros();
  tiempoActual = tiempoPrint;
  tiempo = tiempoPrint + 3000000;
  while(tiempo - tiempoActual > 0){
    if(tiempoActual - tiempoPrint > 100000){
      tiempoPrint = micros();
      Serial.print("Pulsos: ");
      Serial.print(pulsos);
      Serial.print(" --- Ángulo: ");
      anguloActual = (pulsos*4.0*360.0)/(64.0*30);
      Serial.print(anguloActual);
      rads = 1000*(anguloActual-anguloAnterior)/100
      Serial.print("° --- W: ");
      Serial.print(pulsos);
      Serial.print("[rps] --- ");
      Serial.print(pulsos);
      Serial.print("[rpm] --- ");
      Serial.print(pulsos);
      Serial.print("[rad/s]"); 
      anguloAnterior = anguloActual;     
    }
    tiempoActual = micros();
  }
  //Perdemos tiempo para eliminar la inercia entre cada cambio de velocidad
  ledcWrite(channel,0);
  delay(500);
}
