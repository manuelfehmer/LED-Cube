#include "cube.h"


void set_cube(uint8_t color,uint8_t value)
{
	for(int set_x=0;set_x<=8;set_x++)
		for(int set_y=0;set_y<=8;set_y++)
			for(int set_z=0;set_z<=8;set_z++)
				LED[color][set_z][set_x][set_y]=value;	
}

void tmpsetcube(uint8_t color,uint8_t value)
{
	for(int set_x=0;set_x<=8;set_x++)
		for(int set_y=0;set_y<=8;set_y++)
			for(int set_z=0;set_z<=8;set_z++)
				LED_tmp[color][set_z][set_x][set_y]=value;	
}

void setvoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z,uint8_t value)
{
	LED[color][z][y][x] = value;
}

void tmpsetvoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z,uint8_t value)
{
	LED_tmp[color][z][y][x] = value;
}

uint8_t getvoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z)
{
	if (LED[color][z][y][x]<=100)
		return 1;
	 else
		return 0;
}

uint8_t tmpgetvoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z)
{
	if (LED_tmp[color][z][y][x]<=100)
		return 1;
	 else
		return 0;
}

void altervoxel(uint8_t color, uint8_t x, uint8_t y, uint8_t z, uint8_t state)
{
	if (state == 1)
	{
		setvoxel(color,x,y,z,0);
	} else
	{
		setvoxel(color,x,y,z,0xff);
	}
}

void shift (uint8_t color, char axis, int direction)
{
	int i, x ,y;
	int ii, iii;
	int state;

	for (i = 0; i < 8; i++)
	{
		if (direction == -1)
		{
			ii = i;
		} else
		{
			ii = (7-i);
		}	
	
	
		for (x = 0; x < 8; x++)
		{
			for (y = 0; y < 8; y++)
			{
				if (direction == -1)
				{
					iii = ii+1;
				} else
				{
					iii = ii-1;
				}
				
				if (axis == AXIS_Z)
				{
					state = getvoxel(color,x,y,iii);
					altervoxel(color,x,y,ii,state);
				}
				
				if (axis == AXIS_Y)
				{
					state = getvoxel(color,x,iii,y);
					altervoxel(color,x,ii,y,state);
				}
				
				if (axis == AXIS_X)
				{
					state = getvoxel(color,iii,y,x);
					altervoxel(color,ii,y,x,state);
				}
			}
		}
	}
	
	if (direction == -1)
	{
		i = 7;
	} else
	{
		i = 0;
	}	
	
	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{
			if (axis == AXIS_Z)
				setvoxel(color,x,y,i,0x00);
				
			if (axis == AXIS_Y)
				setvoxel(color,x,i,y,0x00);
			
			if (axis == AXIS_X)
				setvoxel(color,i,y,x,0x00);
		}
	}
}
void effect_planboing (uint8_t color, uint8_t plane, int speed)
{
	int i;
	for (i=0;i<8;i++)
	{
		set_cube(color,0);
        setplane(color,plane, i);
		_delay_ms(speed);
	}
	
	for (i=7;i>=0;i--)
	{
		set_cube(color,0);
        setplane(color,plane,i);
		_delay_ms(speed);
	}
}

void effect_stringfly2(uint8_t color, char * str)
{
	int x,y,i;
	unsigned char chr[5];
	
	while (*str)
	{
		font_getchar(*str++, chr);
		
        // Put a character on the back of the cube
		for (x = 0; x < 5; x++)
		{
			for (y = 0; y < 8; y++)
			{
				if ((chr[x] & (0x80>>y)))
				{
					setvoxel(color,7,x+2,y,0xff);
				}
			}
		}
		
        // Shift the entire contents of the cube forward by 6 steps
        // before placing the next character
		for (i = 0; i<6; i++)
		{
			_delay_ms(30);
            shift(color,AXIS_X,-1);
		}
	}
    // Shift the last character out of the cube.
	for (i = 0; i<8; i++)
	{
		_delay_ms(50);
        shift(color,AXIS_X,-1);
	}
	
}

