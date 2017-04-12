/*
 * lfrs.c
 *
 *  Created on: 02 dic 2015
 *      Author: domenicoamelino
 */

#include "lfsr.h"

int init_lfsr(lfsr shift_reg)
{
	for(int i=0;i<SIZE;i++)
	{
			shift_reg[i] = 0;
	}

	return 0;
}

int insert_element(lfsr shift_reg,uint8_t elem)
{
	uint8_t buf1 = 0;
	uint8_t buf2 = 0;
	uint8_t out = 0;
	for(int i=0;i<SIZE;i++)
	{
		/* It's a simply swap */

		if (i==0)
		{
			buf1 = shift_reg[i];
			shift_reg[i] = elem;
		}
		else
		{
			buf2 = shift_reg[i];
			shift_reg[i] = buf1;
			buf1 = buf2;
		}
	}

	out=buf1;

	return out;
}

int remainder_insert(lfsr shift_reg,uint8_t * gen_poly,uint8_t elem)
{
	uint8_t buffer;
	uint8_t buffer_xor;
	if(elem==0)
	{
		 insert_element(shift_reg,0);
		 return shift_reg[SIZE-1];
	}
	else
	{
		for(int k=0;k<SIZE;k++)
		{
				if(k==0)
				{

					buffer = shift_reg[0];
					shift_reg[0] = elem;
				}
				else
				{

					if(gen_poly[k]==1)
					{
						buffer_xor = buffer ^ elem;
						buffer = shift_reg[k];
						shift_reg[k] = buffer_xor;
					}
					else
					{

						buffer_xor = buffer;
						buffer = shift_reg[k];
						shift_reg[k] = buffer_xor;
					}
				}
		}
		return shift_reg[SIZE-1];
	}

}

int parallel_insert(uint8_t* input,lfsr shift_reg)
{
	for(int i=0;i<SIZE;i++)
	{
		shift_reg[i] = input [i];
	}
	return 0;
}

