#include <stdio.h>
#include <stdbool.h>

#define VERTICE_INVALIDO NULL
#define AN -1

typedef float Peso;

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


bool verificaValidadeVertice(int v, Grafo* grafo);
bool inicializaGrafo(Grafo *grafo, int nv);
bool listaAdjVazia(int v,Grafo* grafo);
Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual);
Aresta* primeiroListaAdj(int v, Grafo* grafo);
bool existeAresta(int v1,int v2, Grafo* grafo);
void insereAresta(int v1,int v2, Peso peso, Grafo* grafo);
bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo* grafo);
void liberaGrafo( Grafo* grafo) ;
void imprimeGrafo(Grafo* grafo);
void make_set(Grafo* grafo,int x);
int find_set(Grafo* grafo, int x) ;
void link(Grafo* grafo,int x, int y);
void union_sets(Grafo* grafo,int x, int y);
void insertionSortAresta(Aresta array[], int numero_de_rotas);
Aresta*  AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas);
int dfs(int vertice, int destino, int* visitado, Aresta* arestas, int numArestas, float *menorPeso);

