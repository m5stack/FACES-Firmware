#include <Wire.h>

#define FACES_KEYBOARD_I2C_ADDR 0x08

#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)     (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )
//KEY PORTB
//IRQ PC0
#define IRQ_1 Set_Bit(PORTC,0)
#define IRQ_0 Clr_Bit(PORTC,0)

unsigned char oldPINB=0;
void setup()
{
  DDRC = 0x01;
  PORTC = 0x01;
  DDRD = 0x00;
  PORTD = 0x00;
  DDRB = 0x00;
  PORTB = 0xff;
  oldPINB=PINB;
  Wire.begin(FACES_KEYBOARD_I2C_ADDR);
  Wire.onRequest(requestEvent);
}
unsigned char i = 0;
unsigned char temp = 0, hadPressed = 0;
void requestEvent()
{
  Wire.write(PINB);
  if (hadPressed == 1)
  {
    hadPressed = 0;
    IRQ_1;
  }

}
void loop()
{
  if(PINB!=oldPINB)
  {
    temp=PINB;
    oldPINB=temp;
    IRQ_0;
    hadPressed = 1;
  }
  /*
  if (PINB != 0xff)
  {
    switch(PINB)
    {
      case 0xFE:temp='U';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
      case 0xFD:temp='D';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
      case 0xFB:temp='L';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
      case 0xF7:temp='R';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
      case 0xEF:temp='A';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
      case 0xDF:temp='B';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
      case 0xBF:temp='s';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
      case 0x7F:temp='S';while(PINB!=0xff)delay(1);IRQ_0;hadPressed = 1;break;
    }
  }*/
  delay(10);
}
