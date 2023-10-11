#define EnM 13
#define MB 12
#define MA 14
#define Pot 4

int lecturaPot;
float lectPotVolt;
String giro;

void setup() {
  pinMode(EnM,OUTPUT);
  pinMode(MA,OUTPUT);
  pinMode(MB,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  lecturaPot = 4095 - analogRead(Pot);
  digitalWrite(EnM,HIGH);
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  giro = "Detenido";
  if(lecturaPot*3.3/4095 < 1.32){
    digitalWrite(MA,LOW);
    digitalWrite(MB,HIGH);
    giro = "Levógiro";
  }
  if(lecturaPot*3.3/4095 > 1.98){
    digitalWrite(MA,HIGH);
    digitalWrite(MB,LOW);
    giro = "Dextrógiro";
  }
  lectPotVolt = lecturaPot*3.3/4095;
  Serial.print("Voltaje: ");
  Serial.print(lectPotVolt);
  Serial.println(" v --- "+giro);
  
  delay(400);
}
