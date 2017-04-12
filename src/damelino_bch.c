/*
 ============================================================================
 Name        : damelino_reedmuller.c
 Author      : damelino
 Version     :
 Copyright   : Your copyright notice
 Description : Reed Muller Encoder/Decoder
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "lfsr.h"
#include "bch.h"
#include "galois.h"
// Stato lavorazione: ENCODE-> BCH(15,7) BCH(7,4),BCH(15,5) FUNZIONANO . Dobbiamo estendere i casi però.

int main(void)
{
	int m=4; // valore per determinare campo di galois
	int n = (1<<m)-1; //valore per determinare campo di galois
	int syndr[6]; // 2*t
	//uint8_t res [15] = {0,0,1,0,0,1,0,1,1,0,1,0,0,1,1};
	uint8_t res[15] =    {1,1,1,1,1,0,1,0,1,0,0,1,0,0,1};
	uint8_t result [15];
	for(int i=0;i<10;i++) result[i] = 0;
	int alpha_to[1048576];
	uint8_t p[21];
	read_p(p,m);
	generate_gf(p,m,n,alpha_to);
	printf("Polinomio Generatore \n");

	for(int i=0;i<=m;i++) printf("Posizione %d %d \n",i,p[i]);

	for(int i=0;i<n;i++) printf("%d\n",alpha_to[i]);
	printf("\n");
	for(int i=0;i<n;i++) printf("%d\n",index_of[i]);
	printf("\n");
	for(int i=0;i<n;i++)
	{
		int binary_buffer[m];
		for(int j=0;j<m;j++) binary_buffer[j]=0;
		IntToBin(alpha_to[i],binary_buffer,m);
		for(int j=0;j<m;j++) printf("%d",binary_buffer[j]);
		printf("\n");
	}

	printf("\n");
	printf("Sto per iniziare la decode \n");
	int boh = decode(res,alpha_to,result);

	printf("Result decode = %d ...quindi \n",boh);
	for(int i=0;i<15;i++) printf(" %d ",result[i]);
	printf("\n");
	printf("Original Message \n");
	for(int i=N-K;i<N;i++) printf("%d", result[i]);

    /*
	syndr[0] = syndrome_calc(res,alpha_to,1);
	printf("%d \n",index_of[syndr[0]]); // <--- il valore restituito è il VETTORE polinomiale alfa..ma si trova!
	*/
	lfsr miao;
	uint8_t * ciao = 0;

	init_lfsr(miao);
	// Message vector structure {v0,v1,..vn-1}
	//uint8_t msg [] = {1,0,0,0,0,0,0}; //funge
	//uint8_t msg[] = {0,0,1,1}; // funge
	uint8_t msg[] = {1,0,0,0,1}; // funge
	//uint8_t msg [] = {1,0,1,0,1,0,0,0,0,1,0,0,1,1,1,0}; funge


	// Message vector structure {v0,v1,..vn-1}
	//uint8_t gen_poly [8] = {1,0,0,0,1,0,1,1}; // tanto poi l'ultimo bit non lo si caga.
	//uint8_t gen_poly[] = {1,0,0,0,1,0,1,1,1}; // funziona questo input
	//uint8_t gen_poly[] = {1,1,0,1}; // funge
	uint8_t gen_poly [10] = {1,1,1,0,1,1,0,0,1,0,1};
	//uint8_t gen_poly [] = {1,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1};
	// Vector logic structure {v0,v1...vn-1} matlab fa il contrario.
	//uint8_t gen_poly [] = {1,1,1,1,0,1,0,1,1,1,1,1,0,0,0,1}; // funge
	//uint8_t gen_poly [4] = {1,0,0,0};

	// Funziona ma al momento lo commento!
	//encode(miao,gen_poly,msg,res);

	//printf("Risultato encode \n");
	//for(int i=1;i<=N;i++) printf("%d",res[N-i]);
	//printf("\n");



	return EXIT_SUCCESS;
}
