#define VERDE1 15     //El LED verde del primer semaforo está en el pin 15
#define AMARILLO1 2   //El LED amarillo del primer semaforo está en el pin 2
#define ROJO1 4       //El LED rojo del primer semaforo está en el pin 4
#define VERDE2 18     //El LED verde del segundo semaforo está en el pin 18
#define AMARILLO2 19  //El LED amarillo del segundo semaforo está en el pin 19
#define ROJO2 21      //El LED rojo del segundo semaforo está en el pin 21
void setup() {
  //Declararamos todos los pines como salidas
  pinMode(VERDE1,OUTPUT);          
  pinMode(AMARILLO1,OUTPUT);
  pinMode(ROJO1,OUTPUT);
  pinMode(VERDE2,OUTPUT);
  pinMode(AMARILLO2,OUTPUT);
  pinMode(ROJO2,OUTPUT);
  //Encendemos los LED de acuerdo a la configuración inicial
  //de los semáforos. Sólo la luz verde del segundo y la roja del primero
  //deben estar encendidas.
  digitalWrite(AMARILLO1,LOW);
  digitalWrite(VERDE1,LOW); 
  digitalWrite(ROJO1,HIGH);
  digitalWrite(VERDE2,HIGH);
  digitalWrite(AMARILLO2,LOW);
  digitalWrite(ROJO2,LOW);
}

void loop() {
    //En el primer loop las luces ya tienen esta configuración.
    digitalWrite(ROJO2,LOW);      //Luz roja del semaforo 2 apagada
    digitalWrite(VERDE2,HIGH);    //Luz verde del semaforo 1 prendida
    digitalWrite(AMARILLO1,LOW);  //Luz amarilla del semaforo uno apagada
    digitalWrite(ROJO1,HIGH);     //Luz roja del semaforo 1 prendida
    delay(5000);                  //Se mantienen las luces durante 5 segundos
    digitalWrite(AMARILLO2,HIGH); //Luz amarilla semaforo 2 prendida
    digitalWrite(VERDE2,LOW);     //Luz verde semaforo dos apagada 
    delay(1000);                  //Se mantienen las luces durante 1 segundo
    digitalWrite(ROJO1,LOW);      //Luz roja del semaforo 1 apagada
    digitalWrite(VERDE1,HIGH);    //Luz verde del semaforo 1 prendida
    digitalWrite(AMARILLO2,LOW);  //Luz amarilla del semaforo 2 apagada
    digitalWrite(ROJO2,HIGH);     //Luz roja del semaforo 2 prendida
    delay(5000);                  //Se mantienen las luces durante 5 segundos
    digitalWrite(AMARILLO1,HIGH); //Luz amarilla del semaforo 1 prendida
    digitalWrite(VERDE1,LOW);     //Luz verde del semaforo 1 apagada
    delay(1000);                  //Se mantienen las luces durante 1 segundo
}
