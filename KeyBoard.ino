#include <Wire.h>

#define FACES_KEYBOARD_I2C_ADDR 0x08

#define Set_Bit(val, bitn)    (val |=(1<<(bitn)))
#define Clr_Bit(val, bitn)     (val&=~(1<<(bitn)))
#define Get_Bit(val, bitn)    (val &(1<<(bitn)) )

//C1 PC2
//C2 PC3
//C3 PC1  except alt and enter

//R1 PB1 QA
//R2 PB0 WSZ
//R3 PD5 EDX
//R4 PD6 RFC
//R5 PD7 TGV
//R6 PD4 YHB
//R7 PD3 UJN
//R8 PD2 IKM
//R9 PD1 OL$
//R10 PD0 P+del

//PB6 Enter
//PB4 Alt
//PB3 Fn
//PB7 sym

//PB5 aA
//PC0 space

//PC0 LEDR(1) LEDL(0)
//PB2 IRQ

#define  LED_L_1  Set_Bit(DDRC,0);Set_Bit(PORTC,0)
#define  LED_R_1   Set_Bit(DDRC,0);Clr_Bit(PORTC,0)
#define  LED_L_0   Clr_Bit(DDRC,0);Clr_Bit(PORTC,0)
#define  LED_R_0   Clr_Bit(DDRC,0);Clr_Bit(PORTC,0)

#define IRQ_1 Set_Bit(PORTB,2)
#define IRQ_0 Clr_Bit(PORTB,2)
unsigned char ALT = 0, aA = 0, SYM = 0, FN = 0, ESC = 0;
unsigned char twoBytes = 0;
unsigned char KEY = 0, KEY2 = 0, hadPressed = 0;

#define aA_Pressed (PINB & 0x20) != 0x20
#define SYM_Pressed (PINB & 0x80) != 0x80
#define ALT_Pressed (PINB & 0x10) != 0x10
#define FN_Pressed (PINB & 0x08) != 0x08
#define ENTER_Pressed (PINB & 0x40) != 0x40
#define aA_unPressed (PINB & 0x20) == 0x20
#define SYM_unPressed (PINB & 0x80) == 0x80
#define ALT_unPressed (PINB & 0x10) == 0x10
#define FN_unPressed (PINB & 0x08) == 0x08
#define ENTER_unPressed (PINB & 0x40) == 0x40
int LedMode = 0; //0->LR off.1->L on,2->L slow , 3->L fast, 4->R on,5->R slow,6->R fast,7->L R slow 8->LR Fast

void setup()
{
  DDRB = 0x04;
  PORTB = 0xfb;
  IRQ_1;

  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);

  for(int i=0;i<5;i++)
  {
    LED_L_1;
    delay(i*30);
    LED_R_1;
    delay(i*30);
  }
  LED_R_0;
  DDRD = 0x00;
  PORTD = 0xff;

  LedMode = 1;
  Wire.begin(FACES_KEYBOARD_I2C_ADDR);
  Wire.onRequest(requestEvent);

}

void requestEvent()
{
  if (hadPressed == 1)
  {
    Wire.write(KEY);
    hadPressed = 0;
    IRQ_1;
    return;
  }
  if (twoBytes == 1)
  {
    Wire.write(KEY2);
    twoBytes = 0;
  }
}


