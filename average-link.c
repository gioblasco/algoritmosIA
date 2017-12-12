#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct
{
	char *nome;
	float x;
	float y;
} Elements;

typedef struct
{
	size_t cluster_size; /*Quantos elementos tem no cluster*/
	unsigned *elements; /*Índice dos elementos no vetor do arquivo de dados (Do tipo Elements)*/
} Cluster;

/******Variaveis Globais******/
float** matriz_distancia;
Cluster *clu;
Elements *ele;
size_t cluster_qt; 
size_t elements_qt;
/*****************************/

int main()
{
	/* carrega_arquivo(); não precisa ser função, pode tacar na main também*/
	init_clusters()
	return 0;
}


void init_clusters()
{
	unsigned c;
	clu = (Cluster*) malloc(sizeof(Cluster) * elements_qt);
	for(c=0; c < elements_qt; c++)
	{
	clu[c].cluster_size = 1;
	clu[c].elements = (unsigned*) malloc(sizeof(unsigned) *5); /*Aloquei já em 5 slots para evitar realloc excessivo.
																																	É uma boa prática continuar realocando de 5 em 5*/
	clu[c].elements[0] = c; /*Inicialmente todo cluster contém um elemento*/
	}
	cluster_qt = elements_qt;
}

/*Função para ser chamada após dar merge na matriz*/
void merge_clusters(unsigned x, unsigned y)
{

	unsigned c;
	unsigned temp = clu[x].cluster_size;

	/*Vamos garantir que x é o menor*/
	if(x>y)
	{
		c = x;
		x = y;
		y = c;
	}

	clu[x].cluster_size += cluster[y].cluster_size;
	clu[x].elements = (unsigned *) realloc(clu[x].elements, sizeof(unsigned) * cluster[d].cluster_size);

	/*Jogando todos os elementos do cluster y no cluster x*/
	for(c=temp; c < clu[x].cluster_size; c++) /*Linha incerta, conferir depois*/
		clu[x].elements[c] = clu[y].elements[c-temp];

	free(clu[y].elements);
	/*É uma boa prática mantermos o cluster ordenado para depois fazermos busca binária:

	qsort(clu[x].elements, clu[x].cluster_size, sizeof(unsigned), compara_unsigned);

	Porém acho que não compensa ficar dando sorte  a cada merge, podemos dar sort só quando for imprimir no arquivo
	*/

	cluster_qt--;
	for(c = y; c < cluster_qt; c++)
		clu[c] = clu[c + 1];
	
	clu = realloc(clu, cluster_qt * sizeof(Cluster));
}

int compara_unsigned(const void *e1, const void *e2)
{
	unsigned *t1, *t2; 
	t1 = (unsigned*) e1;
	t2 = (unsigned*) e2;

	if(*t1 < *t2)
		return -1;
	else if(*t1 > *t2)
		return 1;
	return 0;
}

void merge_matriz(unsigned x, unsigned y)
{
	/*Acho que da pra copiar do single link*/
}