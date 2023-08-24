#define BOTON 2     //El push boton está conectado al pin 2
#define LED 15      //El LED del circuito está conectado al pin 15
bool estadoBTN;     //Variable que recive la lectura del boton
void setup() {
  pinMode(BOTON,INPUT); //Declaramos el boton como entrada
  pinMode(LED,OUTPUT);  //Declaramos el LED como salida
  //Serial.begin(115200); //Iniciamos la comunicación serial a 115200 baudios. Código de prueba
}

//Dependiendo del estado del botón se entra a un estado u otro
//Con el boton no presionado: se prende el LED, se mantiene medio segundo,
//se apaga el LED, espera medio segundo antes de la siguiente lectura de boton.
//Este proceso dura un segundo así que el LED parpadea a 1 Hz
//Con el boton no presionado: se prende el LED, se mantiene un octavo de segundo,
//se apaga el LED, espera un octavo de segundo antes de la siguiente lectura de boton.
//Este proceso dura un un cuarto de segundo así que el LED parpadea a 4 Hz
//Debido a la espera que hay entre lecturas del botón, el cambio de un estado a otro tiene
//un percibible retraso. Esto se puede arreglar en código permitiendo lecturas entre los 
//delays pero para efectos de la práctica el siguiente código parece satisfactorio. 
void loop() {
  estadoBTN = digitalRead(BOTON);       //Lee el estado del botón y lo almacena para elegir instrucciones
  if(estadoBTN == true){                //Elige el bloque de instrucciones correspondiente al estado
    //Serial.println("Presionado");       //Código de prueba.
    digitalWrite(LED,HIGH);             //Enciende el LED
    delay(125);                         //Espera 0.125 segundos
    digitalWrite(LED,LOW);              //Apaga el LED
    delay(125);                         //Espera 0.125 segundos
  }
  else{
    //Serial.println("No Presionado");    //Código de prueba
    digitalWrite(LED,HIGH);             //Enciende el LED
    delay(500);                         //Espera 0.5 segundos
    digitalWrite(LED,LOW);              //Apaga el LED
    delay(500);                         //Espera 0.5 segundos
  }
}
