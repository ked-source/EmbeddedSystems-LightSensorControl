#include <Arduino.h>

//Light sensor on A5 - ADC0 - PF0 - pin 41
//Free Running
//LED on PC7 which is also OC4A -> means timer A channel 4 can control LED

unsigned char* DDRCptr = (unsigned char*) 0x27;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // c. Configure channel A on timer 4
  TCCR4A |= (1 << WGM41) | (1 << WGM40) | (1 << COM4A1);
  TCCR4A &= ~(1 << COM4A0);

  // Set prescaler to 1024
  TCCR4B |= (1 << CS42) | (1 << CS40);
  TCCR4B &= ~(1 << CS41);

  // Set top value to 255
  OCR4A = 255;


  // d. Set up the ADC to the channel that reads the light sensor using a prescaler of 16, free running mode, auto trigger enable 
  ADMUX = 0b01000000;
  //  xx - use vcc as reference
  //  x - right justify
  //  xxxxx - using ADC0, all LOW

  ADCSRA = 0b10100100;
  //  x - Turn on ADC hardware
  //  x - don't start yet
  //  x - enable auto trigger
  //  xx - save interrupts for later
  //  xxx - Set prescaler to 16

  ADCSRB = 0b00000000;
  //  x - low speed
  //  x - NA
  //  x - Last bit of MUX
  //  x - NA
  //  xxxx = Free running

  ADCSRA |= (1 << ADSC);

  //e. Set the LED gpio to output mode (again in “setup”).
  *DDRCptr |= (1 << 7);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(ADCW); 

  
  uint16_t ADCvalue = ADCW;
  uint8_t DutyCycle = ADCvalue >> 2;
  OCR4A = DutyCycle;

  //g. Be sure to add delay(10) in the loop() at the end.
  delay(10);
}
