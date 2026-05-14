#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "auxiliares.h"

// perguntar de compensa preencher a primeiras casa do mcfic com o VMP
// sobre o gamma, o puca fez aleatorio, pq ?? vake a pena ?

void vizinhoMaisProximo(Problema *p, int rota[]) {
    int *visitado = (int*)(malloc(sizeof(int)* p->n));
    memset(visitado, 0, sizeof(int) * p->n); // preenchendo tudo com 0
    
    // começa na cidade 0
    rota[0] = 0;
    visitado[0] = 1;
    
    for (int i = 1; i < p->n; i++) { // monta a rota
        int atual = rota[i-1];
        int melhor_viz = -1;
        double melhor_dist = 1e18; // numero mto alto
 
        // procura a cidade nao visitada mais proxima
        for (int j = 0; j < p->n; j++) {
            if (!visitado[j] && p->dist[atual][j] < melhor_dist) {
                melhor_dist = p->dist[atual][j];
                melhor_viz = j;
            }
        }
        
        rota[i] = melhor_viz;
        visitado[melhor_viz] = 1;
    }

    free(visitado);
}

void mcfic(Problema *p,int rota[]) {
    int *fora = (int*)(malloc(sizeof(int) * p->n));
    int nFora = 0;

    rota[0] = 0;
    for (int i = 1; i < p->n; i++)
        fora[nFora++] = i;

    int tamanhoRota = 1;

    // // acha o vizinho mais proximo do deposito
    // int maisProximo = 1;
    // double menorDist = dist[0][1];
    // for (int i = 2; i < n; i++) {
    //     if (dist[0][i] < menorDist) {
    //         menorDist = dist[0][i];
    //         maisProximo = i;
    //     }
    // }

    // // inicializa o ciclo com deposito e vizinho mais proximo
    // rota[0] = 0;
    // rota[1] = maisProximo;
    // tamanhoRota = 2;

    // // remove maisProximo de fora[]
    // for (int i = 0; i < nFora; i++) {
    //     if (fora[i] == maisProximo) {
    //         fora[i] = fora[nFora - 1];
    //         nFora--;
    //         break;
    //     }
    // }
    

    // repete ate todos os clientes entrarem na rota
    while (nFora > 0) {

        int melhorCliente = -1;
        int melhorPosicao = -1;
        double melhorCusto = 1e18;

        // para cada cliente ainda fora
        for (int f = 0; f < nFora; f++) {
            int k = fora[f];

            // testa inserir k em cada posicao da rota
            for (int pos = 0; pos < tamanhoRota; pos++) {
                int i = rota[pos];
                int j = rota[(pos + 1) % tamanhoRota];

                // custo de inserir k entre i e j
                double custo = p->dist[i][k] + p->dist[k][j] - p->dist[i][j] 
                /* - gamma * (dist[0][k] + dist[k][0]) */;

                if (custo < melhorCusto) {
                    melhorCusto   = custo;
                    melhorCliente = f;
                    melhorPosicao = pos;
                }
            }
        }

        // desloca o array para abrir espaco na posicao melhorPosicao+1
        for (int x = tamanhoRota; x > melhorPosicao + 1; x--)
            rota[x] = rota[x - 1]; 

        // insere o cliente na posicao correta
        rota[melhorPosicao + 1] = fora[melhorCliente];

        // remove o cliente inserido de fora[],coloca o ultimo no lugar e diminui nFora
        fora[melhorCliente] = fora[nFora - 1];
        nFora--;

        tamanhoRota++;
    }
    free(fora);
}

bool doisOpt(Problema *p,int rota[]) {
    double melhorDelta = 0.0;
    int k = -1;
    int t = -1;    
    bool melhorou = false;

    for (int i = 0; i < p->n - 1; i++) {
        for (int j = i + 2; j < p->n; j++) {
        
            // custo das arestas atuais
            double atual = p->dist[rota[i]][rota[i+1]] 
                         + p->dist[rota[j]][rota[(j+1) % p->n]]; // se for 0 retorna até o final
            
                         // custo das arestas novas
            double novo = p->dist[rota[i]][rota[j]] 
                        + p->dist[rota[i+1]][rota[(j+1) % p->n]];

            double delta = novo - atual ;
                
                        // se melhorou, inverte o trecho
            if (delta < -TOLERANCIA) { // para diferenca minima
                if(delta < melhorDelta - TOLERANCIA){
                    melhorDelta = delta;
                    k = i;
                    t = j ;
                    melhorou = true;
                }
            }
        }
    }   

    if(melhorou)
        inverter(rota,k+1,t);

    return melhorou;
}

