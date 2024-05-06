#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include <stdio.h>
#include "buzzer.h"

#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3
#define SWITCHES (SW1 | SW2 | SW3 | SW4)

void delay_ms(unsigned int ms);
void tetris_melody();

int main(void){
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();
  enableWDTInterrupts();
  
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;

  
  buzzer_init();

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

void switch_interrupt_handler(){
  static int music_on = 0;
  
  char p2val = P2IN;      /* switch is in P2 */

  //update switch interrupt sense to detenct changes from current buttons
  P2IES |= (p2val & SWITCHES);  //if switch up, sense down
  P2IES &= (p2val | ~SWITCHES); //if switch down, sense up

  /* up=red, down=green */

  if(p2val & SW1){
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_GREEN;
    int button1 = 1;
  }
  else{
    P1OUT &= ~LED_GREEN;
    P1OUT |= LED_GREEN;
  }

  if(p2val & SW2){
    if(music_on){
      buzzer_stop();
      music_on = !music_on;
    }
    else{
      tetris_melody();
      music_on = !music_on;
    }
  }
  }

void __interrupt_vec(PORT2_VECTOR) Port_2(){
    if(P2IFG & SWITCHES){
      P2IFG &= ~SWITCHES;
      switch_interrupt_handler();
    }
  
}

