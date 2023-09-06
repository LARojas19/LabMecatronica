//1 Hess= 9.81m/s2 medida de aceleración
#include <ESP32_Servo.h>
#define X 14
#define Y 12
#define Z 13
#define servoPin 23
#define TWO_PI 6.2831853 // Almacenamos para convertir de radianes a grados
float valX;
float valY;
float valZ;
float roll, pitch;
int angulo;
Servo miServo;  //Declaramos el objeto Servo
void setup() {
  miServo.attach(servoPin); //Adjuntamos el pin de control del servomotor al objeto Serbo
  Serial.begin(115200);
}
//Cuando no hay aceleración, la lectura es de alrededor de 2000
void loop() {
  //Se reciven los datos del acelerómetro
  valX = analogRead(X);
  valY = analogRead(Y);
  valZ = analogRead(Z);
  //Se convierten los datos a valores entre -1.0 y 1.0
  //Por los errores de lectura, se toma un promedio de los valores mínimos y máximos para mapear
  //Estos valores fueron modificados "al tanteo" para mejorar los resultados desplegados en consola
  valX = map(valX,1550,2377,100,-100)/100.0;
  valY = map(valY,1540,2332,-100,100)/100.0;
  valZ = map(valZ,1550,2355,-100,100)/100.0;
  //Se calculan los ángulos de rotación
  roll =valY/sqrt(sq(valX)+sq(valZ));
  pitch =valX/sqrt(sq(valY)+sq(valZ));
  roll = atan(roll);
  pitch = atan(pitch);
  //Se hace la conversión de radianes a grados
  roll = -1*roll*360/TWO_PI;
  pitch = pitch*360/TWO_PI;
  //Mapeamos roll a ángulos para el servomotor
  angulo = map(roll,-90,90,0,180);
  //Topamos los valores del servomotor en caso de exceder sus limites
  if(angulo > 180){
    angulo = 180;}
  if(angulo < 0){
    angulo = 0;}
  //Actualizamos el ángulo del servomotor
  miServo.write(angulo);
  
  //Desplegamos los datos
  Serial.print("X: ");
  Serial.print(valX);
  //Serial.print(","); //Para imprimir varias variables en el serial plotter debemos imprimir una coma entre cada una
  Serial.print(" Y: ");
  Serial.print(valY);
  //Serial.print(",");
  Serial.print(" Z: ");
  Serial.println(valZ);
  Serial.print("Roll = ");
  Serial.print(roll);
  Serial.print("° --- Pitch = ");
  Serial.print(pitch);
  Serial.println("°");
  delay(300);
  
}
