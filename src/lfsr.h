/*
 * lfrs.h
 *
 *  Created on: 02 dic 2015
 *      Author: domenicoamelino
 */

#define SIZE 10
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t lfsr [SIZE];

int init_lfsr(lfsr shift_reg);
int insert_element(lfsr shift_reg,uint8_t elem);
int remainder_insert(lfsr shifg_reg,uint8_t * gen_poly,uint8_t elem);
int parallel_insert(uint8_t* input,lfsr shift_reg);
