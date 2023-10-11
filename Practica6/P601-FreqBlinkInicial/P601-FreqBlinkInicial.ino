#define Rojo 17
#define Amarillo 16
#define Verde 4
#define ButLed 2
#define Button 23

int factor = 1;
int tiempo = 20;
int buttonstate;

void setup() {
  pinMode(Rojo, OUTPUT);
  pinMode(Amarillo, OUTPUT);
  pinMode(Verde, OUTPUT);
  pinMode(ButLed, OUTPUT);
  pinMode(Button, INPUT);

  Serial.begin(115200);
}

void loop() {
  digitalWrite(Rojo, LOW);
  digitalWrite(Verde, HIGH);
  delay(factor*tiempo);
  digitalWrite(Verde, LOW);
  digitalWrite(Amarillo, HIGH);
  delay(factor*tiempo);
  digitalWrite(Amarillo, LOW);
  digitalWrite(Rojo, HIGH);
  delay(factor*tiempo);
  
  buttonstate = digitalRead(Button);

  if (buttonstate){
    Serial.println("Botón presionado");
    factor++;
    digitalWrite(ButLed,HIGH);
    delay(tiempo);
    digitalWrite(ButLed,LOW);
    delay(tiempo);
  }
  
  Serial.println(factor*tiempo);
  

  
  
}
