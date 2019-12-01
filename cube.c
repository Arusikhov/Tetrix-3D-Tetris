/*
 * cube.c
 *
 * Created: 11/26/2019 9:55:55 PM
 *  Author: Arusyak
 */ 

#include <avr/io.h>
#include "cube.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#define HIGH 1
#define LOW 0

const long WHITE   = 7;
const long YELLOW  = 3;
const long CYAN    = 6;
const long GREEN   = 2;
const long MAGENTA = 5;
const long RED     = 1;
const long BLUE    = 4;
const long BLACK   = 0;

const long PSMODE = 0;
const long WRTCMD = 0x96;
const long LSDVLT = 1;
const long LODVLT = 2;
const long SIDLD  = 0;

const unsigned char bit_map[5][5][9] = {{{ 0, 0, 0,  1, 0, 0,  2, 0, 0}, { 3, 0, 0,  4, 0, 0,  5, 0, 0}, { 6, 0, 0,  7, 0, 0,  8, 0, 0}, { 9, 0, 0, 10, 0, 0, 11, 0, 0}, {12, 0, 0, 13, 0, 0, 14, 0, 0}},
	{{15, 0, 0,  0, 1, 0,  1, 1, 0}, { 2, 1, 0,  3, 1, 0,  4, 1, 0}, { 5, 1, 0,  6, 1, 0,  7, 1, 0}, { 8, 1, 0,  9, 1, 0, 10, 1, 0}, {11, 1, 0, 12, 1, 0, 13, 1, 0}},
	{{14, 1, 0, 15, 1, 0,  0, 2, 0}, { 1, 2, 0,  2, 2, 0,  3, 2, 0}, { 4, 2, 0,  5, 2, 0,  6, 2, 0}, { 7, 2, 0,  8, 2, 0,  9, 2, 0}, {10, 2, 0, 11, 2, 0, 12, 2, 0}},
	{{13, 2, 0, 14, 2, 0, 15, 2, 0}, { 0, 0, 1,  1, 0, 1,  2, 0, 1}, { 3, 0, 1,  4, 0, 1,  5, 0, 1}, { 6, 0, 1,  7, 0, 1,  8, 0, 1}, { 9, 0, 1, 10, 0, 1, 11, 0, 1}},
	{{12, 0, 1, 13, 0, 1, 14, 0, 1}, {15, 0, 1,  0, 1, 1,  1, 1, 1}, { 2, 1, 1,  3, 1, 1,  4, 1, 1}, { 5, 1, 1,  6, 1, 1,  7, 1, 1}, { 8, 1, 1,  9, 1, 1, 10, 1, 1}}};

long MC0 = 4;
long BCR0 = 127;
long BCG0 = 40;
long BCB0 = 127;
long MC1 = 4;
long BCR1 = 127;
long BCG1 = 95;
long BCB1 = 127;

long floor_sel = 1;

int BLANK = 4;
int LAT = 3;
int SOUT = 2;
//int SIN = 1;
int SCLK = 0;

long din[] = {0, 0, 0};
unsigned short din_w[2][3] = {{0, 0, 0},
	{0, 0, 0}};
unsigned char cmd_nled = 1;

unsigned char led_matrix[5][5][5]= { { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}
};

unsigned char store_matrix[5][5][5]= { { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}
};

