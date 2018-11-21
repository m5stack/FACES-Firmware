#include <Wire.h>

#define FACES_KEYBOARD_I2C_ADDR 0x08

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  pinMode(5, INPUT);
  digitalWrite(5,HIGH);
}
void loop()
{
  if (digitalRead(5) == LOW)
  {
    Wire.requestFrom(FACES_KEYBOARD_I2C_ADDR, 1);
    while (Wire.available())
    {
      char c = Wire.read(); // receive a byte as character
      Serial.print(c);         // print the character
    }
  }
  delay(10);
}
