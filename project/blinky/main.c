#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include <stdio.h>
#include "buzzer.h"

int main(void){
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();
  enableWDTInterrupts();
  buzzer_init();
  buzzer_set_period(1000);

  or_sr(0x18);
}

void blinking_green_light(){
 static int state = 0;
  static int on = 1;

  switch(on){
  case 0:
    P1OUT &= ~LED_GREEN;
    break;

  case 1:
    P1OUT |= LED_GREEN;
    break;
  }

  state++;

  if(state >= 250){
    state = 0;
    on = !on;
  }

}

void
__interrupt_vec(WDT_VECTOR) WDT()        //250 interrupts/sec
{
 
  blinking_green_light();


}

void tetris_melody(){
    int frequencies[] = {
      659.25511, 493.8833, 523.25113, 587.32954, 523.25113, 493.8833, 440.0, 0.0, 440.0, 
    523.25113, 659.25511, 587.32954, 523.25113, 493.8833, 523.25113, 587.32954, 
      659.25511, 523.25113, 440.0, 0.0, 440.0, 0.0, 440.0, 493.8833, 523.25113, 587.32954, 
    698.45646, 880.0, 783.99087, 698.45646, 659.25511, 523.25113, 659.25511, 
    587.32954, 523.25113, 493.8833, 493.8833, 523.25113, 587.32954, 659.25511, 
      523.25113, 440.0, 0.0, 440.0};
    
    int durations[] = {
      406.250, 203.125, 203.125, 406.250, 203.125, 203.125, 406.250, 10.0, 203.125, 
    203.125, 406.250, 203.125, 203.125, 609.375, 203.125, 406.250, 406.250, 
      406.250, 406.250, 10.0, 406.250, 10.0, 406.250, 203.125, 203.125, 609.375, 203.125, 
    406.250, 203.125, 203.125, 609.375, 203.125, 406.250, 203.125, 203.125, 
      406.250, 203.125, 203.125, 406.250, 406.250, 406.250, 406.250, 10.0, 406.250
    };

    int numNotes = sizeof(frequencies) / sizeof(frequencies[0]);

    for(int i = 0; i < numNotes; i++){
      int ccr0Val = 2000000 / frequencies[i];

      buzzer_set_period(ccr0Val);

      delay_ms(durations[i]*10);
    }
    
}

void delay_ms(unsigned int ms){
  unsigned int i;
  for(i = 0; i < ms; i++){
    __delay_cycles(2000);
  }
}

