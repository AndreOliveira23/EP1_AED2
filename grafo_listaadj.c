#include <stdio.h>
#include <stdlib.h>
#include "grafo_listaadj.h"


bool inicializaGrafo(Grafo *grafo, int nv){
	int i;
	if(nv <= 0){
		fprintf(stderr, "ERRO na chamada de inicializaGrafo: Número de vértices deve ser positivo.\n");
		return false;
	}

	grafo->numVertices = nv;
	if(!(grafo->listaAdj = (Apontador*) calloc(nv+1, sizeof(Apontador)))){
		fprintf(stderr, "ERRO: Falha de alocação de memória na função 'inicalizaGrafo\n");
		return false;
	}

	grafo->vetor_de_pais = (int*) malloc(nv*sizeof(int));//Make Set

	for(i=0;i<nv;i++){
		grafo->vetor_de_pais[i] = i;
	}

	grafo->rank = (int*)malloc(nv*sizeof(int));//Makeset
	grafo->numArestas = 0;
	return true;
}

bool verificaValidadeVertice(int v, Grafo* grafo){
	if(v > grafo->numVertices){
		fprintf(stderr, "Erro: Numero do vertice (%d) maior que o número total de vertices (%d).\n",v,grafo->numVertices);
		return false;
	}
	if(v<0){
		fprintf(stderr, "ERRO: Número do vértice (%d) deve ser positivo.\n",v);
		return false;
	}
	return true;
}


bool listaAdjVazia(int v,Grafo* grafo){
	if(!verificaValidadeVertice(v,grafo))
		return false;
	return (grafo->listaAdj[v]==NULL);
}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual){
	if(atual == NULL){
		fprintf(stderr, "atual  == NULL\n");
		return VERTICE_INVALIDO;
	}
	return(atual->prox);
}

Aresta* primeiroListaAdj(int v, Grafo* grafo){
	return(grafo->listaAdj[v]);
}

bool existeAresta(int v1,int v2, Grafo* grafo){
	Apontador q;

	if(! (verificaValidadeVertice(v1,grafo) && verificaValidadeVertice(v2,grafo)))
		return false;

	q = grafo->listaAdj[v1];
	while((q) && (q->destino != v2))
		q = q->prox;
	if(q != NULL) return true;
	return false;

}

void insereAresta(int v1,int v2, Peso peso, Grafo* grafo){
	Apontador p;

	if(! (verificaValidadeVertice(v1,grafo) && verificaValidadeVertice(v2,grafo)))
		return;

	if(!(p = (Apontador) calloc(1,sizeof(Aresta)) )){
		fprintf(stderr, "ERRO: Falha na alocação de memória na função InsereAresta\n");
	}

    if(v1 > v2){
        fprintf(stderr,"Erro! -> Para a entrada (%d -> %d: %.1f), A ordem do número dos centros de distribuição deve ser crescente! (v1 < v2))\n",v1,v2,peso);
        return;
    }
    
	p->destino = v2;
	p->peso = peso;
	p->prox = grafo->listaAdj[v1];
	grafo->listaAdj[v1] = p;
	grafo->numArestas++;
}

bool removeArestaObtendoPeso(int v1, int v2, Peso* peso, Grafo* grafo){
		Apontador q, ant;

	if(! (verificaValidadeVertice(v1,grafo) && verificaValidadeVertice(v2, grafo)))
		return AN;

	q = grafo->listaAdj[v1];
	while((q != NULL) && (q->destino != v2)){
		ant = q;
		q = q->prox;
	}

	if( q!= NULL){
		if(grafo->listaAdj[v1] == q)
			grafo->listaAdj[v1] = q->prox;
		else ant->prox = q->prox;
		*peso = q->peso;
		q->prox = NULL;
		free(q);
		q = NULL;
		return true;
	}

	return false;
}

void liberaGrafo( Grafo* grafo) {
	int v;
	Apontador p;

	for(v=0;v<= grafo->numVertices; v++){
		p = grafo->listaAdj[v];
		while((p != NULL)) {
			grafo->listaAdj[v] = p->prox;
			p->prox = NULL;
			free(p);
		}
	}
	grafo->numVertices = 0;

	free(grafo->listaAdj);
	grafo->listaAdj = NULL;
}

