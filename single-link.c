#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Data struct for receive the elements from file*/
typedef struct 
{
	char string[32];
	float D1;
	float D2;
} tupla1;
/***************/


/*Dist vector used, instead matrix*/
typedef struct 
{
	float dist;
	unsigned x;
	unsigned y;
} tupla2;
/*********************************/

/*Disjunt-set Data structure for clusters*/
typedef struct
{
	char string[32];
	unsigned short rank;
	unsigned parent;
} tupla3;
/*****************************************/

/*Functions pre declarations*/
int  m2v(int x,int y,int n);
int compara_custo(const void *elem1, const void *elem2);
char merge(unsigned x, unsigned y);
unsigned find_parent(unsigned x);
void print_file(unsigned index);
/****************************/

/*Global Pointers*/
tupla1 *elements;
tupla2 *dist_vector;
tupla3 *floresta;
/*****************/


int main(int argc, char **argv)
{
	FILE *arquivo;

	

	char buffer[128];
	unsigned i, j, kmin, kmax;
	int temp;
	float dx, dy;
	size_t tam;

	if(argc!=4 || !sscanf(argv[2], "%u", &kmin) || !sscanf(argv[3], "%u", &kmax))
	{
		printf("\nChamada inválida use: $ ./single-link <dataset> <corte_mínimo> <corte_máximo>\n");
		return 1;
	}

	arquivo = fopen(argv[1], "r");
	if(!arquivo)
	{
		printf("\nErro ao abrir arquivo de dataset\n");
		return 1;
	}
	printf("**Certifique-se de que o dataset tem seus atributos separados por tab.\n  Caso contrário, a partição gerada é errônea.**\n");
	tam = 0;

	/*consome primeira linha do arquivo*/
	for(fgets(buffer,128,arquivo), elements = NULL;					fgets(buffer,128,arquivo)!=0; tam++)
	{
		if( tam % 500 == 0)
		{
			elements = (tupla1*) realloc(elements,	sizeof(tupla1) * (tam + 500));
			if(!elements)
			{
				printf("Não houve memória suficiente para rodar!\n");
				return 1;
			}
		}
		sscanf(buffer,"%s\t%f\t%f", elements[tam].string, &elements[tam].D1, &elements[tam].D2);
	}
	fclose(arquivo);
	arquivo = NULL;

	/*Setting free on non-used memmory: */
	elements = (tupla1*) realloc(elements, sizeof(tupla1) *	tam );
	


	printf("Arquivo carregado com sucesso! Calculando distâncias...\n");


	dist_vector = (tupla2*) malloc(sizeof(tupla2) * ( tam*tam - tam) );
	if(!dist_vector)
	{
			printf("Não houve memória suficiente para alocar o vetor de distâncias!\n");
			return 1;
	}

	for(i=0; i<tam; i++)
		for(j=i+1; j<tam; j++)
		{
			temp = m2v(i,j,tam);
			dx = (elements[i].D1 - elements[j].D1);
			dy = (elements[i].D2 - elements[j].D2);
			dist_vector[temp].dist = sqrt( dx*dx  +  dy*dy );
			dist_vector[temp].x = i;
			dist_vector[temp].y = j;
		}
	printf("Distâncias calculadas com sucesso! Ordenando Vetor...\n");
	qsort(dist_vector,tam,sizeof(tupla2),compara_custo);
	printf("Vetor ordenado com sucesso! Criando agrupamentos...\n");
	
	floresta = (tupla3*) malloc(sizeof(tupla3) * tam);

	for(i=0; i<tam; i++)
	{
		strcpy(floresta[i].string, elements[i].string);
		floresta[i].parent = i;
		floresta[i].rank = 0;
	}

	/*We don't need more the D1 and D2 at this point, so let's free the array*/
	free(elements);
	elements = NULL;

	for(i=1, j = 0; i < tam && j < tam - kmin; i++)
	{
		if(i==320)
			puts("oi");
		if( find_parent( m2v(dist_vector[i-1].x,dist_vector[i-1].y,tam) ) !=  find_parent( m2v(dist_vector[i].x,dist_vector[i].y,tam) ) )
			j += (int) merge(i-1, i);
		else
			printf("Aparentemente os elementos %hu e %hu já estão no cluster %hu.\n", i-1, i, find_parent( m2v(dist_vector[i].x,dist_vector[i].y,tam) ));
		
		if( j >= tam - kmax && j <= tam - kmin)
			print_file(j+1);
		
	}

	return 0;
}

/*Upper triangule Matrix indexation*/
int m2v(int x,int y,int n)
{
	return (int) ((( n * (n-1) ) / 2) - ( ( (n-x) * (n-x-1) ) / 2 ) + y - x - 1) ;
}



int compara_custo(const void *elem1, const void *elem2)
{
	tupla2 *t1 = (tupla2*) elem1, *t2 = (tupla2*) elem2;
	float temp;
	temp = t1->dist - t2->dist;
	if(temp < 0)
		return -1;
	else if (temp > 0)
		return 1;
	return 0;
}


unsigned find_parent(unsigned x)
{
	if(floresta[x].parent != x) /*if the parent is not me*/
		floresta[x].parent = find_parent(floresta[x].parent);   /* then my parent is the parent of my parent */
	return floresta[x].parent;
}


char merge(unsigned x, unsigned y)
{
	unsigned xRoot = find_parent(x);
	unsigned yRoot = find_parent(y);

	if(xRoot == yRoot)  /* if we do not need to merge the clusters */
		return 0; /*union not maked */

	/*otherwise, lets merge in a not naive mode: */

	if(floresta[xRoot].rank < floresta[yRoot].rank)
		floresta[xRoot].parent = yRoot;

	else if (floresta[xRoot].rank > floresta[yRoot].rank)
		floresta[yRoot].parent = xRoot;
	else
		floresta[yRoot].parent = xRoot;
		floresta[xRoot].rank = floresta[xRoot].rank +1;
	return 1;
}
	
void print_file(unsigned index)
{
	printf("Index recebido foi %hu\n", index);
}