void effect_rain (uint8_t color, uint16_t iterations)
{
	int i, ii;
	int rnd_x;
	int rnd_y;
	int rnd_num;
	
	for (ii=0;ii<iterations;ii++)
	{
		rnd_num = rand()%6;
		
		for (i=0; i < rnd_num;i++)
		{
			rnd_x = rand()%8;
			rnd_y = rand()%8;
			setvoxel(color,rnd_x,rnd_y,0,0xff);
		}
		
		_delay_ms(20);
		shift(color,AXIS_Z,1);
	}
}
void effect_blinky2(uint8_t color) // blinky power on
{
	int i;
	set_cube(color,0);
		i = 75;
		while (i>0)
		{
			set_cube(color,0);
			_delay_ms(i);
			
			set_cube(color,0xff);
			_delay_ms(10);
			
			i = i - (15+(100/(i)));
		}
		
		_delay_ms(100);

}//edit 
void effect_box_shrink_grow (uint8_t color, uint8_t rot, uint8_t flip, uint8_t delay)
{
	uint8_t xyz;
	for (uint8_t i=0;i<16;i++)
	{
           xyz = 7-i; // This reverses counter i between 0 and 7.
           if (i > 7) 
               xyz = i-8; // at i > 7, i 8-15 becomes xyz 0-7.
        cli();
		box_wireframe(color,0,0,0,xyz,xyz,xyz,0xff);
		if (flip > 0) // upside-down
        	mirror(color,AXIS_Z);
        if (rot == 1 || rot == 3)
            mirror(color,AXIS_Y);
        if (rot == 2 || rot == 3)
            mirror(color,AXIS_X);
		sei();
		_delay_ms(delay);
		set_cube(color,0);
    }
	
	set_cube(color,0);
}


void effect_box_woopwoop (uint8_t color,int delay, int grow)
{
	uint8_t ii;
	
	set_cube(color,0);
	for (uint8_t i=0;i<4;i++)
	{
        ii =i;
        if (grow > 0)
            ii = 3-i;
		box_wireframe(color,(4-ii),(4-ii),(4-ii),(3+ii),(3+ii),(3+ii),0xff);
		box_wireframe(color,(3-ii),(3-ii),(3-ii),(4+ii),(4+ii),(4+ii),0);
		_delay_ms(delay);
	}

}

void effect_wormsqueeze (uint8_t color, uint8_t size, uint8_t axis, uint8_t direction, int iterations, int delay)
{
	int x, y, i,j,k, dx, dy;
	int cube_size;
	int origin = 0;
	
	if (direction == -1)
		origin = 7;
	
	cube_size = 8-(size-1);
	
	x = rand()%cube_size;
	y = rand()%cube_size;
	
	for (i=0; i<iterations; i++)
	{
		dx = ((rand()%3)-1);
		dy = ((rand()%3)-1);
	
		if ((x+dx) > 0 && (x+dx) < cube_size)
			x += dx;
			
		if ((y+dy) > 0 && (y+dy) < cube_size)
			y += dy;
	
		shift(color, axis, direction);
		

		for (j=0; j<size;j++)
		{
			for (k=0; k<size;k++)
			{
				if (axis == AXIS_Z)
					setvoxel(color,x+j,y+k,origin,0xff);
					
				if (axis == AXIS_Y)
					setvoxel(color,x+j,origin,y+k,0xff);
					
				if (axis == AXIS_X)
					setvoxel(color,origin,y+j,x+k,0xff);
			}
		}
		
		_delay_ms(delay);
	}
}
int effect_telcstairs_do(uint8_t color, uint8_t x, uint8_t val, int delay)
{
	int y,z;

	for(y = 0, z = x; y <= z; y++, x--)
	{
		if(x < 8 && y < 8)
		{
			LED[color][x][y][val%8] = 0xff;
		}
	}
	_delay_ms(delay);
	return z;
}

