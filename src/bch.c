/*
 * bch.c
 *
 *  Created on: 02 dic 2015
 *      Author: domenicoamelino
 */
#include "bch.h"
#include "galois.h"
#include "utilities.h"

void encode(lfsr shift_reg,uint8_t* gen_poly,uint8_t* msg,uint8_t* result)
{
	//printf("Benvenuti nell'encode \n");
	// Set all the vector to 0.
	for(int i=0;i<N;i++) result[i] = 0;
	uint8_t out=0;
	for(int i=0;i<N;i++)
	{

		if(i<K)
		{
			uint8_t shift_input = msg[i] ^ out;// prima al posto di out c'era shift_reg[N-K-1];
			//printf("Input %d-esimo ->%d \n",i,shift_input);
			out = remainder_insert(shift_reg,gen_poly,shift_input);
			//printf("Output %d-esimo ->%d \n",i,out);
			//for(int k=0;k<SIZE;k++) printf("%d",shift_reg[k]);
			//printf("\n");

			//Result update
			result[i] = msg[i];
		}
		else
		{

			out = insert_element(shift_reg,0); /* questo passaggio forse lo posso fare "più parallelizzato"
												  perchè prendo al contrario pari pari lo shift register e
												  lo posso salvare nel result */
			//for(int k=0;k<SIZE;k++) printf("%d",shift_reg[k]);
			//printf("\n");
			result[i] = out;
		}
	}

}

int decode(uint8_t * received,int * alpha, uint8_t * result)
{
	printf("Sono nella decode \n");
	int thereisanerror;
	// Syndrome Evaluation
	int syndr[doubleT+1];
	int reg[201];
	int root[200]; // cercare di eliminare questi ultimi due;
	int loc[200];
	for(int i=0;i<doubleT+1;i++) syndr[i]=0;
	for(int i=1;i<=doubleT;i++)
	{
		if(i%2==0)
		{
			//printf("Stavolta faccio il prodotto di %d per 2 e viene %d \n", i,index_of[syndr[i/2]]*2);
			int buffer = index_of[syndr[i/2]]*2;
			while (buffer>N)
			{
				buffer = buffer - N;
			}
			syndr[i] = alpha[buffer]; // TODO - verificare con dei test la correttezza;
		}
		else syndr[i] = syndrome_calc(received,alpha,i);

		if(syndr[i]!=0) thereisanerror=1;
	}

	printf("Syndrome: \n");
	for(int i =1 ;i <=doubleT;i++)
	{
		syndr[i] = index_of[syndr[i]];
		printf(" %d ",syndr[i]);
	}
	printf("\n");
	// Berlekamp-Massey Algorithm

	int error_poly[doubleT+1][doubleT+1];

	if(thereisanerror>0)
	{
		int u;
		int q;
		int l_k = bma(alpha,syndr, error_poly,&u);
		//printf("Polinomio di errore \n");
		//for(int i=0;i<=l_k;i++) printf(" %d", error_poly[u][i]);
		printf("\n");
		if(l_k <= T) //...so we can correct errors.
		{
			//printf("Posso ancora salvarli \n");
			// Chien Search
			for(int i=1;i<= l_k ; i++) reg[i] = error_poly[u][i];
			int count = 0;
			for(int i=1; i<= N; i++)
			{
				q = 1;
				for(int j=1;j<=l_k;j++)
				{
					if(reg[j] != -1)
					{
						reg[j] = (reg[j]+j) % N;
						q ^= alpha[reg[j]];
					}
				}
				if(!q)
				{
					root[count ] = i;
					loc[count] = N-i;
					count++;
				}

				//printf("Count %d \n",count);
			}

			int error[N];
			for(int i=0;i<N;i++) error[i] = 0;

			if(count==l_k)
			{
				//printf("Io posso decodificare! \n");
				for(int i=0;i<l_k;i++) error[loc[i]] ^= 1;
				//printf("Vettore di errore \n");
				//for(int i=0;i<N;i++) printf("pos->%d %d \n",i,error[i]);
				//printf("\n");
				//printf("Vettore ricevuto \n");
				//for(int i=0;i<N;i++) printf("%d \n",received[i]);
				//printf("\n");
				// fine chien search!!

				for(int i=0;i<N;i++)
				{
					result[i] = received[i] ^ error[i];
				}


				/* Ora devo fare la somma XOR tra vettore result e vettore ricevuto e poi
				   prendere gli ultimi 5 bit del risultato della XOR..sarà quello il messaggio
				   decodificato! */
				return 0;
			}
			else return -1;

		}
		else return -1;
	}
	else return 0;
}

