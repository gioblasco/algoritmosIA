#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

/*https://home.deib.polimi.it/matteucc/Clustering/tutorial_html/hierarchical.html*/

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

char merge(unsigned x, unsigned y);
unsigned find_parent(unsigned x);
float min(float a, float b);
float busca_elemento(int x,int y);
void merge_matriz(unsigned i, unsigned j);
void print_file(unsigned index, char* argv);
/****************************/

/*Global Vars*/
tupla1 *elements;
float **dist_matrix;
tupla3 *floresta;
size_t tam;
/*****************/


int main(int argc, char **argv)
{
	FILE *arquivo;

	char buffer[128];
	unsigned i, j, kmin, kmax, katual,ultimok, porcentagem;
	float dx, dy;
	tupla2 menor_atual;
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
	for(fgets(buffer,128,arquivo), elements = NULL;	fgets(buffer,128,arquivo)!=0; tam++)
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

	/*Alloc of the matrix lines*/
	dist_matrix = (float**) malloc(tam  * sizeof(float*));

	if(!dist_matrix)
	{
			printf("Não houve memória suficiente para alocar a matrix de distâncias!\n");
			return 1;
	}

	for(i=0; i<tam; i++)
	{
		dist_matrix[i] = malloc ( (i) * sizeof(float) );
		for(j=0; j<i; j++)
		{
			dx = (elements[i].D1 - elements[j].D1);
			dy = (elements[i].D2 - elements[j].D2);
			dist_matrix[i][j] = sqrt( dx*dx  +  dy*dy );
		}
	}
	printf("Distâncias calculadas com sucesso!\n");

	printf("Criando agrupamentos iniciais...\n");
	
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
	printf("Agrupamentos iniciais definidos, memória inicial liberada...\n");

	printf("Iterando na matriz de distância e mesclando agrupamentos...\n");	
	//printf("Porcentagem: 0%%\n");
	katual = tam;
	for(ultimok = 0; tam > kmin; )
	{
		porcentagem = (unsigned) ((float) (katual - tam) / (float) katual * 100.0);
		//if(porcentagem % 5 == 0)
		//printf("\rPorcentagem: %u%%", porcentagem);
		
		menor_atual.dist = FLT_MAX; /*In the begining the min dist is inf */	
		for(i = 1 ; i < tam; i++)
		{
			for(j=0; j < i; j++)
			{	
				printf("%.2f ", dist_matrix[i][j]);
				if(dist_matrix[i][j] < menor_atual.dist && find_parent(i) != find_parent(j))
				{
					menor_atual.dist = dist_matrix[i][j];
					menor_atual.x = i;
					menor_atual.y = j;
				}
			}
			puts("");
		}
		printf("\nIndo dar merge em %u e %u\n\n", menor_atual.x, menor_atual.y);
		merge_matriz(menor_atual.x, menor_atual.y);

		if( tam != ultimok && tam >=  kmin && tam <= kmax)
		{
				print_file(tam, argv[1]);
				ultimok = tam;
		}
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
	else if(xRoot < yRoot)
		floresta[yRoot].parent = x;
	else
		floresta[xRoot].parent = y;
		

	/*otherwise, lets merge in a (!not) naive mode: */

	/*if(floresta[xRoot].rank < floresta[yRoot].rank)
		floresta[xRoot].parent = yRoot;

	else if (floresta[xRoot].rank > floresta[yRoot].rank)
		floresta[yRoot].parent = xRoot;
	else
		if(x<y)
		{
			floresta[yRoot].parent = xRoot;
			floresta[xRoot].rank = floresta[xRoot].rank +1;
		}
		else
		{
			floresta[xRoot].parent = yRoot;
			floresta[yRoot].rank = floresta[yRoot].rank + 1;
		}*/
	return 1;
}

/* Dado um índice X, imprime ele em um arquivo */
void print_file(unsigned index, char* argv)
{
	unsigned i;
	char temp1[128], temp2[128];

	strcpy(temp1, argv);
	strtok(temp1, ".");
	sprintf(temp2, "%s%u.clu",temp1,index);
	FILE *output;
	printf("\nImprimindo o arquivo %s\n", temp2);
	output = fopen(temp2, "w");

	for(i=0; i< tam; i++)
	{
		fprintf(output, "%s\t%u\n", floresta[i].string, floresta[i].parent + 1);
	}

	fclose(output);

}

float min(float a, float b)
{
	return ( a<b ) ? a : b ;
}


float busca_elemento(int x,int y)
{
	if(x>y)
		return dist_matrix[x][y];
	return dist_matrix[y][x];
}



void merge_matriz(unsigned i, unsigned j)
{
	unsigned k;
	unsigned c,d;

	/*Como estamos trabalhando com matriz triangular inferior precisamos dessa garantia*/
	if(i > j)
	{
		k = j;
		j = i;
		i = k;
	}

	/* Vamos pegar todas as aparições de (x,i) na matriz e substituir por min( (x,i) , (x,j) )
	Fazendo assim, no final do calculo com que a linha e a coluna i, na verdade represente ioj (i concatenado j). */

	/*Trocando as linhas i por ioj*/
	for(c=0;c<i;c++)
		dist_matrix[i][c] = min(dist_matrix[i][c], busca_elemento(j,c));

	/* Trocando as colunas i por ioj:*/
	for(c=i+1;c<tam;c++)
		dist_matrix[c][i] = min(dist_matrix[c][i], busca_elemento(c,j));

	/* A partir de agora a linha e coluna i é ioj, não precisamos mais da linha e coluna j */
	
	if(!merge(i,j))
	{
		printf("\nDeu um erro de consistência na hora de rodar o algorítimo!\n\n");
		exit(1);
	}

	/* Removendo linha e coluna j: */

	/* Removendo a linha j */
	for(c = j; c < tam-1; c++)
	{
		memcpy(dist_matrix[c], dist_matrix[c+1], sizeof(float) * tam);
	}

	/*Realocando a matriz em termos de linhas*/
	dist_matrix = (float**) realloc(dist_matrix, sizeof(float*)*(tam-1));

	/* Removendo a coluna j, e realocando em uma coluna a menos*/
	for(c = j+1; c < tam-1; c++)
	{	
		for( d = j; d < tam-1; d++)
			dist_matrix[c][d] = dist_matrix[c][d+1];
		dist_matrix[c] = (float*) realloc(dist_matrix[c],sizeof(float) * (tam-1));
	}

	/*Agora podemos diminuir o tamanho da matriz*/
	tam--;
}
