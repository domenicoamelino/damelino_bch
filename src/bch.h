/*
 * bch.h
 *
 *  Created on: 02 dic 2015
 *      Author: domenicoamelino
 */

#ifndef BCH_H_
#define BCH_H_

#define N 15
#define K 5
#define T 3
#define doubleT 6
#include <stdio.h>
#include <stdlib.h>
#include "lfsr.h"

void encode(lfsr shift_reg,uint8_t* gen_poly,uint8_t* msg,uint8_t* result);
int syndrome_calc(uint8_t * received,int * alpha,int i);
int decode(uint8_t * received,int * alpha,uint8_t * result);
int bma(int * alpha, int * syndr, int sigma[doubleT+1][doubleT+1],int * k);
#endif /* BCH_H_ */

