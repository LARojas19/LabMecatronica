#define EnM 13
#define MA 12
#define MB 14

int channel = 0;
int freq = 1000;
int resolution = 12;


void setup() {
  pinMode(EnM,OUTPUT);
  pinMode(MA,OUTPUT);
  pinMode(MB,OUTPUT);
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(EnM, channel);
}

void loop() {
  // Sentido horario - dextrógiro");
  Serial.println("Dextrógiro");
  ledcWrite(channel, 3000);
  digitalWrite(MA,HIGH);
  digitalWrite(MB,LOW);
  delay(2000);

  //Detenido
  ledcWrite(channel, 0);
  digitalWrite(MA,HIGH);
  digitalWrite(MB,LOW);
  delay(2000);

  //Sentido antihorario - levógico
  Serial.println("Levógiro");
  ledcWrite(channel, 1000);
  digitalWrite(MA,LOW);
  digitalWrite(MB,HIGH);
  delay(2000);


  ledcWrite(channel, 0);
  digitalWrite(MA,LOW);
  digitalWrite(MB,LOW);
  delay(2000);

}
