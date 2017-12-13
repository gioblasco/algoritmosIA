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
} Elements;
/***************/

typedef struct
{
	size_t cluster_size; /*Quantos elementos tem no cluster*/
	unsigned *elements; /*Índice dos elementos no vetor do arquivo de dados (Do tipo Elements)*/
} Cluster;

/******Variaveis Globais******/
float** dist_matrix;
Cluster *clu;
Elements *ele;
size_t cluster_qt; 
size_t elements_qt;
/*****************************/


void init_clusters(); /*Simplezinha*/
int compara_unsigned(const void *e1, const void *e2); /*Pro qsort, ignorar*/
void merge_matriz(unsigned i, unsigned j); /*Acho que ta bugada, temos que ver*/
float busca_elemento(int x,int y); /*Essa função é para pessoas que possuem preguiça de pensar*/
float min_avg(unsigned pivo, unsigned x, unsigned y); /*Coração do baguio*/



int main(int argc, char **argv)
{
	FILE *arquivo;
	Elements *elements;
	char buffer[128];
	unsigned i, j, kmin, kmax, katual,ultimok, porcentagem;
	float dx, dy;
	//tupla2 menor_atual;
	if(argc!=4 || !sscanf(argv[2], "%u", &kmin) || !sscanf(argv[3], "%u", &kmax))
	{
		printf("\nChamada inválida use: $ ./avg-link <dataset> <corte_mínimo> <corte_máximo>\n");
		return 1;
	}

	arquivo = fopen(argv[1], "r");
	if(!arquivo)
	{
		printf("\nErro ao abrir arquivo de dataset\n");
		return 1;
	}
	printf("**Certifique-se de que o dataset tem seus atributos separados por tab.\n  Caso contrário, a partição gerada é errônea.**\n");
	
	for(elements_qt = 0, fgets(buffer,128,arquivo), elements = NULL;	fgets(buffer,128,arquivo)!=0; elements_qt++)
	{
		if( elements_qt % 500 == 0)
		{
			elements = (Elements*) realloc(elements,	sizeof(Elements) * (elements_qt + 500));
			if(!elements)
			{
				printf("Não houve memória suficiente para rodar!\n");
				return 1;
			}
		}
		sscanf(buffer,"%s\t%f\t%f", elements[elements_qt].string, &elements[elements_qt].D1, &elements[elements_qt].D2);
	}
	/*Setting free on non-used memmory: */
	elements = (Elements*) realloc(elements, sizeof(Elements) *	elements_qt );
	
	printf("Arquivo carregado com sucesso! Calculando distâncias...\n");

	/*Alloc of the matrix lines*/
	dist_matrix = (float**) malloc(elements_qt  * sizeof(float*));

	if(!dist_matrix)
	{
			printf("Não houve memória suficiente para alocar a matriz de distâncias!\n");
			return 1;
	}

	for(i=0; i < elements_qt; i++)
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


	fclose(arquivo);
	arquivo = NULL;

	printf("Criando agrupamentos iniciais...\n");
	init_clusters();

	while(cluster_qt > kmin)
	{
		for(i=0; i< elements_qt; i++){
			for(j=0; i< elements_qt; i++){
	  	//merge_matriz(i, j);
			}
		}
    //merge_clusters();
	}

	return 0;
}


void init_clusters()
{
	unsigned c;
	clu = (Cluster*) malloc(sizeof(Cluster) * elements_qt);
	for(c=0; c < elements_qt; c++)
	{
		clu[c].cluster_size = 1;
		clu[c].elements = (unsigned*) malloc(sizeof(unsigned)); 
		clu[c].elements[0] = c; /*Inicialmente todo cluster contém um elemento*/
	}
	cluster_qt = elements_qt;
}

/*Função para ser chamada após dar merge na matriz*/
char merge_clusters(unsigned x, unsigned y)
{

	unsigned c;
	unsigned temp = clu[x].cluster_size;
	if(x==y)
		return 0;
	/*Vamos garantir que x é o menor*/
	if(x>y)
	{
		c = x;
		x = y;
		y = c;
	}

	clu[x].cluster_size += clu[y].cluster_size;
	clu[x].elements = (unsigned *) realloc(clu[x].elements, sizeof(unsigned) * clu[x].cluster_size);

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

	return 1;
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


/* Ta em fase beta ainda*/
void merge_matriz(unsigned i, unsigned j)
{
	unsigned k,c,t;

	/*Como estamos trabalhando com matriz triangular inferior precisamos dessa garantia*/
	if(i > j)
	{
		k = j;
		j = i;
		i = k;
	}

	/* Vamos pegar todas as aparições de (x,i) na matriz e substituir por ( d(x,i)*|i|, d(x,j)*|j| ) / |i| + |j|
	Fazendo assim, no final do calculo com que a linha e a coluna i, na verdade represente ioj (i concatenado j).
	Então após isso podemos excluir j da matriz. */

	/*Trocando as linhas i por ioj*/
	for(c=0;c<i;c++)
		dist_matrix[i][c] = min_avg(c, i,j);

	/* Trocando as colunas i por ioj:*/
	for(c=i+1; c<cluster_qt; c++)
		dist_matrix[c][i] = min_avg(c, i,j);

	/* A partir de agora a linha e coluna i é ioj, não precisamos mais da linha e coluna j */
	

	/* Removendo linha e coluna j: */

	/* Removendo a linha j */
	for(c = j; c < cluster_qt - 1; c++)
		memcpy(dist_matrix[c], dist_matrix[c+1], sizeof(float) * cluster_qt);

	/*Realocando a matriz em termos de linhas*/
	dist_matrix = (float**) realloc(dist_matrix, sizeof(float*)*(cluster_qt-1));

	/* Removendo a coluna j, e realocando em uma coluna a menos*/
	for(c = j+1; c < cluster_qt-1; c++)
	{	
		for( t = j; t < cluster_qt-1; t++)
			dist_matrix[c][t] = dist_matrix[c][t+1];
		dist_matrix[c] = (float*) realloc(dist_matrix[c],sizeof(float) * (cluster_qt-1));
	}

	/*Agora podemos diminuir o tamanho da matriz*/
	if(!merge_clusters(i,j))
	{
		printf("\nDeu um erro de consistência na hora de rodar o algorítimo!\n\n");
		exit(1);
	}
}

float min_avg(unsigned pivo, unsigned x, unsigned y)
{
	return ( busca_elemento(pivo,x) * clu[x].cluster_size + busca_elemento(pivo,y) * clu[y].cluster_size ) / (float) (clu[x].cluster_size + clu[y].cluster_size);
}

float busca_elemento(int x,int y)
{
	if(x>y)
		return dist_matrix[x][y];
	return dist_matrix[y][x];
}