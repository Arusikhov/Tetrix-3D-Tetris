/*
 * cube.h
 *
 * Created: 11/26/2019 9:53:00 PM
 *  Author: Arusyak
 */ 

#ifndef CUBE_H_
#define CUBE_H_

unsigned char bitRead(long d, unsigned char b);
long bitWrite(long d, unsigned char b, unsigned char v);
void digitalWrite(unsigned char b, unsigned char v);
void delay_ms(int miliSec);
void shift_98_bits();
void load_ctrl_regs();
void cube_init();
void load_led_on_offs(long floor_sel);
void teq_hartutjun();
void vert_hartutyun();
void cube_verev_vazox();
void cube_vazox();
void tetrix();

#endif /* CUBE_H_ */