#include "cube.h"


void clear_cube(uint8_t value)
{
	for(int clear_x=0;clear_x<=8;clear_x++)
		for(int clear_y=0;clear_y<=8;clear_y++)
			for(int clear_z=0;clear_z<=8;clear_z++)
				LED[clear_z][clear_x][clear_y]=value;	
}
void setvoxel(uint8_t x, uint8_t y, uint8_t z,uint8_t value)
{
	LED[z][y][x] = value;
}
uint8_t getvoxel(int x, int y, int z)
{

	if (LED[z][y][x]<=100)
		return 1;
	 else
		return 0;

}
void altervoxel(int x, int y, int z, int state)
{
	if (state == 1)
	{
		setvoxel(x,y,z,0);
	} else
	{
		setvoxel(x,y,z,0xff);
	}
}


void shift (char axis, int direction)
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
					state = getvoxel(x,y,iii);
					altervoxel(x,y,ii,state);
				}
				
				if (axis == AXIS_Y)
				{
					state = getvoxel(x,iii,y);
					altervoxel(x,ii,y,state);
				}
				
				if (axis == AXIS_X)
				{
					state = getvoxel(iii,y,x);
					altervoxel(ii,y,x,state);
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
				setvoxel(x,y,i,0xff);
				
			if (axis == AXIS_Y)
				setvoxel(x,i,y,0xff);
			
			if (axis == AXIS_X)
				setvoxel(i,y,x,0xff);
		}
	}
}
void effect_planboing (int plane, int speed)
{
	int i;
	for (i=0;i<8;i++)
	{
		clear_cube(0xff);
        setplane(plane, i);
		_delay_ms(speed);
	}
	
	for (i=7;i>=0;i--)
	{
		clear_cube(0xff);
        setplane(plane,i);
		_delay_ms(speed);
	}
}

void effect_stringfly2(char * str)
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
					setvoxel(7,x+2,y,0x00);
				}
			}
		}
		
        // Shift the entire contents of the cube forward by 6 steps
        // before placing the next character
		for (i = 0; i<6; i++)
		{
			_delay_ms(30);
            shift(AXIS_X,-1);
		}
	}
    // Shift the last character out of the cube.
	for (i = 0; i<8; i++)
	{
		_delay_ms(50);
        shift(AXIS_X,-1);
	}
	
}
void effect_rain (uint16_t iterations)
{
	int i, ii;
	int rnd_x;
	int rnd_y;
	int rnd_num;
	
	for (ii=0;ii<iterations;ii++)
	{
		rnd_num = rand()%4;
		
		for (i=0; i < rnd_num;i++)
		{
			rnd_x = rand()%8;
			rnd_y = rand()%8;
			setvoxel(rnd_x,rnd_y,7,0);
		}
		
		_delay_ms(30);
		shift(AXIS_Z,-1);
	}
}
void effect_blinky2()
{
	int i;
	clear_cube(0xff);
	
	for (uint8_t r=0;r<2;r++)
	{
		i = 75;
		while (i>0)
		{
			clear_cube(0xff);
			_delay_ms(i);
			
			clear_cube(0x00);
			_delay_ms(10);
			
			i = i - (15+(100/(i)));
		}
		
		_delay_ms(100);
		
		i = 75;
		while (i>0)
		{
			clear_cube(0xff);
			_delay_ms(74-i);
			
			clear_cube(0x00);
			_delay_ms(10);
			
			i = i - (15+(100/(i)));
		}
	}

}
void effect_box_shrink_grow (uint8_t rot, uint8_t flip, uint8_t delay)
{
	uint8_t xyz;
	for (uint8_t i=0;i<16;i++)
	{
           xyz = 7-i; // This reverses counter i between 0 and 7.
           if (i > 7) 
               xyz = i-8; // at i > 7, i 8-15 becomes xyz 0-7.
        cli();
		box_wireframe(0,0,0,xyz,xyz,xyz,0);
		if (flip > 0) // upside-down
        	mirror(AXIS_Z);
        if (rot == 1 || rot == 3)
            mirror(AXIS_Y);
        if (rot == 2 || rot == 3)
            mirror(AXIS_X);
		sei();
		_delay_ms(delay);
		clear_cube(0xff);
    }
	
	clear_cube(0xff);
}


