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
long BCR0 = 35;
long BCG0 = 14;
long BCB0 = 127;
long MC1 = 4;
long BCR1 = 35;
long BCG1 = 14;
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
unsigned char new_shape = 1;

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

void delay(int miliSec) //for 8 Mhz crystal
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
	srand(time(0));
	DDRB = 0x1D;
	PORTB = 0xE2; // PORTB set to output, outputs init 0s
	
	load_ctrl_regs();
	load_ctrl_regs();
	delay(10);
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

// first 3 inputs change position. shape picks which of the 6 shapes to generate.
void shape_generator(unsigned char z_gen, unsigned char y_gen, unsigned char x_gen, unsigned char shape, unsigned char color ) {
	unsigned char moveDown1 = x_gen-1;
	unsigned char moveDown2 = x_gen-2;
	unsigned char moveUp1 = x_gen+1;
	unsigned char moveSide = y_gen+1;
	//for()shape_matrix[z_gen][y_gen][x_gen] = BLACK;
	//unsigned char moveDownSide = y_gen-1;
	// .... shape
	if(shape == 0) {
		//unsigned char randomColor = 0;
		//randomColor = rand()%7 + 1;
		//store_matrix[z_gen-1][y_gen][x_gen] = BLACK;
		shape_matrix[z_gen][y_gen][x_gen] = color;
	} else if(shape == 4) {
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
		shape_matrix[z_gen][moveSide][x_gen] = color;
	// ..: shape
	} else if(shape == 3) {
		unsigned color = BLUE;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;
		shape_matrix[z_gen][moveSide][moveDown1] = color;
		shape_matrix[z_gen][y_gen][moveUp1] = color;
	// :.. shape
	} else if(shape == 1) {
		unsigned color = YELLOW;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][y_gen][moveUp1] = color;
		shape_matrix[z_gen][moveSide][moveUp1] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;
	// .:* shape
	} else if(shape == 5) {
	unsigned color = GREEN;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][moveSide][x_gen] = color;
		shape_matrix[z_gen][moveSide][moveUp1] = color;
		shape_matrix[z_gen][y_gen][moveDown1] = color;
	// *:. shape
	} else if(shape == 6) {
		unsigned color = RED;
		shape_matrix[z_gen][y_gen][x_gen] = color;
		shape_matrix[z_gen][moveSide][x_gen] = color;
		shape_matrix[z_gen][moveSide][moveDown1] = color;
		shape_matrix[z_gen][y_gen][moveUp1] = color;
	}
};

// checks for collision between falling object and next LEDs
unsigned char check_collide() {
	unsigned char is_collide = 0;
	for (unsigned char z=0; z<4; z++) {
		for (unsigned char y=0; y<5; y++) {
			for (unsigned char x=0; x<5; x++) {
				if((store_matrix[z][y][x] != BLACK) && (shape_matrix[z+1][y][x] != BLACK)) {
					is_collide = 1;
				}
				if((z == 0) && (shape_matrix[z][y][x] != BLACK)) {
					is_collide = 1;
				}
			}
		}
	}
	return is_collide;
}

// if floor is full on x & y axis, pops out floor
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
					store_matrix[z][y][x] = BLACK;
				}
			}
//	if(z!=0) {
//		unsigned char z_axis;
//		for ( z_axis=0; z_axis<z-1; z_axis++){
//			shape_matrix[z][y][x] = 
	}	
	return full_floor;
}

//moves down every x and y in store_matrix
void move_down() {
	for (unsigned char z = 0; z<5; z++){
		for (unsigned char y=0; y<5; y++) {
			for (unsigned char x=0; x<5; x++) {
				if (z == 4) shape_matrix[z][y][x] = BLACK;
				else shape_matrix[z][y][x] = shape_matrix[z+1][y][x];
			}
		}
	}
}

void clr_cube(){
	for (unsigned char z = 0; z<5; z++){
		for (unsigned char y=0; y<5; y++) {
			for (unsigned char x=0; x<5; x++) {
				store_matrix[z][y][x] = BLACK;
				shape_matrix[z][y][x] = BLACK;
			}
		}
	}
	new_shape = 1;
};

void shape_to_store(){
	for (unsigned char  z=0; z<5; z++){
		for (unsigned char  y=0; y<5; y++) {
			for (unsigned char  x=0; x<5; x++) {
				if (shape_matrix[z][y][x] != BLACK) {
					store_matrix[z][y][x] = shape_matrix[z][y][x];
					shape_matrix[z][y][x] = BLACK;
				}
			} // x iterator
		} // y iterator
	} //z iterator
};

unsigned char is_floor_full(){//checks if floor is full
	unsigned char not_full = 0x00; 
	for (unsigned char  z=0; z<5; z++){
		for (unsigned char  y=0; y<5; y++) {
			for (unsigned char  x=0; x<5; x++) {
				if (shape_matrix[z][y][x] == BLACK) {
					not_full |= 1 << z;
				}
			} // x iterator
		} // y iterator
	} //z iterator
	return (~not_full) & 0x1f;
};


unsigned char tetrix() {
	unsigned char shpe_ok = 1;
	unsigned char setY = 2; // y axis location shape is generated
	unsigned char setX = 2; // x axis location shape is generated
	unsigned char randomColor= 1;
	unsigned char x = 0;  // x-axis
	unsigned char y = 0;  // y-axis
	unsigned char z = 0;  // z-axis
	unsigned char floor_to_clear = 0;
	unsigned char randomShape = 0; // Determines shape generated. Inputed into shape_generator function
	if (new_shape){
		randomShape = rand()%6 + 1;
		randomColor = rand()%7 + 1;
		//setX = rand()%5;
		//setY = rand()%5;
		if (store_matrix[4][setX][setY] == BLACK) shape_generator(4, setY, setX, randomShape, randomColor);
		else shpe_ok = 0;
		new_shape = 0;
	} else {
		if (check_collide()){
			new_shape = 1;
			shape_to_store();
			if (floor_to_clear = is_floor_full()){
				if (floor_to_clear & 1) popFloor(0);
				else if (floor_to_clear & 2) popFloor(1);
				else if (floor_to_clear & 4) popFloor(2);
				else if (floor_to_clear & 8) popFloor(3);
				else popFloor(4);
			}
		} else {
			move_down();
		}
	}
	
	for ( z=0; z<5; z++){
		for ( y=0; y<5; y++) {
			for ( x=0; x<5; x++) {
				led_matrix[z][y][x] = shape_matrix[z][y][x] | store_matrix[z][y][x];
			} // x iterator
		} // y iterator
	} //z iterator
	for (int i =0; i<250; i++){
		load_led_on_offs(floor_sel);
		delay(1);
		if (bitRead(floor_sel, 4))
			floor_sel = 1;
		else
			floor_sel = floor_sel << 1;
	}
	return shpe_ok;	
}
