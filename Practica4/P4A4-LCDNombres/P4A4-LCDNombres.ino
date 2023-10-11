#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  //lcd.noBacklight();

}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("Herculano Bais");
  lcd.setCursor(0,1);
  lcd.print("bye bitch");
  delay(5000);
  lcd.clear();
  delay(2000);
}