unsigned char shape_matrix[5][5][5]= { { {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
	{ {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}}
};

unsigned char cntr = 0;
unsigned char cntr_dir = 1;

unsigned char bitRead(long d, unsigned char b){
	return ((d >> b) & 1);
}

long bitWrite(long d, unsigned char b, unsigned char v){
	if (v) return (d | (1 << b));
	else return (d & (~(1 << b)));
}

void digitalWrite(unsigned char b, unsigned char v){
	if (v) PORTB |= (1 << b);
	else PORTB &= ~(1 << b);
}

void delay_ms(int miliSec) //for 8 Mhz crystal
{
	int i,j;
	for(i=0;i<miliSec;i++)
	for(j=0;j<775;j++) {
		asm("nop");
	}
}

void shift_98_bits(){
	for(int i=1; i>-1; i--){
		if (cmd_nled){
			digitalWrite(SOUT, HIGH);
			digitalWrite(SCLK,LOW);
			digitalWrite(SCLK,HIGH);
			//Serial.print(1, BIN);
			} else {
			digitalWrite(SOUT, LOW);
			digitalWrite(SCLK,LOW);
			digitalWrite(SCLK,HIGH);
			//Serial.print(0, BIN);
		}
		for(int j=2; j>-1; j--){
			for(int k=15; k>-1; k--){
				digitalWrite(SOUT, bitRead(din_w[i][j], k));
				digitalWrite(SCLK,LOW);
				digitalWrite(SCLK,HIGH);
				//Serial.print(bitRead(din_w[i][j], k), BIN);
			}
		}
	}
	digitalWrite(SCLK,LOW);
	digitalWrite(SOUT, LOW);
	digitalWrite(LAT,HIGH);
	digitalWrite(LAT,LOW);
	//Serial.print("\n");
}

void load_ctrl_regs(){
	cmd_nled = 1;
	//               47-40         32-31
	din_w[1][2]= (WRTCMD<<8) | (PSMODE>>1);
	//              32-31         30-29           28-26        25-24         23-17        16
	din_w[1][1]= (PSMODE<<31) | (LSDVLT<<29) | (LODVLT<<26)| (SIDLD<<24) | (BCB0<<17) | (BCG1>>5);
	//             16-10        9-3       2-0
	din_w[1][0]= (BCG1<<10) | (BCR1<<3) | MC1;
	//               47-40         32-31
	din_w[0][2]= (WRTCMD<<8) | (PSMODE>>1);
	//              32-31         30-29           28-26        25-24         23-17        16
	din_w[0][1]= (PSMODE<<31) | (LSDVLT<<29) | (LODVLT<<26)| (SIDLD<<24) | (BCB0<<17) | (BCG0>>5);
	//             16-10        9-3       2-0
	din_w[0][0]= (BCG0<<10) | (BCR0<<3) | MC0;
	shift_98_bits();
}

void cube_init(){
	////Serial.begin(115200);
	//  pinMode(BLANK, OUTPUT);
	//  digitalWrite(BLANK,LOW);
	//  pinMode(LAT, OUTPUT);
	//  digitalWrite(LAT,LOW);
	//  pinMode(SOUT, OUTPUT);
	//  digitalWrite(SOUT,LOW);
	////  pinMode(SIN, INPUT);
	//  pinMode(SCLK, OUTPUT);
	//  digitalWrite(SCLK,LOW);
	
	DDRB = 0x1D;
	PORTB = 0xE2; // PORTB set to output, outputs init 0s
	
	load_ctrl_regs();
	load_ctrl_regs();
	delay_ms(10);
}

void load_led_on_offs(long floor_sel){
	int x,y,z;
	cmd_nled = 0;
	if      (bitRead(floor_sel, 0)) z = 0;
	else if (bitRead(floor_sel, 1)) z = 1;
	else if (bitRead(floor_sel, 2)) z = 2;
	else if (bitRead(floor_sel, 3)) z = 3;
	else                            z = 4;
	for (y=0; y<5; y++){
		for (x=0; x<5; x++){
			for (int bit_sel=0; bit_sel<3; bit_sel++){
				din_w[bit_map[y][x][3*bit_sel+2]][bit_map[y][x][3*bit_sel+1]] = bitWrite(din_w[bit_map[y][x][3*bit_sel+2]][bit_map[y][x][3*bit_sel+1]], bit_map[y][x][3*bit_sel], bitRead(led_matrix[z][y][x], bit_sel));
			}
		}
	}
	for (int bit_sel=11; bit_sel<16; bit_sel++){
		din_w[1][1] = bitWrite(din_w[1][1], bit_sel, bitRead(floor_sel, bit_sel-11));
	}
	digitalWrite(BLANK,HIGH);
	shift_98_bits();
	digitalWrite(BLANK,LOW);
}

void shape_generator(unsigned char z_gen, unsigned char y_gen, unsigned char x_gen, unsigned char shape ) {
	unsigned char moveDown1 = x_gen-1;
	unsigned char moveDown2 = x_gen-2;
	unsigned char moveUp1 = x_gen+1;
	unsigned char moveUpSide = y_gen+1;
	//for()shape_matrix[z_gen][y_gen][x_gen] = BLACK;
	//unsigned char moveDownSide = y_gen-1;
	// .... shape
	if(shape == 4) {
		unsigned color =CYAN;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;	 
		shape_matrix[z_gen][y_gen][moveUp1] = color;	 
		shape_matrix[z_gen][y_gen][moveDown2] = color;	 
	// .:.	shape
	} else if(shape == 2) {
		unsigned color = MAGENTA;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;
		shape_matrix[z_gen][y_gen][moveUp1] = color;
		shape_matrix[z_gen][moveUpSide][x_gen] = color;
	// ..: shape
	} else if(shape == 3) {
		unsigned color = BLUE;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;
		shape_matrix[z_gen][moveUpSide][moveDown1] = color;
		shape_matrix[z_gen][y_gen][moveUp1] = color;
	// :.. shape
	} else if(shape == 1) {
		unsigned color = YELLOW;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][y_gen][moveUp1] = color;
		shape_matrix[z_gen][moveUpSide][moveUp1] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;
	// .:* shape
	} else if(shape == 5) {
	unsigned color = GREEN;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][moveUpSide][x_gen] = color;
		shape_matrix[z_gen][moveUpSide][moveUp1] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;
	// *:. shape
	} else if(shape == 6) {
		unsigned color = RED;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][moveUpSide][x_gen] = color;
		shape_matrix[z_gen][moveUpSide][moveDown1] = color;
		shape_matrix[z_gen][y_gen][moveUp1] = color;
	}
};

