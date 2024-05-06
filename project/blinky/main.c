#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include <stdio.h>

int main(void){
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();
  enableWDTInterrupts();
  // buzzer_init();

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

