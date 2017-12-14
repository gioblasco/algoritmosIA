#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

/*Data struct for receive the elements from file*/


typedef struct 
{
		float D1;
		float D2;
} Ponto;

typedef union data
{
	unsigned indice_cluster;
	Ponto p;
} data;

typedef struct 
{
	char string[32];
	data d;
} Elements;
/***************/

typedef struct
{
	size_t cluster_size; /*Quantos elementos tem no cluster*/
	//unsigned *elements; /*Índice dos elementos no vetor do arquivo de dados (Do tipo Elements)*/
} Cluster;

typedef struct
{
	float dist;
	unsigned x;
	unsigned y;
} Menor_atual;


/******Variaveis Globais******/
float** dist_matrix;
Cluster *clu;
Elements *elements;
size_t cluster_qt; 
size_t elements_qt;

/*****************************/


void init_clusters(); /*Simplezinha*/
int compara_unsigned(const void *e1, const void *e2); /*Pro qsort, ignorar*/
void merge_matriz(unsigned i, unsigned j); /*Acho que ta bugada, temos que ver*/
float busca_elemento(int x,int y); /*Essa função é para pessoas que possuem preguiça de pensar*/
float min_avg(unsigned pivo, unsigned x, unsigned y); /*Coração do baguio*/
void print_file(unsigned index, char* argv); /*Imprime saída para realizar o índice Rand*/
void print_matrix();


int main(int argc, char **argv) /* A MAIN TA PRONTA, NÃO CAGAR A NÃO SER QUE SEJA PARA DESCAGAR OUTRA COISA !!!!*/
{
	FILE *arquivo;
	char buffer[128];
	unsigned i, j, kmin, kmax, porcentagem;
	float dx, dy;
	Menor_atual menor_atual;
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
		sscanf(buffer,"%s\t%f\t%f", elements[elements_qt].string, &elements[elements_qt].d.p.D1, &elements[elements_qt].d.p.D2);
	}
	/*Setting free on non-used memmory: */
	elements = (Elements*) realloc(elements, sizeof(Elements) *	elements_qt );
	
	printf("Arquivo carregado com sucesso!\nCalculando distâncias...\n");

	/*Alloc of the matrix lines*/
	dist_matrix = (float**) malloc(elements_qt  * sizeof(float*));

	if(!dist_matrix)
	{
			printf("Não houve memória suficiente para alocar a matriz de distâncias!\n");
			return 1;
	}

	for(i=0; i < elements_qt; i++)
	{
		porcentagem = (unsigned) (i / (float) elements_qt);
		if(porcentagem%5==0)
		printf("\rPorcentagem: %u", porcentagem);
		dist_matrix[i] = malloc ( (i) * sizeof(float) );
		for(j=0; j<i; j++)
		{
			dx = (elements[i].d.p.D1 - elements[j].d.p.D1);
			dy = (elements[i].d.p.D2 - elements[j].d.p.D2);
			dist_matrix[i][j] = sqrt( dx*dx  +  dy*dy );
		}
	}

	for(i=0; i < elements_qt; i++)
		elements[i].d.indice_cluster = i;

	printf("\rPorcentagem: 100%%\n");
	printf("Distâncias calculadas com sucesso!\n");


	fclose(arquivo);
	arquivo = NULL;

	printf("Criando agrupamentos iniciais...\n");
	init_clusters();
	printf("Agrupamentos Iniciais criados com sucesso, iterando o algoritmo.\n");
	while(cluster_qt >= kmin)
	{
		if(cluster_qt >= kmin && cluster_qt <= kmax)
			print_file(cluster_qt, argv[1]);

		menor_atual.dist = FLT_MAX;
		//print_matrix();
		for(i = 1; i< cluster_qt; i++)
			for(j=0; j < i ; j++)
				if(dist_matrix[i][j] < menor_atual.dist)
				{
					menor_atual.dist = dist_matrix[i][j];
					menor_atual.x = i;
					menor_atual.y = j;
				}
		printf("Indo mesclar os índices: %u e %u\n", menor_atual.x, menor_atual.y);
		merge_matriz(menor_atual.x, menor_atual.y);
	}

	return 0;
}

void print_matrix()
{
	unsigned c,d;
	for(c=0; c < cluster_qt; c++, puts(""))
		for(d = 0; d < c; d++)
			printf("%.2f ", dist_matrix[c][d]);
}
void init_clusters()
{
	unsigned c;
	clu = (Cluster*) malloc(sizeof(Cluster) * elements_qt);
	for(c=0; c < elements_qt; c++)
	{
		clu[c].cluster_size = 1;
	}
	cluster_qt = elements_qt;
}

/*Função para ser chamada após dar merge na matriz*/
char merge_clusters(unsigned x, unsigned y)
{

	unsigned c;
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
	cluster_qt--;

	for(c = 0; c < elements_qt; c++){
		if(elements[c].d.indice_cluster == y)
			elements[c].d.indice_cluster = x;
		else if (elements[c].d.indice_cluster > y)
			elements[c].d.indice_cluster--;
	}
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


void print_file(unsigned index, char* argv)
{
	unsigned i/*,d,t;*/;
	char temp1[128], temp2[128];
	//d = mapear();
	strcpy(temp1, argv);
	strtok(temp1, ".");
	sprintf(temp2, "%s%u.clu",temp1,index);
	FILE *output;
	printf("Imprimindo o arquivo %s\n", temp2);
	output = fopen(temp2, "w");

	for(i=0; i< elements_qt; i++)
	{
		fprintf(output, "%s\t%u\n", elements[i].string, elements[i].d.indice_cluster+1);
	}

	fclose(output);
}






