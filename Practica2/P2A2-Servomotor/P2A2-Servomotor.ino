#include <ESP32_Servo.h> //Importamos la libreria para utilizar el servomotor
#define Pot 4         //Declaramos el pin de entrada del potenciomerto
#define servoPin 3    //Decaramos el pin de salida del servomotor

int lectura;
int angulo;
Servo miservo;  //Ojeto Servo sirve para controlar el servomotor
void setup() {
  miservo.attach(servoPin); //Se le adjunta el pin del servomotor al objeto Servo 
  Serial.begin(115200);     
}


void loop() {
  lectura = analogRead(Pot);  //Leemos el voltaje en el potenciometro
  angulo = map(lectura,0,4095,180,0); //Mapeamos la lectura a el ángulo elegido
  miservo.write(angulo);  //Se sobreescribe el ángulo del servomotor 
  Serial.print("ADC = ");
  Serial.print(lectura);
  Serial.print(" BITS ----- THETA = ");
  Serial.print(angulo);
  Serial.println("°");
  delay(300);
}