void effect_telcstairs (uint8_t color, uint8_t invert, int delay, uint8_t val)
{
	int x;

	if(invert)
	{
		for(x = 8*2; x >= 0; x--)
		{
			x = effect_telcstairs_do(color,x,val,delay);
		}
	}
	else
	{
		for(x = 0; x < 8*2; x++)
		{
			x = effect_telcstairs_do(color,x,val,delay);
		}
	}
}
// Send a voxel flying from one side of the cube to the other
// If its at the bottom, send it to the top..
void sendvoxel_z (uint8_t color, unsigned char x, unsigned char y, unsigned char z, int delay)
{
	uint8_t ii;
	for (uint8_t i=0; i<8; i++)
	{
		if (z == 7)
		{
			ii = 7-i;
			setvoxel(color,x,y,ii+1,0);
		} else
		{
			ii = i;
			setvoxel(color,x,y,ii-1,0);
		}
		setvoxel(color,x,y,ii,0xff);
		_delay_ms(delay);
	}
}
void sendvoxels_rand_z (uint8_t color, int iterations, int delay, uint8_t wait)
{
	unsigned char x, y, last_x = 0, last_y = 0, i;

	set_cube(color,0);

	// Loop through all the X and Y coordinates
	for (x=0;x<8;x++)
	{
		for (y=0;y<8;y++)
		{
			// Then set a voxel either at the top or at the bottom
            // rand()%2 returns either 0 or 1. multiplying by 7 gives either 0 or 7.
            setvoxel(color,x,y,((rand()%2)*7),0xff);
		}
	}

	for (i=0;i<iterations;i++)
	{
		// Pick a random x,y position
		x = rand()%8;
		y = rand()%8;
		// but not the sameone twice in a row
		if (y != last_y && x != last_x)
		{
			// If the voxel at this x,y is at the bottom
			if (getvoxel(color,x,y,0))
			{
				// send it to the top
				sendvoxel_z(color,x,y,7,delay);
			} else
			{
				// if its at the top, send it to the bottom
				sendvoxel_z(color,x,y,0,delay);
			}
			_delay_ms(wait);
			
			// Remember the last move
			last_y = y;
			last_x = x;
		}
	}

}
void box_wireframe(uint8_t color, uint8_t x1, uint8_t y1, uint8_t z1, uint8_t x2, uint8_t y2, uint8_t z2,uint8_t value)
{
	// Lines along X axis
	for (uint8_t ix=x1;ix<=x2;ix++)
	{
		setvoxel(color,ix,y1,z1,value);
		setvoxel(color,ix,y1,z2,value);
		setvoxel(color,ix,y2,z1,value);
		setvoxel(color,ix,y2,z2,value);
	}
	// Lines along Y axis
	for (uint8_t iy=y1;iy<=y2;iy++)
	{
		setvoxel(color,x1,iy,z1,value);
		setvoxel(color,x1,iy,z2,value);
		setvoxel(color,x2,iy,z1,value);
		setvoxel(color,x2,iy,z2,value);
	}
	// Lines along Z axis
	for (uint8_t iz=z1;iz<=z2;iz++)
	{
		setvoxel(color,x1,y1,iz,value);
		setvoxel(color,x1,y2,iz,value);
		setvoxel(color,x2,y1,iz,value);
		setvoxel(color,x2,y2,iz,value);
	}
} 


void setplane(uint8_t color, uint8_t plane, uint8_t level)
{
	int i;
	for (i=0;i<8;i++)
		for(int j=0;j<8;j++)
		{		
			switch (plane)
			{
			case AXIS_Z:	LED[color][level%8][i][j] = 0xff;
					break;
			case AXIS_Y:	LED[color][i][level%8][j] = 0xff;
					break;
			case AXIS_X:	LED[color][i][j][level%8] = 0xff;
					break;
			default: break;
			}
		}	
}

void mirror (uint8_t color, uint8_t axis)
{
    unsigned char temp;
	for (uint8_t i=0; i<4; i++)
	   	for (uint8_t j=0; j<8; j++)
	       	for (uint8_t k=0; k<8; k++)
	       	{
				switch (axis)
				{
				case AXIS_Z:temp=LED[color][i][j][k];
				           	LED[color][i][j][k]=LED[7-i][j][k];
							LED[color][7-i][j][k]=temp;
							break;
				case AXIS_Y:temp=LED[color][j][i][k];
				           	LED[color][j][i][k]=LED[color][j][7-i][k];
							LED[color][j][7-i][k]=temp;
							break;
				case AXIS_X:temp=LED[color][k][j][i];
				           	LED[color][k][j][i]=LED[color][k][j][7-i];
							LED[color][k][j][7-i]=temp;
				default:	break;
				}
			}
}

// Set n number of voxels at random positions
void effect_random_sparkle_flash (uint8_t color, int iterations, int voxels,int value, int delay)
{
	int i;
	int v;
	for (i = 0; i < iterations; i++)
	{
		for (v=0;v<=voxels;v++)
			setvoxel(color,rand()%8,rand()%8,rand()%8,value);
			
		_delay_ms(delay);
		set_cube(color,0);
	}
}

// blink 1 random voxel, blink 2 random voxels..... blink 20 random voxels
// and back to 1 again.
void effect_random_sparkle (uint8_t color, int iterations, int max_voxels, int delay)
{
	int i;
	
	for (i=1;i<max_voxels;i++)
	{
		effect_random_sparkle_flash(color,iterations,i,0xff,delay);
	}
	
	for (i=20;i>=1;i--)
	{
		effect_random_sparkle_flash(color,5,i,0xff,delay);
	}
	
}
