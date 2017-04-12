/*
 * galois.h
 *
 *  Created on: 10 dic 2015
 *      Author: domenicoamelino
 */

#ifndef SRC_GALOIS_H_
#define SRC_GALOIS_H_

#include <stdint.h>
#define MASK_SHREG (0xFFFFFFFF >> 32 -m)
#define MASK_LSB   (0x1)
#define MASK_LSmB  (0xFFFF) >> (16-m)
#define MASK_MSB    (0x1 << m)
#define PRIM_POLY  0x000000 // da impostare.

//Global Variable

int index_of[1048576];
//Functions
int gf_c_mul(int z, int exponent,int m);
int gf_div(int * z,int n, int f,int m);
int gf_mul(int y, int z,int m);
void generate_gf(uint8_t * p,int m,int n,int * alpha_to);
void read_p(uint8_t * p,int m);

#endif /* SRC_GALOIS_H_ */
