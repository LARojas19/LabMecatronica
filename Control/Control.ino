#define A 0
void setup() {
  pinMode(A,OUTPUT);
}

void loop() {
  digitalWrite(A,HIGH);
  delay(3000);
  digitalWrite(A,LOW);
  delay(3000);
}
