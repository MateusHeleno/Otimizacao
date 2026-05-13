#ifndef OTIM_H
#define OTIM_H
 
#include <stdbool.h>

#define TOLERANCIA  1e-10
 
//  Estruturas 
typedef struct {
    int    id;
    double x, y;    
} Cidade;
 
typedef struct{
    int n;
    Cidade *cidades;
    double **dist;
} Problema;

// Heurística de Construção
void vizinhoMaisProximo(Problema *p,int rota[]);
void mcfic(Problema *p,int rota[]);

// Heurísticas de Busca Local
bool doisOpt(Problema *p,int rota[]);
bool orOpt1(Problema *p,int rota[]); 
bool orOpt2(Problema *p,int rota[]);
bool orOpt3(Problema *p,int rota[]);
bool exchange(Problema *p,int rota[]);

// Meta-heurísticas de Busca Local
void vnd(Problema *p,int rota[]);
void vndI(Problema *p,int rota[]);
void rvnd(Problema *p,int rota[]);
 
#endif /* OTIM_H */