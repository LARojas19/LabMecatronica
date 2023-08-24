#define LED 15  //El LED del circuito está conectado la pin 15
#define LED2 2  //El LED integrado está conectado al pin 2. 
//No es requerido por la práctica pero lo usamos de manera análoga al del circuito.
void setup() {
  //Declaramos ambos pines como salidas
  pinMode(LED,OUTPUT);
  pinMode(LED2,OUTPUT);
}

void loop() {
  digitalWrite(LED,HIGH);   //Manda una señal de prender el LED
  digitalWrite(LED2,LOW );  //Manda una señal de apagar el LED integrado
  delay(1000);              //Retardar tiempo 1 segundo
  digitalWrite(LED,LOW);    //Manda una señal de apagar el LED
  digitalWrite(LED2,HIGH);  //Manda una señal de prender el LED integrado
  delay(1000);              //Retardar tiempo 1 segundo
}