bool orOpt1(Problema *p,int rota[]){
    double melhorDelta = 0.0;
    int k = -1;
    int t = -1;    
    bool melhorou = false;
    
    
    for (int j = 1; j < p->n; j++) {
        int ant = (j - 1 + p->n) % p->n;   // so para tratar o primeiro caso e o utimo
        int prox = (j + 1) % p->n;    

        double custo_remocao = p->dist[rota[ant]][rota[j]] // tira a anterior com a de agr
                                + p->dist[rota[j]][rota[prox]] // a anterior com a proxima 
                                - p->dist[rota[ant]][rota[prox]]; // a nova conexao
        
        
        
        for (int i = 0; i < p->n; i++) {
            if (i == ant || i == j) 
                continue; // mesma posição, pula

            int iProx = (i + 1) % p->n;

            
            double custo_insercao = p->dist[rota[i]][rota[j]] // o atual ate o novo
                                    + p->dist[rota[j]][rota[iProx]] // o novo ate o proximo do i
                                    - p->dist[rota[i]][rota[iProx]]; // tira a distancia dos dois 

            double delta = custo_insercao - custo_remocao;

            if (delta < - TOLERANCIA){//numero mto pequeno
                if (delta < melhorDelta - TOLERANCIA) {
                    melhorDelta = delta;
                    t = j;
                    k = i;
                    melhorou = true;
                }
            }   
        }
    }

    if (melhorou)
        insercao(rota, t, k);

    return melhorou;
}

bool orOpt2(Problema *p,int rota[]) {
    double melhorDelta = 0.0;
    int k = -1;
    int t = -1;    
    bool melhorou = false;
    bool mInvertido = false;
    
    
    // j inicio bloco
    for (int j = 0; j < p->n - 1; j++) {
        int ant  = (j - 1 + p->n) % p->n;
        int fim  = j + 1;            // segundo elemento do bloco
        int prox = (j + 2) % p->n;      // vizinho apos o bloco

        // custo de remover
        double custo_remocao = p->dist[rota[ant]][rota[j]]
                                + p->dist[rota[fim]][rota[prox]]
                                - p->dist[rota[ant]][rota[prox]];

        for (int i = 0; i < p->n; i++) {
            // pula posicoes que  sao do bloco
            if (i == ant || i == j || i == fim) 
                continue;

            int iProx = (i + 1) % p->n;

            // j fim
            double delta_normal = p->dist[rota[i]][rota[j]]
                                + p->dist[rota[fim]][rota[iProx]]
                                - p->dist[rota[i]][rota[iProx]]
                                - custo_remocao;

            // fim j
            double delta_inv = p->dist[rota[i]][rota[fim]]
                                + p->dist[rota[j]][rota[iProx]]
                                - p->dist[rota[i]][rota[iProx]]
                                - custo_remocao;
            double delta; 
            bool invertido = false;

            if(delta_normal < delta_inv){
                delta = delta_normal;
                invertido = false;
            }
            else{ 
                delta = delta_inv;
                invertido = true;   
            }

            if (delta < -TOLERANCIA) {
                if (delta < melhorDelta - TOLERANCIA) {
                    melhorDelta = delta;
                    t = j;
                    k = i;
                    mInvertido = invertido;
                    melhorou = true;
                }
            }
        }
    }

    if (melhorou) {
        if (!mInvertido) {
            insercao(rota, t + 1, k);
            insercao(rota, t, k);
        } else {
            insercao(rota, t, k);
            insercao(rota, t + 1, k);
        }
    }

    return melhorou;
}

