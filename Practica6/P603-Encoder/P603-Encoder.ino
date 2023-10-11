#define EncA 19
#define EncB 18

long pulses = 0;

void IRAM_ATTR PulsesCounter(){
  if(digitalRead(EncB) == HIGH) pulses++;
  else pulses--;
}

void setup() {
  pinMode(EncA, INPUT);
  pinMode(EncB, INPUT);
  attachInterrupt(digitalPinToInterrupt(EncA),PulsesCounter,RISING);
  Serial.begin(115200);
}

void loop() {
  printValues();
  delay(100);
}


void printValues(){
  Serial.print("Pulsos: ");
  Serial.println(pulses);
}
