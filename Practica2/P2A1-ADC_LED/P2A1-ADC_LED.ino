#define Pot 4
#define LED 15
int lectura;
float voltxBit = 3.3/4095;    //Factor de conversión volts por bit
float lecturaVolt;
void setup() {
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
}

void loop() {
  lectura = analogRead(Pot);    //Lectura del potenciometro
  //3.3V/4095 = Volts por bit leido
  // Bits leidos * (3.3/4095) = Volts leidos
  //El LED se enciende a partir del bit 2,482
  lecturaVolt = lectura*voltxBit; //Conversión de bits a volts. La alternativa es usar map()
  //Despliegue de datos 
  Serial.print("ADC = ");
  Serial.print(lectura);
  Serial.print(" bits ----- V = ");
  Serial.print(lecturaVolt);
  Serial.println(" Volts");
  //Se prende el LED si la lectura de voltaje es mayor a 2 
  if(lecturaVolt >2){
    digitalWrite(LED,HIGH);
  }
  else{
    digitalWrite(LED,LOW);
  }
  delay(300);
}
