#ifndef CUBE_H
	#define CUBE_H
	#include "main.h"
	//#include "font.h"
	#include <math.h>
	#include <stdlib.h>
	
	#define AXIS_X 0
	#define AXIS_Y 1
	#define AXIS_Z 2

	void set_cube(uint8_t color,uint8_t value);
	void tmpsetcube(uint8_t color,uint8_t value);
	uint8_t getvoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z);
	uint8_t tmpgetvoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z);
	void setvoxel(uint8_t color,uint8_t x, uint8_t y, uint8_t z,uint8_t value);
	void tmpsetvoxel(uint8_t color,uint8_t x, uint8_t y, uint8_t z,uint8_t value);
	void shift (uint8_t color,char axis, int direction);
	void effect_stringfly2(uint8_t color, char * str);
	void altervoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z, uint8_t state);
	void effect_rain (uint8_t color, uint16_t iterations);
	void setplane(uint8_t color, uint8_t plane, uint8_t level);
	void effect_planboing (uint8_t color, uint8_t plane, int speed);
	void effect_blinky2(uint8_t color);
	void box_wireframe(uint8_t color, uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2,uint8_t value);
	void effect_box_shrink_grow (uint8_t color, uint8_t rot, uint8_t flip, uint8_t delay);
	void effect_box_woopwoop (uint8_t color, int delay, int grow);
	void mirror (uint8_t color, uint8_t axis);
	void sendvoxel_z (uint8_t color, unsigned char x, unsigned char y, unsigned char z, int delay);
	void sendvoxels_rand_z (uint8_t color, int iterations, int delay, uint8_t wait);
	void effect_wormsqueeze (uint8_t color, uint8_t size, uint8_t axis, uint8_t direction, int iterations, int delay);
	int effect_telcstairs_do(uint8_t color, uint8_t x, uint8_t val, int delay);
	void effect_telcstairs (uint8_t color, uint8_t invert, int delay, uint8_t val);
	void effect_random_sparkle_flash (uint8_t color, int iterations, int voxels,int value, int delay);
	void effect_random_sparkle (uint8_t color, int iterations, int max_voxels, int delay);

	volatile unsigned char LED[2][8][8][8];	//Ebene //Reihe //Spalte
	volatile unsigned char LED_tmp[2][8][8][8];	//Farbe//Ebene //Reihe //Spalte
	volatile unsigned char Ebene_ein;
	volatile unsigned char input_A;
	volatile unsigned char count;



#endif
