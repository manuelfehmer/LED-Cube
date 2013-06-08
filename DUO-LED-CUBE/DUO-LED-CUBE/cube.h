#ifndef CUBE_H
	#define CUBE_H
	#include "main.h"
	#include "font.h"
	#include <math.h>
	#include <stdlib.h>
	
	#define AXIS_X 0
	#define AXIS_Y 1
	#define AXIS_Z 2

	void set_cube(uint8_t value);
	void tmpsetcube(uint8_t value);
	uint8_t getvoxel(int x, int y, int z);
	uint8_t tmpgetvoxel(int x, int y, int z);
	void setvoxel(uint8_t x, uint8_t y, uint8_t z,uint8_t value);
	void tmpsetvoxel(uint8_t x, uint8_t y, uint8_t z,uint8_t value);
	void shift (char axis, int direction);
	void effect_stringfly2(char * str);
	void altervoxel(int x, int y, int z, int state);
	void effect_rain (uint16_t iterations);
	void setplane(uint8_t plane, uint8_t level);
	void effect_planboing (int plane, int speed);
	void effect_blinky2();
	void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2,int value);
	void effect_box_shrink_grow (uint8_t rot, uint8_t flip, uint8_t delay);
	void effect_box_woopwoop (int delay, int grow);
	void mirror (uint8_t axis);
	void sendvoxel_z (unsigned char x, unsigned char y, unsigned char z, int delay);
	void sendvoxels_rand_z (int iterations, int delay, int wait);
	void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay);
	int effect_telcstairs_do(int x, int val, int delay);
	void effect_telcstairs (int invert, int delay, int val);
	void effect_random_sparkle_flash (int iterations, int voxels,int value, int delay);
	void effect_random_sparkle (int iterations, int max_voxels, int delay);

	volatile unsigned char LED[8][8][8];	//Ebene //Reihe //Spalte
	volatile unsigned char LED_tmp[8][8][8];	//Ebene //Reihe //Spalte
	volatile unsigned char Ebene_ein;



#endif
