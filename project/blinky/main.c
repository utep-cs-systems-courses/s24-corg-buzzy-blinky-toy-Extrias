#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void){
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_GREEN;

  configureClocks();
  enableWDTInterrupts();

  or_sr(0x18);
}

void
_interrupt_vec(WDT_VECTOR) WDT()        //250 interrupts/sec
{
  P1OUT |= LED_GREEN;

}