#include <Wire.h>

#define FACES_KEYBOARD_I2C_ADDR 0x08

#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)     (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )

//L1 PD2
//L2 PD3
//L3 PD4
//L4 PD5
//L5 PD6

//C1 PC0
//C2 PC1
//C3 PC2
//C4 PC3

//IRQ PB0
#define IRQ_1 Set_Bit(PORTB,0)
#define IRQ_0 Clr_Bit(PORTB,0)
void setup()
{
  Set_Bit(DDRB, 0);
  IRQ_1;
  DDRC = 0x00;
  PORTC = 0xff;
  DDRD = 0xff;
  PORTD = 0x00;
  Wire.begin(FACES_KEYBOARD_I2C_ADDR);
  Wire.onRequest(requestEvent);
}


unsigned char i = 0;
unsigned char temp = 0,  hadPressed = 0;
void requestEvent()
{
  if (hadPressed == 1)
  {
    Wire.write(temp);
    hadPressed = 0;
    IRQ_1;
    return;
  }
}

int count = 0;
void loop()
{
  // Serial.println(temp);
  Clr_Bit(PORTD, 2);
  Set_Bit(PORTD, 3);
  Set_Bit(PORTD, 4);
  Set_Bit(PORTD, 5);
  Set_Bit(PORTD, 6);
  delay(1);
  switch (PINC & 0x0f)
  {
    case 0x0e:
      count = 0;
      while ((PINC & 0x0f) != 0x0f)
      {
        if (count++ > 400)
        {
          temp = 0x08;  //backspace
          hadPressed = 1; IRQ_0;
          while ((PINC & 0x0f) != 0x0f) delay(1);
          return;  break;
        }
        delay(1);
      }
      temp = 'A'; hadPressed = 1; IRQ_0; return;  break;
    case 0x0d: temp = 'M'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
    case 0x0b: temp = '%'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x07: temp = '/'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
  }
  Set_Bit(PORTD, 2);
  Clr_Bit(PORTD, 3);
  Set_Bit(PORTD, 4);
  Set_Bit(PORTD, 5);
  Set_Bit(PORTD, 6);
  delay(1);
  switch (PINC & 0x0f)
  {
    case 0x0e: temp = '7'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x0d: temp = '8'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
    case 0x0b: temp = '9'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x07: temp = '*'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
  }
  Set_Bit(PORTD, 2);
  Set_Bit(PORTD, 3);
  Clr_Bit(PORTD, 4);
  Set_Bit(PORTD, 5);
  Set_Bit(PORTD, 6);
  delay(1);
  switch (PINC & 0x0f)
  {
    case 0x0e: temp = '4'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x0d: temp = '5'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
    case 0x0b: temp = '6'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x07: temp = '-'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
  }

  Set_Bit(PORTD, 2);
  Set_Bit(PORTD, 3);
  Set_Bit(PORTD, 4);
  Clr_Bit(PORTD, 5);
  Set_Bit(PORTD, 6);
  delay(1);
  switch (PINC & 0x0f)
  {
    case 0x0e: temp = '1'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x0d: temp = '2'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
    case 0x0b: temp = '3'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x07: temp = '+'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
  }
  Set_Bit(PORTD, 2);
  Set_Bit(PORTD, 3);
  Set_Bit(PORTD, 4);
  Set_Bit(PORTD, 5);
  Clr_Bit(PORTD, 6);
  delay(1);
  switch (PINC & 0x0f)
  {
    case 0x0e: temp = '.'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x0d: temp = '0'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return;  break;
    case 0x0b: temp = '`'; while ((PINC & 0x0f) != 0x0f)delay(1); hadPressed = 1; IRQ_0; return; break;
    case 0x07:
      count = 0;
      while ((PINC & 0x0f) != 0x0f)
      {
        if (count++ > 400)
        {
          temp = 0x0d;
          hadPressed = 1; IRQ_0;
          while ((PINC & 0x0f) != 0x0f) delay(1);
          return;  break;
        }
        delay(1);
      }
      temp = '='; hadPressed = 1; IRQ_0; return;  break;
  }
  delay(10);
}
