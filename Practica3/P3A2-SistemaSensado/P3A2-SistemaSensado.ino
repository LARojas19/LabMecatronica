#define echoPin 15
#define trigPin 2

int duracion;
int distancia;
int velSonido = 0.0343; //cm/microsegundo
void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(115200);
}

void loop() {
  //Ultrasonico(); crear funcion
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  
  duracion = pulseIn(echoPin,HIGH);
  //Serial.println(duracion);
  distancia = velSonido * duracion * 0.5;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println("cm");
}
