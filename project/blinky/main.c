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

static int color = 1;        //for first button to switch the light thats blinking
static int button3 = 0;      //changes to 1 when button3(SW3) is pressed
static int button4 = 0;      //changes to 1 when buttin4 (SW4) is pressed

int main(void){
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();
  enableWDTInterrupts();
  
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;

  
  buzzer_init();

  or_sr(0x18);
}

void blinking_green_light(){
 static int state = 0;
  static int on = 1;

  switch(on){
  case 0:
    switch(color){
    case 0:
      P1OUT &= ~LED_RED;
      break;
    case 1:
      P1OUT &= ~LED_GREEN;
      break;
    }
    
    break;

  case 1:
    switch(color){
    case 0:
      P1OUT |= LED_RED;
      break;
    case 1:
      P1OUT |= LED_GREEN;
      break;
    }
    
    break;
  }

  state++;

  if(state >= 250){
    state = 0;
    on = !on;
  }

}

/*void crazy_light(){
  static int state = 0;
  static int on = 1;

  switch(on){
  case 0:
    
      P1OUT &= ~LED_RED;
      P1OUT |= LED_GREEN;
    
    break;

  case 1:
    
      P1OUT |= LED_RED;
      P1OUT &= ~LED_GREEN;
    
    break;
  }

  state++;

  if(state >= 30){
    state = 0;
    on = !on;
  }
}*/
crazy_light:
    ; Load the address of the variable 'state' into a register
    mov.w &state, r12

    ; Load the value of 'state' into a register
    mov.w @r12, r13

    ; Load the address of the variable 'on' into a register
    mov.w &on, r14

    ; Load the value of 'on' into a register
    mov.w @r14, r15

    ; Compare 'on' with 0
    cmp #0, r15

    ; If 'on' is equal to 0, jump to on_is_zero label
    jeq on_is_zero

    ; If 'on' is not equal to 0, execute the following instructions

    ; Set LED_RED (P1OUT |= LED_RED)
    bis #LED_RED, &P1OUT

    ; Clear LED_GREEN (P1OUT &= ~LED_GREEN)
    bic #LED_GREEN, &P1OUT

    ; Jump to end_of_switch_case
    jmp end_of_switch_case

on_is_zero:
    ; If 'on' is equal to 0, execute the following instructions

    ; Clear LED_RED (P1OUT &= ~LED_RED)
    bic #LED_RED, &P1OUT

    ; Set LED_GREEN (P1OUT |= LED_GREEN)
    bis #LED_GREEN, &P1OUT

end_of_switch_case:
    ; Increment 'state' by 1
    inc @r12

    ; Compare 'state' with 30
    cmp #30, r13

    ; If 'state' is greater than or equal to 30, jump to end_of_state_check

    ; If 'state' is less than 30, execute the following instructions

    ; Store the updated value of 'state' back into memory
    mov.w r13, @r12

    ; Jump to end_of_function
    jmp end_of_function

end_of_state_check:
    ; If 'state' is greater than or equal to 30, execute the following instructions

    ; Reset 'state' to 0
    clr @r12

    ; Toggle the value of 'on' (on = !on)
    bit #1, &on

end_of_function:
    ret

void glowy(){
  static int timer = 0;
  static int state = 0;
  int timer2 = 0;
  if(state/(41*(1+timer)) == 1){
    int timer2 = state/(41);
    if(state % 41 == 0){
      timer++;
    }
  }
  
  
  switch(timer2){
  case 1:
    if(state % 1 == 0){
  P1OUT |= LED_GREEN;
  }
  else{
    P1OUT &= ~LED_GREEN;
  }
    break;

  case 2:
    if(state % 2 == 0){
  P1OUT |= LED_GREEN;
  }
  else{
    P1OUT &= ~LED_GREEN;
  }
    break;

  case 3:
    if(state % 3 == 0){
  P1OUT |= LED_GREEN;
  }
  else{
    P1OUT &= ~LED_GREEN;
  }
    break;
  case 4:
    if(state % 4 == 0){
  P1OUT |= LED_GREEN;
  }
  else{
    P1OUT &= ~LED_GREEN;
  }
    break;

  case 5:
    if(state % 5 == 0){
  P1OUT |= LED_GREEN;
  }
  else{
    P1OUT &= ~LED_GREEN;
  }
    break;

  case 6:
    if(state % 6 == 0){
  P1OUT |= LED_GREEN;
  }
  else{
    P1OUT &= ~LED_GREEN;
  }
    break;
  default:
    P1OUT |= LED_GREEN;
    break;
    
  }
  
 
  
  state++;
  if(state == 250){
    state = 0;
    timer = 0;
  }
}

void
__interrupt_vec(WDT_VECTOR) WDT()        //250 interrupts/sec
{
  if(!button3 & !button4){
  blinking_green_light();
  }
  else if(button3){
    crazy_light();
  }
  else if(button4){
    glowy();
  }


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
  int button = 0;            //to see what button was pressedd
  
  if(p2val & SW1 ? 0 : 1){ button = 1;}
  if(p2val & SW2 ? 0 : 1){ button = 2;}
  if(p2val & SW3 ? 0 : 1){ button = 3;}
  if(p2val & SW4 ? 0 : 1){ button = 4;}
  

  switch (button){
  case 1:
    color = !color;
    break;
  case 2:                  //activates when s1
    tetris_melody();
    buzzer_set_period(0);
    break;
  case 3:
    button3 = !button3;
    break;
  case 4:
    button4 = !button4;
    break;
  default:
    break;
  }
  
  }

void __interrupt_vec(PORT2_VECTOR) Port_2(){
    if(P2IFG & SWITCHES){
      P2IFG &= ~SWITCHES;
      switch_interrupt_handler();
    }
 
}