int bma(int * alpha, int * syndr, int sigma[doubleT+1][doubleT+1],int * u)
{
	int k=0;
	int q=0;
	int j=0;
	int l[1026];
	int d[1026],u_lu[1026];
	d[0] = 0;
	d[1] = syndr[1];
	sigma[0][0] = 0;
	sigma[1][0] = 1;

	for(int i=1;i<doubleT;i++)
	{
		sigma[0][i] = -1;
		sigma[1][i] = 0;
	}

	l[0] = 0;
	l[1] = 0;
	u_lu[0] = -1;
	u_lu[1] = 0;
	do
	{
		k++;
		if(d[k] == -1)
		{
			l[k+1] = l[k];
			for(int i=0;i<=l[k];i++)
			{
				sigma[k+1][i] = sigma[k][i];
				sigma[k][i]= index_of[sigma[k][i]];
			}
		}
		else
		{
			q = k-1;
			while( (d[q]==-1) && (q>0)) q--;
			if(q>0)
			{
				j=q;
				do
				{
					j--;
					if((d[j]!=-1) && (u_lu[q]<u_lu[j])) q=j;
				} while(j>0);
			}

			if(l[k]>l[q]+k-q) l[k+1]=l[k];
			else l[k+1] = l[q]+k-q;

			//printf("TEST L[k+1] iterazione %d = %d \n ",k,l[k+1]);
			//printf("TEST l[q] = %d \n",l[q]);

			for(int i=0;i<doubleT;i++) sigma[k+1][i] = 0;
			for(int i=0;i<=l[q];i++)
			{
				//printf("condizione sigma %d \n",sigma[q][i]);
				if(sigma[q][i] != -1) sigma[k+1][i+k-q] = alpha[(d[k]+N-d[q]+sigma[q][i])%N];
			}

			for(int i=0;i<=l[k];i++)
			{
				//printf("Sigma[%d][%d]=%d XOR Sigma[%d][%d]=%d \n",k+1,i,sigma[k+1][i],k,i,sigma[k][i]);
				sigma[k+1][i] ^= sigma[k][i];
				sigma[k][i] = index_of[sigma[k][i]];
			}
		}
		u_lu[k+1] = k - l[k+1];

		if(k<doubleT)
		{
			if(syndr[k+1] != -1) d[k+1] = alpha[syndr[k+1]];
			else d[k+1] = 0;

			for(int i=1;i<=l[k+1];i++)
			{
				//printf("TEST syndr %d \n",syndr[k+1-i]);
				//printf("TEST sigma [%d][%d] = %d \n",k+1,i,sigma[k+1][i]);
				if((syndr[k+1-i] != -1) && (sigma[k+1][i] != 0))
				{
					d[k+1] ^= alpha[(syndr[k+1-i] + index_of[sigma[k+1][i]])%N];
				}

			}

			//printf("Iterazione %d -> Discrepancy = %d \n",k,d[k+1]);
			d[k+1] = index_of[d[k+1]];
			//printf("Iterazione %d -> Discrepancy = %d (forma index of) \n",k,d[k+1]);

		}

		printf("\n");

	} while((k<doubleT) && (l[k+1]<= T));
	k++;
	for(int i=0;i<= l[k] ;i++) sigma[k][i] = index_of[sigma[k][i]];
	*u = k;
	return l[k];
}

int syndrome_calc(uint8_t * received,int * alpha,int i)
{
	int j;
	int syndr=0;
	for(j=0;j<N;j++)
	{
		if(received[j]>0)
		{
			//printf("Cifra %d maggiore di 0 \n",j);
			syndr = syndr ^ alpha[(i*j)%N];
			//printf("= %d\n",syndr);
		}
	}
	return syndr;
}