void imprimeGrafo(Grafo* grafo){
	int i;
	for(i=1;i<=	 grafo->numVertices;i++){
		printf("V[%d]:",i);
		Aresta* atual = grafo->listaAdj[i];
		while(atual != NULL	){
			printf("(%d, peso: %.1f)",atual->destino,atual->peso);
			atual = atual->prox;
		}
	printf("\n");
	}
}


/*Variáveis e Funções para o algoritmo de Kruskal*/


void make_set(Grafo* grafo,int x) {
    grafo->vetor_de_pais[x] = x;
    grafo->rank[x] = 0;
}

int find_set(Grafo* grafo, int x) {
   
    if ( x != grafo->vetor_de_pais[x]) {
        grafo->vetor_de_pais[x] = find_set(grafo,grafo->vetor_de_pais[x]);
    }

    return grafo->vetor_de_pais[x];
}

void link(Grafo* grafo,int x, int y){
    if(grafo->rank[x] > grafo->rank[y]){
        grafo->vetor_de_pais[y] = x;
    }else{
        grafo->vetor_de_pais[x]= y;
    }

    if(grafo->rank[x] == grafo->rank[y]){
        grafo->rank[y]++;
    }
}

void union_sets(Grafo* grafo,int x, int y){
    link(grafo,find_set(grafo,x),find_set(grafo,y));
}

void insertionSortAresta(Aresta array[], int numero_de_rotas){
    int i, j;
    Aresta key;

    for (i = 1; i < numero_de_rotas; i++) {
        key = array[i];
        j = i - 1;
        while (j >= 0 && array[j].peso < key.peso) {
            array[j + 1] = array[j];
            j = j - 1;
        }
        array[j + 1] = key;
    }

}

Aresta*  AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas){

    
    Aresta *ArvoreGeradoraMaxima = malloc(numero_de_rotas * sizeof(Aresta));

    int tamanho_arvore = 0;

    insertionSortAresta(array_de_arestas, numero_de_rotas); //Ordenando edges por ordem não-crescente

    int i;
    for(i = 0; i < numero_de_rotas; i++){
        if(find_set(grafo, array_de_arestas[i].origem) != find_set(grafo, array_de_arestas[i].destino)){
              ArvoreGeradoraMaxima[tamanho_arvore] = array_de_arestas[i];
              tamanho_arvore++;
              union_sets(grafo, array_de_arestas[i].origem, array_de_arestas[i].destino);
              //printf("Aresta %d->%d: %.1f é segura e será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].vdest, array_de_arestas[i].peso);
       	 } else {
              //printf("Aresta %d->%d: %.1f NÃO é segura e NÃO será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].vdest, array_de_arestas[i].peso);       
       }
    }

    printf("AGM:\n");
    for(i = 0; i < tamanho_arvore; i++){
       	printf("Aresta %d->%d: %.1f\n", ArvoreGeradoraMaxima[i].origem, ArvoreGeradoraMaxima[i].destino, ArvoreGeradoraMaxima[i].peso);
    }

    return ArvoreGeradoraMaxima;
}

int buscaEmProfundidadeAGM(int vertice, int destino, int* visitado, Aresta* arestas, int numArestas, float *menorPeso){
	if(vertice == destino){
		return 1;
	}

	visitado[vertice]=1;

	int i;
	for(i=0;i<numArestas;i++){
		if(arestas[i].origem == vertice && !visitado[arestas[i].destino]){
		//Se a aresta parte do vértice atual e o destino ainda não foi visitado
			if(buscaEmProfundidadeAGM(arestas[i].destino,destino,visitado,arestas,numArestas,menorPeso)){
			//Se encontrou o destino a partir desse vértice, verifica se o peso da aresta é menor
				if(*menorPeso == -1 || arestas[i].peso <*menorPeso){
					*menorPeso = arestas[i	].peso;
				}
			return 1;
			}
		}else if(arestas[i].destino == vertice && !visitado[arestas[i].origem]){
			//Se a aresta chega ao vértice atual e a origem ainda não foi visitada
			if(buscaEmProfundidadeAGM(arestas[i].origem,destino,visitado,arestas,numArestas,menorPeso)){
			//Se encontrou o destino a partir desse vértice, verifica se o peso da aresta é menor
				if(*menorPeso == -1 || arestas[i].peso <*menorPeso){
					*menorPeso = arestas[i].peso;
				}
			return 1;
			}
		}
	}
	return 0; //Não encontrou o destino a patir desse vértice

}
/*****************************************************************************************************************************************************/

