#include <stdio.h>
#include <stdbool.h>

#define VERTICE_INVALIDO NULL
#define AN -1//Aresta nula


typedef struct str_aresta{
	int origem; //Para algoritmo de kruskal		
	int destino;
	Peso peso;
	struct str_aresta* prox;
} Aresta;

typedef Aresta* Apontador;


typedef struct{
	Apontador* listaAdj;
	int numVertices;
	int numArestas;
	int* vetor_de_pais;
   	int* rank;
}Grafo;


typedef float Peso;
	

/*Funções do Grafo*/
bool verificaValidadeVertice(int v, Grafo* grafo);
bool inicializaGrafo(Grafo *grafo, int nv);
bool listaAdjVazia(int v,Grafo* grafo);
bool existeAresta(int v1,int v2, Grafo* grafo);
bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo* grafo);
void insereAresta(int v1,int v2, Peso peso, Grafo* grafo);
void liberaGrafo( Grafo* grafo) ;
void imprimeGrafo(Grafo* grafo);
Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual);
Aresta* primeiroListaAdj(int v, Grafo* grafo);



/*Funções do algoritmo de kruskal*/
void union_sets(Grafo* grafo,int x, int y);
void insertionSortAresta(Aresta array[], int numero_de_rotas);
void make_set(Grafo* grafo,int x);
void link(Grafo* grafo,int x, int y);
int buscaEmProfundidadeAGM(int vertice, int destino, int* visitado, Aresta* arestas, int numArestas, float *menorPeso);
int find_set(Grafo* grafo, int x);
Aresta*  AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas);
