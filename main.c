#include <avr/io.h>
#include "cube.h"
#include <avr/interrupt.h>
#include "io.h"
#define buttonPressed 0x01
#define timerInterrupt 0x00

//#define SREGSS _SFR_MEM8(0x3F)

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_cntcurr = 0;
unsigned long _avr_timer_M = 1;
long floor_sel = 1;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1= 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
	load_led_on_offs(floor_sel);
	if (bitRead(floor_sel, 4)) floor_sel = 1;
	else floor_sel = floor_sel << 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
}

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	//ADEN: setting this bit enables analog-to-digital conversion
	//ADSC: setting this bit starts the first conversion
	//ADATE: setting this bit enables auto-triggering. Since we are
	//in Free Running Mode, a new conversion will trigger when the
	//previous conversion completes.
	ADMUX = 0;
}

typedef enum States {start, game, gamePause, gameOver, reset} States;
//unsigned char dir = 0;
unsigned char score = 0; //LED Game Score

int LED_Game(int state, int source) {
	unsigned char playButton = (~PINA) & 0x04;
	unsigned char resetButton = (~PINA) & 0x08;
	//unsigned char VRx = 0;
	//unsigned char VRy = 1;
	switch (state) { // Transitions
		case start:
			if(playButton) {
				state = game;
				score = 0;
				clr_cube();
				LCD_DisplayString(1, "Press Button To Stop the Game");
			} 
		break;
		
		case game:
			//ADMUX = VRx;
			if (tetrix(0)) {
				unsigned char x = (~PINA) & 0x01;
				unsigned char y = (~PINA) & 0x02;
				//unsigned char z = (~PINA) & 0x08;
				score++;
				LCD_Cursor(1);
				LCD_WriteData(score+0x30);
				if(x) move_shape(1);
				else if(y) move_shape(2);
				//else if(z) {move_shape(3);}
			//	else if((VRx_adc < 512) && (VRx_adc > 256)) {move_shape(3);}
				if(playButton) state = gamePause;
				if(resetButton) state = reset;
			} else {
				state = gameOver;
			}
		break;
		case gamePause:
			tetrix(0);
			if(playButton) {
				state = game;
			} else if(resetButton) {
				state = reset;
			}
		break;
		case gameOver:
			if(playButton) {
				state = start;
				clr_cube();
			}
			LCD_Cursor(1);
			LCD_WriteData(score+0x30);
			//TimerSet(30);
			//TimerOn();
		break;
		case reset:
			state = start;
			clr_cube();
		break;
		default:
			state = start;
		break;
	}
	return state;
}



int main() {
	DDRA = 0x00; PORTA = 0xFF; // ACD input
	// DDRB = 0xFF; PORTB = 0x00; // output

	// DDRB = 0x1D; PORTB = 0xE2; // PORTB set to output, outputs init 0s


	DDRC = 0xFF; PORTC = 0x00; // output
	DDRD = 0xFF; PORTD = 0x00; // output
	
	unsigned long game_speed = 500000;
	unsigned long call_count = 0;

	//unsigned char my_char;
	//unsigned char button = 0;
	//unsigned char buttonReg = 0;
	//unsigned char VRx = 0;
	//unsigned char VRy = 0;
	States state = start;
	
	//ADC_init(); // initialize ADC
	LCD_init(); // initialize LCD
	cube_init(); // initialize LED cube
	LCD_ClearScreen(); 
	TimerOn();
	TimerSet(7);
	//TimerFlag = 0;
	LCD_Cursor(1);
	LCD_DisplayString(1, "Press Button To Start a New Game");
	clr_cube();
	tetrix(1);
	while(1) {
		
		//my_short = ADC;
		//my_char = (char)(my_short >> 8);
		//PORTD = my_char;
		//LCD_WriteData(my_char);
		if (call_count == game_speed) {
			state = LED_Game(state, timerInterrupt);
			call_count = 0;
		} else call_count++;
		//load_led_on_offs(floor_sel);
		//while (!TimerFlag) {}
		//TimerFlag = 0;
		//delay(1);
		
	}
}


/*
int main(){
	cube_init();
	while (1)
	{
		//teq_hartutjun();
		//vert_hartutyun();
		//cube_verev_vazox();
		//cube_vazox();
		if (!tetrix()) {
			clr_cube();
			// Do other actions to report game over
		}
	}
	return 1;
}
*/
