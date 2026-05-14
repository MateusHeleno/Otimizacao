#ifndef AUXILIARES_h
#define AUXILIARES_h

#include "otim.h"


void leEntrada(Problema *p,char *nomeArq);
void imprimirMatriz(Problema *p);
double distancia(Cidade a, Cidade b);
void montarMatriz(Problema *p);
double calcularCusto(Problema *p,int rota[]);
void imprimirRota(Problema *p,int rota[]);
 

void inverter(int rota[], int i, int j);
void insercao(int rota[], int k, int i);
void troca(int rota[], int i, int j);
void embaralha(int rota[], int tam);

void liberaProblema(Problema *p);
int procuraSolucaoLit(char *nomeArq);
int contaLetras(char *nomeArq);
bool validarSolucao(Problema *p, int rota[]);

#endif //AUXILIARES_h