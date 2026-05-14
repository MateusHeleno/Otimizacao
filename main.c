#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "auxiliares.h"
#include "otim.h"

int main(int argc, char *argv[]) {
    clock_t inicio, fim;
    srand(time(NULL));

    if (argc < 2) {
        printf("Erro no nome da escrita\n");
        printf("Use: %s <nome_da_instancia.tsp>\n", argv[0]);
        return 0;
    }
    
    Problema p;
    leEntrada(&p,argv[1]);

    // monta a matriz de distancias
    montarMatriz(&p);

    // imprimirMatriz();
   
    int *rota1 =(int*)(malloc(sizeof(int)* p.n));
    int *rota1M =(int*)(malloc(sizeof(int)* p.n));
    int *rota2 =(int*)(malloc(sizeof(int)* p.n));
    int *rota2M =(int*)(malloc(sizeof(int)* p.n));

    inicio = clock();

    vizinhoMaisProximo(&p,rota1);
    vizinhoMaisProximo(&p,rota2);

    mcfic(&p,rota1M);
    mcfic(&p,rota2M);


    vnd(&p,rota1);
    printf("--- APOS VND-1---\n");
    imprimirRota(&p,rota1);
    printf("Termino VND ---\n\n");

    rvnd(&p,rota1M);
    printf("--- APOS RVND-1M---\n");
    imprimirRota(&p,rota1M);
    printf("Termino RVND-1M---\n\n");

    vnd(&p,rota2M);
    printf("--- APOS VND-2M---\n");
    imprimirRota(&p,rota2M);
    printf("Termino VND-2M---\n\n");

    rvnd(&p,rota2);
    printf("--- APOS RVND-2---\n");
    imprimirRota(&p,rota2);
    printf("Termino RVND-2---\n\n");


    fim = clock();

    double tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
    printf("Tempo de execucao: %f segundos\n", tempo);
    int otimo = procuraSolucaoLit(argv[1]);
    printf("%s: %d\n",argv[1],otimo);

    liberaProblema(&p);

    free(rota1);
    free(rota1M);
    free(rota2);
    free(rota2M);

    return 0;
}