bool orOpt3(Problema *p,int rota[]) {
    double melhorDelta = 0.0;
    int k = -1;
    int t = -1;    
    bool melhorou = false;
    bool mInvertido = false;

    // j = inicio do bloco [j, j+1, j+2]
    for (int j = 0; j < p->n - 2; j++) {
        int ant  = (j - 1 + p->n) % p->n;
        int meio = j + 1;
        int fim  = j + 2;            // terceiro elemento do bloco
        int prox = (j + 3) % p->n;      // vizinho apos o bloco

        // custo de remover o bloco [j, meio, fim] de onde esta
        double custo_remocao = p->dist[rota[ant]][rota[j]]
                                + p->dist[rota[fim]][rota[prox]]
                                - p->dist[rota[ant]][rota[prox]];

        for (int i = 0; i < p->n; i++) {
            if (i == ant || i == j || i == meio || i == fim) continue;

            int iProx = (i + 1) % p->n;

            // --- movimento normal: insere [j, meio, fim] ---
            double delta_normal = p->dist[rota[i]][rota[j]]
                                + p->dist[rota[fim]][rota[iProx]]
                                - p->dist[rota[i]][rota[iProx]]
                                - custo_remocao;

            // --- movimento invertido: insere [fim, meio, j] ---
            double delta_inv = p->dist[rota[i]][rota[fim]]
                                + p->dist[rota[j]][rota[iProx]]
                                - p->dist[rota[i]][rota[iProx]]
                                - custo_remocao;

            double delta;
            bool invertido = false;

            if(delta_normal < delta_inv){
                delta = delta_normal;
                invertido = false;
            }
            else {
                delta = delta_inv ;
                invertido = true;
            }

            if(delta < - TOLERANCIA){
                if(delta < melhorDelta -TOLERANCIA){
                    melhorDelta = delta;
                    t = j;
                    k = i;
                    mInvertido = invertido;
                    melhorou = true;
                }
            }
        }
    }
    if (melhorou) {
        if (!mInvertido) {
            insercao(rota, t + 2, k);
            insercao(rota, t + 1, k);
            insercao(rota, t, k);
        } else {
            insercao(rota, t, k);
            insercao(rota, t + 1, k);
            insercao(rota, t + 2, k);
        }
    }

    return melhorou;
}

bool exchange(Problema *p,int rota[]) {
    
    double melhorDelta = 0.0;
    int k = -1;
    int t = -1;    
    bool melhorou = false;
    
    for (int i = 1; i < p->n - 1; i++) {
        for (int j = i + 2; j < p->n; j++) {

            int iAnt  = (i - 1 + p->n) % p->n;
            int iProx = (i + 1) % p->n;
            int jAnt  = (j - 1 + p->n) % p->n;
            int jProx = (j + 1) % p->n;

            // custo atual: 4 arestas em volta dos dois clientes
            double atual = p->dist[rota[iAnt]][rota[i]]
                            + p->dist[rota[i]][rota[iProx]]
                            + p->dist[rota[jAnt]][rota[j]]
                            + p->dist[rota[j]][rota[jProx]];

            // custo apos a troca
            double novo = p->dist[rota[iAnt]][rota[j]]
                        + p->dist[rota[j]][rota[iProx]]
                        + p->dist[rota[jAnt]][rota[i]]
                        + p->dist[rota[i]][rota[jProx]];
            
            double delta = novo - atual;

            if (delta <- TOLERANCIA) {
                if (delta < melhorDelta - TOLERANCIA) {
                    melhorDelta = delta;
                    k = i;
                    t = j;
                    melhorou = true;
                }
            }
        }
    }

    if (melhorou)
        troca(rota, k, t);

    return melhorou;

}

void vnd(Problema *p,int rota[]) {
    int melhorou = 1;
    while (melhorou) {
        melhorou = 0;

        if (orOpt1(p,rota))   { melhorou = 1; continue; }
        if (orOpt2(p,rota))   { melhorou = 1; continue; }
        if (orOpt3(p,rota))   { melhorou = 1; continue; }
        if (exchange(p,rota)) { melhorou = 1; continue; }
        if (doisOpt(p,rota))  { melhorou = 1; continue; }
    }
}

void vndI(Problema *p,int rota[]) {
    int melhorou = 1;
    while (melhorou) {
        melhorou = 0;

        if (doisOpt(p,rota))  { melhorou = 1; continue; }
        if (exchange(p,rota)) { melhorou = 1; continue; }
        if (orOpt3(p,rota))   { melhorou = 1; continue; }
        if (orOpt2(p,rota))   { melhorou = 1; continue; }
        if (orOpt1(p,rota))   { melhorou = 1; continue; }
    }
}

void rvnd(Problema *p,int rota[]) {
    // 0 orOpt1 1 orOpt2 2 orOpt3 3 exchange 4 doisOpt 
    int viz[] = {0, 1, 2, 3, 4};
    int tam = 5;
    int melhorou;

    embaralha(viz, tam);

    int i = 0;
    while (i < tam) {
        melhorou = 0;

        switch (viz[i]) {
            case 0: melhorou = orOpt1(p,rota);   break;
            case 1: melhorou = orOpt2(p,rota);   break;
            case 2: melhorou = orOpt3(p,rota);   break;
            case 3: melhorou = exchange(p,rota);  break;
            case 4: melhorou = doisOpt(p,rota);   break;
        }

        if (melhorou) {
            embaralha(viz, tam); // muda dnv
            i = 0;
        } else {
            i++; // vai pra proxima escolha
        }
    }
}
