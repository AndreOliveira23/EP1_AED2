#include<stdio.h>
#include <stdbool.h>

#define MAXNUMVERTICES 100
#define AN -1 //Aresta nula
#define VERTICE_INVALIDO -1

typedef float Peso;

typedef struct{
	Peso mat[MAXNUMVERTICES + 1][MAXNUMVERTICES + 1];
	int numVertices;
	int numArestas;
    	int* vetor_de_pais;
    	int* rank;
}Grafo;

/*Para algoritmo de kruskal*/
typedef struct {
    int origem;
    int destino;
    float peso;
} Aresta;

typedef int Apontador;

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
