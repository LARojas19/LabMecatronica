#define led1 4
#define led2 16
#define led3 17
#define but1 22
#define but2 23

int factor = 1;
int tiempo = 20;


void IRAM_ATTR incrementaFactor(){
  factor++;
}
void IRAM_ATTR decrementaFactor(){
  if(factor > 1) factor--;
}

void setup() {
  pinMode(but1, INPUT);
  pinMode(but2, INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(but1),incrementaFactor,RISING);
  attachInterrupt(digitalPinToInterrupt(but2),decrementaFactor,RISING);
  Serial.begin(115200);
  digitalWrite(led1,LOW);
  digitalWrite(led2,LOW);
  digitalWrite(led3,LOW);
}

void loop() {
  digitalWrite(led3,LOW);
  digitalWrite(led1,HIGH);
  imprimeFactor();
  delay(factor*tiempo);
  digitalWrite(led1,LOW);
  digitalWrite(led2,HIGH);
  imprimeFactor();
  delay(factor*tiempo);
  digitalWrite(led2,LOW);
  digitalWrite(led3,HIGH);
  imprimeFactor();
  delay(factor*tiempo);
}

                                           
void imprimeFactor(){
  Serial.print("Factor de Tiempo: ");
  Serial.println(factor);
}