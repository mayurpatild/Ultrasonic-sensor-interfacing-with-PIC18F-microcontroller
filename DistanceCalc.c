 /*
  * Ultrasonic sensor HC-SR04
  * Configuracion:
      MCU: PIC16F877A
      Oscillator: HS, 12.0000 MHz
      HS 4MHz
      Character LCD 2x16
      HC-SR04
      Todo "Default".
      Watch dog OFF
      Timer up OFF

      RA1= TRIGGER

      RC2= ECHO

      SW: mikroC PRO for PIC
 */

// LCD connections
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB6_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;


sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB6_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;
// End LCD connections


char txt1[] = "Distance sensor";
char txt2[] = "HC-SR04 BMP";
char txt3[] = "Distancia cm :";
char txt4[7];
char txt5[7];
char txt6[7];
unsigned long Cumul;
unsigned tWord,tOld, tNew ;
char th,tl;
char edge = 0;
char capture = 0;
float temps = 0;
float distance = 0;  float distance3 = 0;  float distance5 = 0;
float distance2 = 0; float distance4 = 0;  float distance6 = 0;
unsigned int Int_Distance1=0;
unsigned int Int_Distance2=0;
unsigned int Int_Distance3=0;



void interrupt()    // FUNCION INTERRUPTION:
{
  if(PIR1.CCP1IF)
  {
      if(!edge)
      {
        CCP1CON = 0x04;
        tOld = 256*CCPR1H + CCPR1L;
        edge = 1;
      }     else
            {
              tNew =256*CCPR1H + CCPR1L;
              capture = 1;
              edge = 0;
            }
      PIR1.CCP1IF = 0;
  }

}


void Calcular_Distancia2()
{
  if(capture)
  {
        PIE1.CCP1IE = 0;
        capture = 0;
        tWord = ~tOld + tNew+1;
        CCP1CON = 0x05;

        temps =((float)tWord*4)/3;
        distance3 = ((float)temps*17)/1000;
        Int_distance2=distance3;
        IntToStr(Int_distance2,txt5);

        Delay_ms(70);
        PIR1.CCP1IF = 0;
        PIE1.CCP1IE = 1;

        if(distance3 !=distance4)
        {
          //Lcd_Cmd(_LCD_CLEAR);
          //Lcd_Cmd(_LCD_CURSOR_OFF);
          //Lcd_Out(1,1,txt3);
          Lcd_Out(2,1,txt5);
        }
        distance4 = distance3;
    }
}





void main()
{

  PCFG3_bit=0;
  PCFG2_bit=1;
  PCFG1_bit=1;

  TRISA = 0;
  PORTA = 0x00;
  TRISE= 0;
  PORTE=0;
  TRISB = 0;
  PORTB = 0;
  TRISC = 0;
  PORTC=0;
  TRISD=0;
  PORTD=0;
  CCP1CON = 0x05;
  TRISC.F2 = 1;

  T1CON = 0x21;
  INTCON.GIE = 1;
  INTCON.PEIE =1;
  PIE1.CCP1IE = 1;
  PIR1.CCP1IF = 0;

  Lcd_Init();
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Cmd(_LCD_CURSOR_OFF);
  Lcd_Out(1,2,txt1);
  Lcd_Out(2,6,txt2);
  Delay_ms(2000);
  Lcd_Cmd(_LCD_CLEAR);


  while(1)
  {


   // SEND THE PULSE TO THE SENSOR BY RA1 TO THE TRIGGER:


        delay_ms(100);

        PORTA.F1=1; Delay_ms(100); PORTA.F1 = 0;       //PIN 3

        Calcular_distancia2();

         PORTD=Int_Distance2; delay_ms(50);  // Show distance in Binary through Port D

  }

}
