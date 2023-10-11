#define Rojo 5
#define Amarillo 17
#define Verde 16
#define Button 23

int factor = 1;
int tiempo = 20;
int buttonstate;

void IRAM_ATTR ISR(){
  Serial.println("Botón presionado");
  factor++;
}

void setup() {
  pinMode(Rojo, OUTPUT);
  pinMode(Amarillo, OUTPUT);
  pinMode(Verde, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(Button),ISR,RISING);
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
    
  Serial.println(factor*tiempo);
  
}
