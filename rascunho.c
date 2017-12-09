#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define TAMANHO 6
#include <float.h>
#include <string.h>

/*
1.54 
4.96 4.71 
2.60 1.71 7.76 

Indo dar merge em 1 e 0

Resultado Final:

0.00 
0.00 0.00 

*/

/*Dist vector used, instead matrix*/
typedef struct 
{
	float dist;
	unsigned x;
	unsigned y;
} tupla2;

float **matriz;

float min(float a, float b);
float busca_elemento(int x,int y);
void merge_matriz(unsigned i, unsigned j);
unsigned tam;

int main(int argc, char** argv)
{
	unsigned c,d,e;
	tam = TAMANHO;
	tupla2 tamanho;

	if(argc!=2)
	{
		return 1;
	}
	sscanf(argv[1],"%u", &e);
	srand(e);
	matriz = (float**) malloc(sizeof(float*) * tam);
	for(c=0; c < tam;c++)
	{
		matriz[c] = (float*) calloc(sizeof(float) , tam);
		for(d=0; d<c; d++)
		{
			matriz[c][d] = (float) (rand()%790)/100.0;
			printf("%.2f ", matriz[c][d]);
		}
		puts("");
	}
	tamanho.dist = FLT_MAX;

	for(c=0; c< tam; c++)
		for(d=0; d<c; d++)
			if(matriz[c][d] < tamanho.dist)
			{
				tamanho.dist = matriz[c][d];
				tamanho.x = c;
				tamanho.y = d;
			}

	printf("\nIndo dar merge em %u e %u\n\n", tamanho.x, tamanho.y);
	merge_matriz(tamanho.x, tamanho.y);
	printf("Resultado Final:\n");


	for(c=0; c< tam; puts(""), c++)
		for(d=0; d<c; d++)
			printf("%.2f ", matriz[c][d]);
		
	
	return 0;
}

float min(float a, float b)
{
	return ( a<b ) ? a : b ;
}


float busca_elemento(int x,int y)
{
	if(x>y)
		return matriz[x][y];
	return matriz[y][x];
}



void merge_matriz(unsigned i, unsigned j)
{
	unsigned k;
	unsigned c,d;

	if(i > j)
	{
		k = j;
		j = i;
		i = k;
	}

	//pegar todas as aparições de [i] na matriz e substituir por min(d(i), d(j))

	///na linha i:
	for(c=0;c<i;c++)
		matriz[i][c] = min(matriz[i][c], busca_elemento(j,c));

	//na coluna i:
	for(c=i+1;c<tam;c++)
		matriz[c][i] = min(matriz[c][i], busca_elemento(c,j));

	// A partir de agora a linha e coluna i é (i,j)

	//salva_informação_de_agrupamento()

	// Remove linha e coluna j:


	// Removendo a linha
	for(c = j; c < tam-1; c++)
	{
		memcpy(matriz[c], matriz[c+1], sizeof(float) * tam);
	}

	// Removendo a coluna
	for(c = 0; c < tam-1; c++)
	{	
		for( d = j; d < tam-1; d++)
			matriz[c][d] = matriz[c][d+1];
		matriz[c] = (float*) realloc(matriz[c],sizeof(float) * (tam-1));
	}

	free(matriz[tam-1]);
	matriz = (float**) realloc(matriz, sizeof(float*)*(tam-1));
	tam--;



}

