#define LED 2

int channel = 0;
int freq = 1000;
int resolution = 10;
int l; //luminosidad

void setup() {
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(LED, channel);
}

void loop() {
  //Modulacion por pulsos
  for(l = 0; l <= 1023; l++){
    ledcWrite(channel, l);
    delay(10);
  }
  for(l = 1023; l >= 0; l--){
    ledcWrite(channel, l);
    delay(10);
  }
}
