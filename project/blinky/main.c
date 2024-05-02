#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void){
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();
  enableWDTInterrupts();
  buzzer_init();

  or_sr(0x18);
}

void
__interrupt_vec(WDT_VECTOR) WDT()        //250 interrupts/sec
{
  static int state;
  P1OUT |= LED_GREEN;

}
