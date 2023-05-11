#include <stdio.h>
#include <stdlib.h>

int main(){
    
    FILE* file;
    file =  fopen("entrada.txt","r");
    int i,j,x,y,z;
    float c;
    /*Lê a primeira linha*/
    fscanf(file,"%d %d %d",&x,&y,&z);
    printf("%d %d %d\n",x,y,z);

    /*Lê as arestas e pesos*/
    for(i=0;i<=4;i++){
        fscanf(file,"%d %d %f",&x,&y,&c);
        printf("%d %d %.1f\n",x,y,c);
    }    
    
    /*Lê os caminhos desejados*/
     for(i=0;i<=2;i++){
        fscanf(file,"%d %d",&x,&y);
        printf("%d %d\n",x,y);
    }
/*
    while(fscanf(file,"%d",&x)){
        if(feof(file)) break;        
        printf("%d ",x);
         
    }
*/
    fclose(file);

    return 0;
}
