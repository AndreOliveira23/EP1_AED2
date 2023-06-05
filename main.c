#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>

#include "grafo_listaadj.h"
//#include "grafo_matrizadj.h"


int main(int argc, char **argv){
    
    FILE* arquivo_de_entrada;
    arquivo_de_entrada =  fopen(argv[1],"r");
    
    int numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas;
    int centro_de_origem, centro_de_destino,origem_consulta,destino_consulta;
    float altura_maxima_carreta;

    Grafo g1;



    /*Lê a primeira linha*/
    fscanf(arquivo_de_entrada,"%d %d %d",&numero_de_centros_de_distribuicao, &numero_de_rotas, &quantidade_de_consultas);
  
    printf("%d %d %d\n",numero_de_centros_de_distribuicao, numero_de_rotas, quantidade_de_consultas);
    inicializaGrafo(&g1,numero_de_centros_de_distribuicao);
    imprimeGrafo(&g1);
    Aresta array[numero_de_rotas];

    /*Lê as arestas e pesos*/
    int i;
    for(i=0;i<numero_de_rotas;i++){
        fscanf(arquivo_de_entrada,"%d %d %f",&centro_de_origem, &centro_de_destino, &altura_maxima_carreta);
        insereAresta(centro_de_origem,centro_de_destino,altura_maxima_carreta,&g1);
        array[i].origem = centro_de_origem;
        array[i].destino = centro_de_destino;
        array[i].peso = altura_maxima_carreta;
        printf("%d %d %.1f\n",centro_de_origem, centro_de_destino, altura_maxima_carreta);
    }    


    imprimeGrafo(&g1);
   
    printf("numArestas: %d\n",g1.numArestas);

    Aresta* ArvoreGeradoraMaxima = AgmKruskal(&g1,g1.numArestas,array);

    
    /*Lendo consultas*/
    float* alturas_finais_arquivo = (float*) malloc(quantidade_de_consultas * sizeof(float) + 1);

    int *visitado = (int*) calloc(g1.numVertices, sizeof(int));


    for(i=0;i<quantidade_de_consultas;i++){
       float altura_final = 0.0;
       fscanf(arquivo_de_entrada,"%d %d",&origem_consulta, &destino_consulta);
       printf("Consulta: Origem: %d-> Destino: %d\n",origem_consulta, destino_consulta);   
        int *visitadoTemp = (int*) calloc(g1.numVertices, sizeof(int));
        float menorPeso = -1;
        if (buscaEmProfundidadeAGM(origem_consulta, destino_consulta, visitadoTemp, ArvoreGeradoraMaxima, g1.numArestas, &menorPeso)) {
            printf("Existe um caminho entre %d e %d. A aresta de menor peso tem peso %.1f.\n", origem_consulta, destino_consulta, menorPeso);
            //Comparar pesos para carreta
	        
	        float altura_dos_baus[] = {2.5,3.0,3.5,4.0,4.5};
	        float altura_final ;
	        int z=0;
	        for(z=0;z<=4;z++){
		        if(altura_dos_baus[z] <= menorPeso){
			        altura_final = altura_dos_baus[z];	
		        }
	        }	
	        printf("Altura final: %.1f \n",altura_final);
            alturas_finais_arquivo[i] = altura_final;            
        } else {
            printf("Não existe um caminho entre %d e %d.\n", origem_consulta, destino_consulta);
        }
        
        free(visitadoTemp);
    }
    
    free(visitado);
    

    printf("########################################################################################\n");
    


    //Escrevendo arquivo de saída

    FILE *arquivo_de_saida;
    arquivo_de_saida = fopen("saida.txt", "a");
    for(i=0; i < quantidade_de_consultas; i++){
		fprintf(arquivo_de_saida,"%.1f\n",alturas_finais_arquivo[i]);  
    }

    fclose(arquivo_de_saida);
    fclose(arquivo_de_entrada);

    return 0;
}
			