void effect_box_woopwoop (int delay, int grow)
{
	uint8_t ii;
	
//	clear_cube(0xff);
	for (uint8_t i=0;i<4;i++)
	{
        ii =i;
        if (grow > 0)
            ii = 3-i;
		box_wireframe((4-ii),(4-ii),(4-ii),(3+ii),(3+ii),(3+ii),0xff);
		box_wireframe((3-ii),(3-ii),(3-ii),(4+ii),(4+ii),(4+ii),0x00);
		_delay_ms(delay);
	}

}
void effect_wormsqueeze (int size, int axis, int direction, int iterations, int delay)
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
	
		shift(axis, direction);
		

		for (j=0; j<size;j++)
		{
			for (k=0; k<size;k++)
			{
				if (axis == AXIS_Z)
					setvoxel(x+j,y+k,origin,0);
					
				if (axis == AXIS_Y)
					setvoxel(x+j,origin,y+k,0);
					
				if (axis == AXIS_X)
					setvoxel(origin,y+j,x+k,0);
			}
		}
		
		_delay_ms(delay);
	}
}
int effect_telcstairs_do(int x, int val, int delay)
{
	int y,z;

	for(y = 0, z = x; y <= z; y++, x--)
	{
		if(x < 8 && y < 8)
		{
			LED[x][y][val%8] = 0;
		}
	}
	_delay_ms(delay);
	return z;
}

void effect_telcstairs (int invert, int delay, int val)
{
	int x;

	if(invert)
	{
		for(x = 8*2; x >= 0; x--)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
	else
	{
		for(x = 0; x < 8*2; x++)
		{
			x = effect_telcstairs_do(x,val,delay);
		}
	}
}
// Send a voxel flying from one side of the cube to the other
// If its at the bottom, send it to the top..
void sendvoxel_z (unsigned char x, unsigned char y, unsigned char z, int delay)
{
	uint8_t ii;
	for (uint8_t i=0; i<8; i++)
	{
		if (z == 7)
		{
			ii = 7-i;
			setvoxel(x,y,ii+1,0xff);
		} else
		{
			ii = i;
			setvoxel(x,y,ii-1,0xff);
		}
		setvoxel(x,y,ii,0);
		_delay_ms(delay);
	}
}
void sendvoxels_rand_z (int iterations, int delay, int wait)
{
	unsigned char x, y, last_x = 0, last_y = 0, i;

	clear_cube(0xff);

	// Loop through all the X and Y coordinates
	for (x=0;x<8;x++)
	{
		for (y=0;y<8;y++)
		{
			// Then set a voxel either at the top or at the bottom
            // rand()%2 returns either 0 or 1. multiplying by 7 gives either 0 or 7.
            setvoxel(x,y,((rand()%2)*7),0);
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
			if (getvoxel(x,y,0))
			{
				// send it to the top
				sendvoxel_z(x,y,0,delay);
			} else
			{
				// if its at the top, send it to the bottom
				sendvoxel_z(x,y,7,delay);
			}
			_delay_ms(wait);
			
			// Remember the last move
			last_y = y;
			last_x = x;
		}
	}

}
void box_wireframe(int x1, int y1, int z1, int x2, int y2, int z2,int value)
{
	// Lines along X axis
	

	for (uint8_t ix=x1;ix<=x2;ix++)
	{
		setvoxel(ix,y1,z1,value);
		setvoxel(ix,y1,z2,value);
		setvoxel(ix,y2,z1,value);
		setvoxel(ix,y2,z2,value);
	}
	// Lines along Y axis
	for (uint8_t iy=y1;iy<=y2;iy++)
	{
		setvoxel(x1,iy,z1,value);
		setvoxel(x1,iy,z2,value);
		setvoxel(x2,iy,z1,value);
		setvoxel(x2,iy,z2,value);
	}

	// Lines along Z axis
	for (uint8_t iz=z1;iz<=z2;iz++)
	{
		setvoxel(x1,y1,iz,value);
		setvoxel(x1,y2,iz,value);
		setvoxel(x2,y1,iz,value);
		setvoxel(x2,y2,iz,value);
	}

} 


void setplane(uint8_t plane, uint8_t level)
{
	int i;
	for (i=0;i<8;i++)
		for(int j=0;j<8;j++)
		{		
			switch (plane)
			{
			case AXIS_Z:	LED[level%8][i][j] = 0x00;
					break;
			case AXIS_Y:	LED[i][level%8][j] = 0x00;
					break;
			case AXIS_X:	LED[i][j][level%8] = 0x00;
					break;
			default: break;
			}
		}	
}

void mirror (uint8_t axis)
{
    unsigned char temp;
	for (uint8_t i=0; i<4; i++)
	   	for (uint8_t j=0; j<8; j++)
	       	for (uint8_t k=0; k<8; k++)
	       	{
				switch (axis)
				{
				case AXIS_Z:temp=LED[i][j][k];
				           	LED[i][j][k]=LED[7-i][j][k];
							LED[7-i][j][k]=temp;
							break;
				case AXIS_Y:temp=LED[j][i][k];
				           	LED[j][i][k]=LED[j][7-i][k];
							LED[j][7-i][k]=temp;
							break;
				case AXIS_X:temp=LED[k][j][i];
				           	LED[k][j][i]=LED[k][j][7-i];
							LED[k][j][7-i]=temp;
				default:	break;
				}
			}
}