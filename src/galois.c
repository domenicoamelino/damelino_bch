/*
 * galois.c
 *
 *  Created on: 10 dic 2015
 *      Author: domenicoamelino
 */
#include "galois.h"

//Division from z(x) n order, and f(x) m order.
// I valori, tranne Z sono visti come "vettori" di coefficienti di un polinomio.
int gf_div(int * z,int n, int f,int m)
{
	int idWord,idBit,currWord,currBit,shREG;
	int shRegMSB = 0;
	for(idWord=0;idWord<=(n/32);idWord++)
	{
		currWord = z[idWord];
		for(idBit=32;idBit>0;--idBit)
		{
			currBit = (currWord>>idBit) & MASK_LSB;
			shRegMSB = shREG >> (m-1);
			shREG = (shREG << 1) & MASK_SHREG;
			if(shRegMSB ^ currBit) shREG ^= f;

		}
	}

	return shREG;  // da valutare ancora un po la dimensionalità delle variabili in ingresso/uscita
}

int gf_c_mul(int z, int exponent,int m)
{
	int itr;
	for(itr=0;itr<exponent;itr++)
	{
		z = z << 1;
		if(z & MASK_MSB)
		{
			z ^= PRIM_POLY;
			z &= MASK_LSmB;
		}
	}
	return z;
}

// Multiplication between two polynomial vector with m order
int gf_mul(int y, int z,int m)
{
	int yz=0;
	int itr;
	for(itr=0;itr<m;itr++)
	{
		if(y & MASK_LSB) yz ^= gf_c_mul(z,itr,m);
		y = y >> 1;
	}

	return yz;
}

void generate_gf(uint8_t * p,int m,int n,int * alpha_to)
{
	register int i,mask;

	mask=1;
	alpha_to[m]=0;
	for(i=0;i<m;i++)
	{
		alpha_to[i]=mask;
		index_of[alpha_to[i]] = i;
		if(p[i]!=0) alpha_to[m] ^= mask;
		mask <<=1;
	}
	index_of[alpha_to[m]] = m;
	mask >>=1;
	for(i=m+1;i<n;i++)
	{
		if(alpha_to[i-1] >= mask) alpha_to[i] = alpha_to[m] ^ ( (alpha_to[i-1] ^ mask)<<1);
		else alpha_to[i] = alpha_to[i-1] <<1;

		index_of[alpha_to[i]] = i;
	}
	index_of[0]=-1;
}

void read_p(uint8_t * p,int m)
{
	int i;
	for(i=1;i<m;i++) p[i]=0;
	p[0]=p[m]=1;
	if(m==2) p[1]=1;
	else if(m==3)  p[1]=1;
	else if(m==4)  p[1]=1;
	else if(m==5)  p[2]=1;
	else if(m==6)  p[1]=1;
	else if(m==7)  p[1]=1;
	else if(m==8)  p[4]=p[5]=p[6]=1;
	else if(m==9)  p[4]=1;
	else if(m==10) p[3]=1;
	else if(m==11) p[2]=1;
	else if(m==12) p[3]=p[4]=p[7]=1;
	else if(m==13) p[1]=p[3]=p[4]=1;
	else if(m==14) p[1]=p[11]=p[12]=1;
	else if(m==15) p[1]=1;
	else if(m==16) p[2]=p[3]=p[5]=1;
	else if(m==17) p[3]=1;
	else if(m==18) p[7]=1;
	else if(m==19) p[1]=p[5]=p[6]=1;
	else if(m==20) p[3]=1;
}