unsigned char check_collide() {
	unsigned char is_collide = 0;
	for (unsigned char z=0; z<5; z++) {
		for (unsigned char y=0; y<5; y++) {
			for (unsigned char x=0; x<5; x++) {
				if(shape_matrix[z][y][x] == store_matrix[z+1][y][x]) {
					unsigned char is_collide = 1;
				}
			}
		}
	}
	return is_collide;
}

unsigned char popFloor(unsigned char z) {
	unsigned char x = 0;
	unsigned char y = 0;
	unsigned char full_floor = 1;	
	for ( y=0; y<5; y++) {
		for ( x=0; x<5; x++) {
			if (store_matrix[z][y][x] == BLACK) {
				full_floor = 0;	
			}
		}
	}	
	if(full_floor) {
			for ( y=0; y<5; y++) {
				for ( x=0; x<5; x++) {
					store_matrix[z][y][x] == BLACK;
				}
			}
//	if(z!=0) {
//		unsigned char z_axis;
//		for ( z_axis=0; z_axis<z-1; z_axis++){
//			shape_matrix[z][y][x] = 
//		}
//	}	
	
}

//moves down every x and y in store_matrix
void move_down(unsigned char z) {
	if(z > 0) {
		unsigned char internal_z = 0;
		for ( internal_z = 0; internal_z<z-1; internal_z++){
			for ( y=0; y<5; y++) {
				for ( x=0; x<5; x++) {
					store_matrix[internal_z][y][x] = store_matrix[internal_z-1][y][x];
				}
			}
		}
	}
}

void tetrix() {
	srand(time(0));
	unsigned char setY = 2; // y axis location shape is generated
	unsigned char setX = 2; // x axis location shape is generated
	//unsigned char randomColor= 0;
	unsigned char x = 0;  // x-axis
	unsigned char y = 0;  // y-axis
	unsigned char z = 0;  // z-axis
	unsigned char shape = 0; // Determines shape generated. Inputed into shape_generator function
	for (int j=0; j<75; j++){
		if (cntr==5){
			cntr--;
		} else if (cntr==0){
			shape = rand()%6+1; // random number for random shape to generate
			//randomX = rand()%2 + 2;
			//randomY = rand()%4;
			//randomColor = rand()%7 + 1;
			cntr=5; // 5 count for 5 floors
			cntr--;
		} else cntr--;
		//long color = BLACK;
		//if(cntr == 0) color = randomColor;
		//if(cntr == 1) color = randomColor;
		//if(cntr == 2) color = randomColor;
		//if(cntr == 3) color = randomColor;
		//if(cntr == 4) color = randomColor;
		//if(cntr == 5) color = randomColor;
		for ( z=0; z<5; z++){
			for ( y=0; y<5; y++) {
				for ( x=0; x<5; x++) {
					led_matrix[z][y][x] = BLACK; // refresh LEDs
					shape_matrix[z][y][x] = BLACK;
					if(z == cntr) {
						
						shape_generator(z, setY, setX, shape);
						if((z==0)) {
							store_matrix[z][y][x] = shape_matrix[z][y][x] | store_matrix[z][y][x];
						}
					}	
					led_matrix[z][y][x] = shape_matrix[z][y][x] | store_matrix[z][y][x];
				}
			} 
		}
		for (int i =0; i<100; i++){
			load_led_on_offs(floor_sel);
			delay_ms(1);
			if (bitRead(floor_sel, 4))
				floor_sel = 1;
			else
				floor_sel = floor_sel << 1;
		}
	}
}
