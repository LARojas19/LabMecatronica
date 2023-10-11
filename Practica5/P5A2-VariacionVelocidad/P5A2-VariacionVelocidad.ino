#define EnM 13
#define MB 12
#define MA 14
#define Pot 4

int channel = 1;
int freq = 1000;
int resolution = 12;
int lecturaPot;
float lectPotVolt;
String giro;
int velocidad;
float conversion;

void setup() {
  pinMode(EnM,OUTPUT);
  pinMode(MA,OUTPUT);
  pinMode(MB,OUTPUT);
  //pinMode(Pot, INPUT);
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(EnM, channel);
}

void loop() {
  //Sentido horario - dextrógiro");
  lecturaPot = 4095 - analogRead(Pot);
  //Serial.println(lecturaPot);
  
  //Default-Detenido
  ledcWrite(channel,0);
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  giro = "Detenido";
  velocidad = 0;
  if(lecturaPot*3.3/4095 < 1.32){
    digitalWrite(MA,LOW);
    digitalWrite(MB,HIGH);
    conversion = 4095*(1638 - lecturaPot)/1638;
    ledcWrite(channel,conversion);
    velocidad = conversion*100/4095;
    giro = "Levógiro";
  }
  if(lecturaPot*3.3/4095 > 1.98){
    digitalWrite(MA,HIGH);
    digitalWrite(MB,LOW);
    conversion = 4095*(lecturaPot - 2457)/1638;
    ledcWrite(channel,conversion);
    velocidad = conversion*100/4095;
    giro = "Dextrógiro";
  }
  lectPotVolt = lecturaPot*3.3/4095;
  Serial.print("Voltaje: ");
  Serial.print(lectPotVolt);
  Serial.print(" v --- "+giro+" --- ");
  Serial.print(velocidad);
  Serial.println("%");
  
  delay(400);
}
