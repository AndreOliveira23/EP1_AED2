#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "grafo_matrizadj.h"

//Variáveis globais para usar no algoritmo de kruskal
int numero_de_vertices;


bool inicializaGrafo(Grafo* grafo, int nv){
	int i,j;
	if(nv > MAXNUMVERTICES){
		fprintf(stderr,"ERRO na chamada de inicializaGrafo: Numero de vértices maior que o máximo permitido de %d.\n",MAXNUMVERTICES);
		return false;
	}
	if(nv <0){
		fprintf(stderr,"ERRO Na chamada de inicializaGrafo: Numero de vértices deve ser positivo.\n");
		return false;
	}

	grafo->numVertices = nv;
	grafo->numArestas = 0;
    grafo->vetor_de_pais = (int*) malloc(nv * sizeof(int));//Make set
    for (int i = 0; i < nv; i++) {
        grafo->vetor_de_pais[i] = i; // atribui o índice atual a cada posição do vetor
    }
    grafo->rank = (int*) malloc(nv * sizeof(int));//Make sET
	for( i = 0; i<= grafo->numVertices; i++){
		for( j = 0; j<= grafo->numVertices; j++){
			grafo->mat[i][j] = AN;
		}
	}
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

void insereAresta(int v1, int v2, Peso peso, Grafo* grafo){
	if(! ( verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
	 return;


    /*Verifica se o caminho de volta já existe)*/
    if((grafo->mat[v1][v2] != AN) && (v1 > v2)){
        printf("Erro! -> A ordem do número dos centros de distribuição deve ser crescente: ");
        grafo->mat[v1][v2] = AN;
        //grafo->numArestas--; 
        return;
    }else{
	    grafo->mat[v1][v2] = peso;
        if(grafo->mat[v2][v1] != AN){
	        
        }else{
            grafo->numArestas++;        
        }
    }
}

bool existeAresta(int v1, int v2, Grafo* grafo){
	if(! ( verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
	 return false;
	if(grafo->mat[v1][v2] != AN) return true;
		else return false;
}


bool removeAresta(int v1, int v2, Peso* peso, Grafo* grafo){
	if(! ( verificaValidadeVertice(v1, grafo) && verificaValidadeVertice(v2, grafo)))
	 return false;

	if(grafo->mat[v1][v2] != AN){
		*peso = grafo->mat[v1][v2];
		grafo->mat[v1][v2];
		grafo->numArestas--;
		return true;
	}

	return false;
}

bool listaADjVazia(int v, Grafo* grafo){
	if(!verificaValidadeVertice(v, grafo))
	  return true;

	int i;
	for(i=1; i<=grafo->numVertices; i++){
		if(grafo->mat[v][i] != AN) return false;
	return true;
	}

}

Apontador proxListaAdj(int v, Grafo* grafo, Apontador atual){
	if( !verificaValidadeVertice(v, grafo))
		return VERTICE_INVALIDO;

	atual++;
	while ((atual <= grafo->numVertices) && (grafo->mat[v][atual] == AN))
		atual++;
	if(atual > grafo->numVertices){
		return VERTICE_INVALIDO;
	}
	return atual;
}

void liberaGrafo(Grafo* grafo){
}

void imprimeGrafo(Grafo* grafo){
	int i,j;
	for(i=1;i<=grafo->numVertices;i++){
		for(j=1;j<=grafo->numVertices;j++){
			printf("[%d][%d] = %.1f | ",i,j,grafo->mat[i][j]);
		}
	printf("\n");
	}
    printf("---------------------------------------------------------------------------------------------------------- \n");
}

/*Variáveis e Funções para o algoritmo de Kruskal*/
/*

void make_set(Grafo* grafo,int x) {
    grafo->vetor_de_pais[x] = x;
    grafo->rank[x] = 0;
}

int find_set(Grafo* grafo, int x) {
    //printf("execução find-set!\n x=%d\n",x);
    if ( x != grafo->vetor_de_pais[x]) {
        //printf("entrou no if...\n");
        //printf("grafo->vetor_de_pais[%d] = %d\n",x,grafo->vetor_de_pais[x]);
        grafo->vetor_de_pais[x] = find_set(grafo,grafo->vetor_de_pais[x]);
    }
    //printf("vai retornar: %d\n",grafo->vetor_de_pais[x]);
    return grafo->vetor_de_pais[x];
}

void link(Grafo* grafo,int x, int y){
    if(grafo->rank[x] > grafo->rank[y]){
        grafo->vetor_de_pais[y] = x;    
    }else{
        grafo->vetor_de_pais[x] = y;    
    }
    
    if(grafo->rank[x] == grafo->rank[y]){
        grafo->rank[y]++;    
    }
}

void union_sets(Grafo* grafo,int x, int y){
    link(grafo,find_set(grafo,x),find_set(grafo,y));
}

void insertionSortAresta(Aresta array[], int numero_de_rotas){
    float i, j;
    Aresta key;
    for (i = 1; i < numero_de_rotas; i++) {
        key = array[(int)i];
        j = i - 1;
        while (j >= 0 && array[(int)j].peso > key.peso) {
            array[(int)j + 1] = array[(int)j];
            j = j - 1;
        }
        array[(int)j + 1] = key;
    }
}


void AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas){
    int i;
    Aresta *ArvoreGeradoraMinima = malloc(numero_de_rotas * sizeof(Aresta));
    int tamanho_arvore = 0;

    insertionSortAresta(array_de_arestas, numero_de_rotas); //Ordenando edges por ordem não-decrescente

    for(i = 0; i < numero_de_rotas; i++){
        if(find_set(grafo, array_de_arestas[i].origem) != find_set(grafo, array_de_arestas[i].destino)){
            ArvoreGeradoraMinima[tamanho_arvore] = array_de_arestas[i];
            tamanho_arvore++;
            union_sets(grafo, array_de_arestas[i].origem, array_de_arestas[i].destino);
            printf("Aresta %d->%d: %.1f é segura e será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].destino, array_de_arestas[i].peso);
        } else {
            printf("Aresta %d->%d: %.1f NÃO é segura e NÃO será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].destino, array_de_arestas[i].peso);       
        }
    }
    
    printf("AGM:\n");
    for(i = 0; i < tamanho_arvore; i++){
       printf("Aresta %d->%d: %.1f\n", ArvoreGeradoraMinima[i].origem, ArvoreGeradoraMinima[i].destino, ArvoreGeradoraMinima[i].peso);
    }
    

    free(ArvoreGeradoraMinima);
}
*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------*/

void make_set(Grafo* grafo, int x) {
    grafo->vetor_de_pais[x] = x;
    grafo->rank[x] = 0;
}

int find_set(Grafo* grafo, int x) {
    if (x != grafo->vetor_de_pais[x]) {
        grafo->vetor_de_pais[x] = find_set(grafo, grafo->vetor_de_pais[x]);
    }
    return grafo->vetor_de_pais[x];
}

void link(Grafo* grafo, int x, int y) {
    if (grafo->rank[x] < grafo->rank[y]) {
        grafo->vetor_de_pais[y] = x;
    } else {
        grafo->vetor_de_pais[x] = y;
    }
    
    if (grafo->rank[x] == grafo->rank[y]) {
        grafo->rank[y]++;
    }
}

void union_sets(Grafo* grafo, int x, int y) {
    link(grafo, find_set(grafo, x), find_set(grafo, y));
}

void sortAresta(Aresta array[], int numero_de_rotas) {
    float i, j;
    Aresta key;
    for (i = 1; i < numero_de_rotas; i++) {
        key = array[(int)i];
        j = i - 1;
        while (j >= 0 && array[(int)j].peso < key.peso) {
            array[(int)j + 1] = array[(int)j];
            j = j - 1;
        }
        array[(int)j + 1] = key;
    }
}


void AgmKruskal(Grafo* grafo, int numero_de_rotas, Aresta* array_de_arestas) {
    int i;
    Aresta* arvoreGeradoraMaxima = malloc(numero_de_rotas * sizeof(Aresta));
    int tamanho_arvore = 0;

    sortAresta(array_de_arestas, numero_de_rotas); // Ordenando arestas por ordem não-crescente

    for (i = numero_de_rotas - 1; i >= 0; i--) {
        if (find_set(grafo, array_de_arestas[i].origem) != find_set(grafo, array_de_arestas[i].destino)) {
            arvoreGeradoraMaxima[tamanho_arvore] = array_de_arestas[i];
            tamanho_arvore++;
            union_sets(grafo, array_de_arestas[i].origem, array_de_arestas[i].destino);
            printf("Aresta %d->%d: %.1f é segura e será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].destino, array_de_arestas[i].peso);
        } else {
            printf("Aresta %d->%d: %.1f NÃO é segura e NÃO será colocada na AGM.\n", array_de_arestas[i].origem, array_de_arestas[i].destino, array_de_arestas[i].peso);       
        }
    }
    
    printf("AGM:\n");
    for (i = tamanho_arvore - 1; i >= 0; i--) {
       printf("Aresta %d->%d: %.1f\n", arvoreGeradoraMaxima[i].origem, arvoreGeradoraMaxima[i].destino, arvoreGeradoraMaxima[i].peso);
    }
    

    free(arvoreGeradoraMaxima);
}


bool buscaEmLargura(Grafo* grafo, int origem, int destino, int* vetor_de_pais) {
    int numVertices = grafo->numVertices;
    bool visitado[numVertices + 1];

    for (int i = 1; i <= numVertices; i++) {
        visitado[i] = false;
    }

    visitado[origem] = true;

    int fila[numVertices];
    int inicio = 0;
    int fim = 0;

    fila[fim++] = origem;

    while (inicio != fim) {
        int verticeAtual = fila[inicio++];
        
        if (verticeAtual == destino) {
            return true;
        }

        for (int i = 1; i <= numVertices; i++) {
            if (!visitado[i] && grafo->mat[verticeAtual][i] > 0) {
                visitado[i] = true;
                fila[fim++] = i;
                vetor_de_pais[i] = verticeAtual;
            }
        }
    }

    return false;
}


void imprimirCaminho(Grafo* grafo, int* vetor_de_pais, int destino) {
float altura_dos_baus[] = {2.5,3.0,3.5,4.0,4.5}; 
     float altura_final;
     int z=0;
    
    if (vetor_de_pais[destino] != -1) {     
         while (altura_dos_baus[z] <= grafo->mat[vetor_de_pais[destino]][destino] && grafo->mat[vetor_de_pais[destino]][destino] != 5.5){
       
             altura_final = altura_dos_baus[z];
             z++;
           
     }
    if(grafo->mat[vetor_de_pais[destino]][destino] >= 4.5){
    altura_final = 4.5;    
    }

    printf("Teste: %.1f\n",grafo->mat[vetor_de_pais[destino]][destino]);
    printf("Test2: %.1f\n",altura_final);
        //imprimirCaminho(grafo, vetor_de_pais, vetor_de_pais[destino]);
        //printf(" -> ");
    }

    //printf("%d", destino);
 
}






/*****************************************************************************************************************************************************/
int main(int argc, char **argv){
    
    FILE* file;
    file =  fopen(argv[1],"r");
    
    int numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas;
    int centro_de_origem, centro_de_destino,origem_consulta,destino_consulta;
    float altura_maxima_carreta;
    int altura_dos_baus[] = {2.5,3.0,2.5,4.0,4.5}; 
    int i,j,x,y,z;
    float c;
    Grafo g1;



    /*Lê a primeira linha*/
    fscanf(file,"%d %d %d",&numero_de_centros_de_distribuicao, &numero_de_rotas, &quantidade_de_consultas);
    numero_de_vertices = numero_de_rotas;
    printf("%d %d %d\n",numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas);
    inicializaGrafo(&g1,numero_de_centros_de_distribuicao);
    imprimeGrafo(&g1);
    Aresta array[numero_de_rotas];
    /*Lê as arestas e pesos*/
    for(i=0;i<numero_de_rotas;i++){
        fscanf(file,"%d %d %f",&centro_de_origem, &centro_de_destino, &altura_maxima_carreta);
        insereAresta(centro_de_origem,centro_de_destino,altura_maxima_carreta,&g1);
        insereAresta(centro_de_destino,centro_de_origem,altura_maxima_carreta,&g1);
        array[i].origem = centro_de_origem;
        array[i].destino = centro_de_destino;
        array[i].peso = altura_maxima_carreta;
        printf("%d %d %.1f\n",centro_de_origem, centro_de_destino, altura_maxima_carreta);
    }    
    imprimeGrafo(&g1);
    printf("numArestas: %d\n",g1.numArestas);
    //encontrarCaminhos(&g1, 0, 7);
    //maiorAlturaCarretaBau(&g1,0,7);
    AgmKruskal(&g1,g1.numArestas,array);
//dijkstra(&g1,0,7);
//dijkstra(&g1,1,6);
//dijkstra(&g1,1,7);
//dijkstra(&g1,6,7);    
int vetor_de_pais[g1.numVertices + 1];
    for (int i = 1; i <= g1.numVertices; i++) {
        vetor_de_pais[i] = -1;
    }
    for(i=0;i<quantidade_de_consultas;i++){
        fscanf(file,"%d %d",&origem_consulta, &destino_consulta);
        printf("Consulta: Origem: %d-> Destino: %d\n",origem_consulta, destino_consulta);
        printf("Caminho encontrado: ");
        if (buscaEmLargura(&g1, origem_consulta,destino_consulta, vetor_de_pais)) {
            imprimirCaminho(&g1,vetor_de_pais, destino_consulta);
            printf("\n");
        } else {
            printf("Nenhum caminho encontrado.\n");
        }
    }
/*
    printf("%.1f\n",maiorCarretaBau(&g1,1,4));
     int vetor_de_pais[g1.numVertices + 1];
    for (int i = 1; i <= g1.numVertices; i++) {
        vetor_de_pais[i] = -1;
    }
        
    printf("Caminho encontrado: ");
    if (buscaEmLargura(&g1, 1,2, vetor_de_pais)) {
        imprimirCaminho(vetor_de_pais, 2);
    } else {
        printf("Nenhum caminho encontrado.");
    }
    printf("\n");
  */

















    /*Lê os caminhos desejados
     for(i=0;i<=2;i++){
        fscanf(file,"%d %d",&x,&y);
        printf("%d %d\n",x,y);
    }
    fclose(file);
    
    Grafo g1;
    if(inicializaGrafo(&g1,10)) printf("true");*/
    return 0;
}
