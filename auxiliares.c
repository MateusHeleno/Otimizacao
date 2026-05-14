#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "otim.h"

void leEntrada(Problema *p,char *nomeArq){
    char nome[512];
    sprintf(nome, "arquivos/%s", nomeArq);

    FILE *arquivo = fopen(nome, "r");

    if (arquivo == NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    char linha[256];

    // le o cabecalho
    while (fgets(linha, sizeof(linha), arquivo)) {
        // pega o n
        if (!strncmp(linha, "DIMENSION", 9)){// se o sprimeiros x caracteres forem iguais 
            char *val = strstr(linha, ":");
            if (val) // val é o ponteiro para os dois pontos
                sscanf(val + 1, "%d", &p->n);
        }

        // cheguei na parte das cidades
        if (!strncmp(linha, "NODE_COORD_SECTION", 18))
            break;
    }

    p->cidades = (Cidade*)(malloc(sizeof(Cidade)*p->n));
    if(!p->cidades){
        printf("Erro de alocacao de memória");
        fclose(arquivo);
        exit(1);
    }

    p->dist = (double**)(malloc(sizeof(double*)*p->n));
    if(!p->dist){
        printf("Erro de alocacao de memoria");
        free(p->cidades);
        fclose(arquivo);
        exit(1);
    }

    for(int i = 0; i < p->n;i++){
        p->dist[i] = (double*)(malloc(sizeof(double)* p->n));
        if(!p->dist[i]){
            printf("Erro de alocaco de memoria");
            for(int j =i-1;j>=0;j--){
                free(p->dist[j]);
            }

            free(p->dist);
            free(p->cidades);
            fclose(arquivo);
            exit(1);
        }
    }

    // le as cidades
    for (int i = 0; i < p->n; i++) 
        fscanf(arquivo,"%d %lf %lf",&p->cidades[i].id,&p->cidades[i].x,&p->cidades[i].y);
    
    fclose(arquivo);
}

void imprimirMatriz(Problema *p) {
    printf("\nMatriz de distancias:\n");
    for (int i = 0; i < p->n; i++) {
        for (int j = 0; j < p->n; j++)
            printf("%.2f  ", p->dist[i][j]);
        printf("\n");
    }
}

// calcula distancia euclidiana entre duas cidades
double distancia(Cidade a, Cidade b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx*dx + dy*dy);
}

// monta a matriz de distancias
void montarMatriz(Problema *p) {
    for (int i = 0; i < p->n; i++)
        for (int j = 0; j < p->n; j++)
            p->dist[i][j] = distancia(p->cidades[i], p->cidades[j]);

}

double calcularCusto(Problema *p,int rota[]) {
    double custo = 0;
    
    for (int i = 0; i < p->n - 1; i++)
        custo += p->dist[rota[i]][rota[i+1]];
    
    // volta a origem
    custo += p->dist[rota[p->n-1]][rota[0]];

    return custo;
    
}

// imprime a rota e seu custo
void imprimirRota(Problema *p,int rota[]) {
    printf("Rota: ");
    for (int i = 0; i < p->n; i++)
        printf("%d -> ", rota[i]);
    printf("%d\n", rota[0]); // volta a origem
    
    printf("Custo: %.10f\n", calcularCusto(p,rota));
}

// inverte o trecho da rota entre os indices i+1 e j
void inverter(int rota[], int i, int j) {
    while (i < j) {
        int aux = rota[i];
        rota[i] = rota[j];
        rota[j] = aux;
        i++;
        j--;
    }
}

void insercao(int rota[], int k, int i) {
    int entrada = rota[k]; // salva a entrada que vai ser movida

    if (k < i) {
        for (int x = k; x < i; x++)
            rota[x] = rota[x + 1]; // aqui puxa todos pra tras e poe na posicao do i
        rota[i] = entrada;

    } else {   
        for (int x = k; x > i + 1; x--)
            rota[x] = rota[x - 1]; // aqui puxa pra frente, ai ja coloca direto na posicao dele i +1
        rota[i + 1] = entrada;
    }
}

void troca(int rota[], int i, int j) {
    int aux = rota[i];
    rota[i] = rota[j];
    rota[j] = aux;
}

void embaralha(int rota[], int tam) {
    for (int i = 0; i < tam; i++) {
        int j = rand() % tam;
        troca(rota, i, j);
    }
}

void liberaProblema(Problema *p){
    if(!p)
        return;

    if(p->dist){
        for(int i = 0;i<p->n;i++){
            if(p->dist[i])
                free(p->dist[i]);
        }
        free(p->dist);
    }

    if(p->cidades)
        free(p->cidades);
};

int contaLetras(char *nomeArq);

int procuraSolucaoLit(char *nomeArq){
    FILE *arquivo = fopen("arquivos/solutions", "r");

    if (arquivo == NULL){
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    char linha[256];

    char *ponto = strchr(nomeArq, '.'); // ponteiro do ponto
    int num ;
    if (ponto != NULL) {
        num = (int)(ponto - nomeArq);
    } else {
        num = strlen(nomeArq);
    }
    int resposta = -1;

    // le o cabecalho
    while (fgets(linha, sizeof(linha), arquivo)) {
        // pega o n
        if (!strncmp(linha, nomeArq, num)){// se o sprimeiros x caracteres forem iguais 
            char *val = strstr(linha, ":");
            if (val){ // val é o ponteiro para os dois pontos
                sscanf(val + 1, "%d", &resposta);
                break;
            }
        }

    }

    fclose(arquivo);
    return resposta;
}

int contaLetras(char *nomeArq){
    int cont = 0;
    while(nomeArq[cont] != '\0'){
        cont++;
    }
    return cont;
}