unsigned char GetInput()
{
  if (aA_Pressed)
  {
    FN = 0; SYM = 0; ALT = 0;
    while (aA_Pressed)delay(1); //release key
    if (aA == 0) //had not pressed
    {
      delay(200);  //detect doblue click
      if (aA_Pressed)
      {
        while (aA_Pressed)delay(1);
        aA = 2;
        LedMode = 2;
      }
      else
      {
        aA = 1;
        LedMode = 1;
      }
    }
    else // had pressed
    {
      delay(200);
      if (aA_Pressed)
      {
        while (aA_Pressed)delay(1);
        if (aA == 2)
        {
          aA = 0;
        }
        else
        {
          LedMode = 2;
          aA = 2;
        }
      }
      else
      {
        aA = 0;
      }
    }
  }
  if (ALT_Pressed)
  {
    FN = 0; SYM = 0; aA = 0;
    LedMode = 3;
    ALT = 1;
  }
  else
  {
    ALT = 0;
  }


  if (FN_Pressed)
  {
    aA = 0; SYM = 0; ALT = 0;
    while (FN_Pressed)delay(1); //release key
    if (FN == 0) //had not pressed
    {
      delay(200);  //detect doblue click
      if (FN_Pressed)
      {
        while (FN_Pressed)delay(1);
        FN = 2;
        LedMode = 5;
      }
      else
      {
        FN = 1;
        LedMode = 4;
      }
    }
    else // had pressed
    {
      delay(200);
      if (FN_Pressed)
      {
        while (FN_Pressed)delay(1);
        if (FN == 2)
        {
          FN = 0;
        }
        else
        {
          LedMode = 5;
          FN = 2;
        }
      }
      else
      {
        FN = 0;
      }
    }
  }
  if (SYM_Pressed)
  {
    aA = 0; FN = 0; ALT = 0;
    while (SYM_Pressed)delay(1); //release key
    if (SYM == 0) //had not pressed
    {
      delay(200);  //detect doblue click
      if (SYM_Pressed)
      {
        while (SYM_Pressed)delay(1);
        SYM = 2;
        LedMode = 6;
      }
      else
      {
        SYM = 1;
        LedMode = 7;
      }
    }
    else // had pressed
    {
      delay(200);
      if (SYM_Pressed)
      {
        while (SYM_Pressed)delay(1);
        if (SYM == 2)
        {
          SYM = 0;
        }
        else
        {
          LedMode = 6;
          SYM = 2;
        }
      }
      else
      {
        SYM = 0;
      }
    }
  }

  if (ENTER_Pressed)
  {
    while (ENTER_Pressed)delay(1);  //ENTER
    return 29;
  }

  //normal key scan
  Set_Bit(PORTC, 1);
  Set_Bit(PORTC, 3);
  Clr_Bit(PORTC, 2);
  delay(2);
  switch (PIND)
  {
    case 0xfe: while (PIND != 0xff)delay(1); return  9; break;
    case 0xfd: while (PIND != 0xff)delay(1);  return 8; break;
    case 0xfb: while (PIND != 0xff)delay(1);  return 7; break;
    case 0xf7: while (PIND != 0xff)delay(1); return 6; break;
    case 0xef: while (PIND != 0xff)delay(1); return 5; break;
    case 0xdf: while (PIND != 0xff)delay(1); return 2; break;
    case 0xbf: while (PIND != 0xff)delay(1); return  3; break;
    case 0x7f: while (PIND != 0xff)delay(1); return  4; break;
  }
  switch (PINB & 0x03)
  {
    case 0x01: while ((PINB & 0x03) != 0x03)delay(1); return 0; break;
    case 0x02: while ((PINB & 0x03) != 0x03)delay(1); return 1; break;
  }
  Set_Bit(PORTC, 1);
  Set_Bit(PORTC, 2);
  Clr_Bit(PORTC, 3);
  delay(2);
  switch (PIND)
  {
    case 0xfe: while (PIND != 0xff)delay(1); return  19; break;
    case 0xfd: while (PIND != 0xff)delay(1);  return 18; break;
    case 0xfb: while (PIND != 0xff)delay(1);  return 17; break;
    case 0xf7: while (PIND != 0xff)delay(1); return 16; break;
    case 0xef: while (PIND != 0xff)delay(1); return 15; break;
    case 0xdf: while (PIND != 0xff)delay(1); return 12; break;
    case 0xbf: while (PIND != 0xff)delay(1); return  13; break;
    case 0x7f: while (PIND != 0xff)delay(1); return  14; break;
  }
  switch (PINB & 0x03)
  {
    case 0x01: while ((PINB & 0x03) != 0x03)delay(1); return 10; break;
    case 0x02: while ((PINB & 0x03) != 0x03)delay(1); return 11; break;
  }

  Set_Bit(PORTC, 2);
  Set_Bit(PORTC, 3);
  Clr_Bit(PORTC, 1);
  delay(2);
  switch (PIND)
  {
    case 0xfe: while (PIND != 0xff)delay(1); return  32; break;
    case 0xfd: while (PIND != 0xff)delay(1);  return 28; break;
    case 0xfb: while (PIND != 0xff)delay(1);  return 27; break;
    case 0xf7: while (PIND != 0xff)delay(1); return 26; break;
    case 0xef: while (PIND != 0xff)delay(1); return 25; break;
    case 0xdf: while (PIND != 0xff)delay(1); return 22; break;
    case 0xbf: while (PIND != 0xff)delay(1); return  23; break;
    case 0x7f: while (PIND != 0xff)delay(1); return  24; break;
  }
  switch (PINB & 0x03)
  {
    case 0x01: while ((PINB & 0x03) != 0x03)delay(1); return 31; break;
    case 0x02: while ((PINB & 0x03) != 0x03)delay(1); return 21; break;
  }

  return 255;

}
unsigned int idle = 0;
unsigned char KeyMap[35][5] =
{ {'q', 'Q', '#', '~', 144},
  {'w', 'W', '1', '^', 145},
  {'e', 'E', '2', '&', 146},
  {'r', 'R', '3', '`', 147},
  {'t', 'T', '(', '<', 148},
  {'y', 'Y', ')', '>', 149},
  {'u', 'U', '_', '{', 150},
  {'i', 'I', '-', '}', 151},
  {'o', 'O', '+', '[', 152},
  {'p', 'P', '@', ']', 153},
  {'a', 'A', '*', '|', 154},
  {'s', 'S', '4', '=', 155},
  { 'd', 'D', '5', '\\', 156},
  {'f', 'F', '6', '%', 157},
  {'g', 'G', '/', 180, 158},
  {'h', 'H', ':', 181, 159},
  {'j', 'J', ';', 182, 160},
  {'k', 'K', '\'', 183 , 161},
  {'l', 'L', '"', 184, 162},
  {8 , 8, 127, 8, 163}, //del & backspace
  {255, 255, 255, 185, 164}, //alt
  {'z', 'Z', '7', 186, 165},
  {'x', 'X', '8', 187, 166},
  {'c', 'C', '9', 188, 167},
  {'v', 'V', '?', 189, 168},
  {'b', 'B', '!', 190, 169},
  {'n', 'N', ',', 191, 170},
  {'m', 'M', '.', 192, 171},
  {'$', '$', 255, 193, 172},
  {13, 13, 13, 13, 173}, //enter
  {255, 255, 255, 255, 174}, //aA
  {'0', '0', '0', '0', 175},
  {' ', ' ', ' ', ' ', 176},
  {255, 255, 255, 255, 177},
  {255, 255, 27, 255, 178}
};
void loop()
{
  idle++;
  unsigned char kk = GetInput();
  if (aA == 0 && ALT == 0 && FN == 0 && SYM == 0)
    LedMode = 0;
  switch (LedMode)
  {
    case 0: LED_L_0; break;
    case 1: LED_L_1; break;
    case 2:
      if ((idle / 20) % 2 == 1)
      {
        LED_L_0;
      } else {
        LED_L_1;
      } break;
    case 3:
      if ((idle / 10) % 2 == 1)
      {
        LED_L_0;
      }
      else
      {
        LED_L_1;
      } break;
    case 4: LED_R_1; break;
    case 5:
      if ((idle / 20) % 2 == 1)
      {
        LED_R_0;
      }
      else
      {
        LED_R_1;
      } break;
    case 6:
      if ((idle / 10) % 2 == 1)
      {
        LED_R_0;
      }
      else
      {
        LED_R_1;
      } break;
    case 7:
      if ((idle / 30) % 2 == 1)
      {
        LED_L_1;
      }
      else
      {
        LED_R_1;
      } break;
    case 8:
      if ((idle / 20) % 2 == 1)
      {
        LED_L_1;
      }
      else
      {
        LED_R_1;
      } break;
  }
  if (kk == 29)
  {
    KEY = 0x0d; twoBytes = 1; KEY2 = 0x0a;//enter
    hadPressed = 1; IRQ_0;
  }
  else if (kk < 36)
  {
    if (ALT > 0)
    {
      KEY = KeyMap[kk][4];
    }
    else
    {
      if (SYM > 0)
      {
        if (SYM == 1)SYM = 0;
        KEY = KeyMap[kk][2];
      }
      else
      {
        if (FN > 0)
        {
          if (FN == 1)FN = 0;
          KEY = KeyMap[kk][3];
        }
        else
        {
          if (aA > 0)
          {
            if (aA == 1)
              aA = 0;
            KEY = KeyMap[kk][1];
          }
          else
            KEY = KeyMap[kk][0];
        }
      }
    }
    hadPressed = 1;
    IRQ_0;
  }
}
