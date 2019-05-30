#ifndef __grafo_la_h
#define __grafo_la_h

#include <stdio.h>
#include <stdlib.h>

#include "listaarchi.h"


#define TRUE  1
#define FALSE 0
#define boolean int


typedef struct _grafo grafo;

struct _grafo
{
  int n;
  int m;
  listaarchi L;
};


void crea_grafo (int n, grafo *pG);

void distrugge_grafo (grafo *pG);

/* Aggiunge l'arco (orig,dest) al grafo *pG.
   Non verifica che l'arco non esista gia'! */
void ins_arco (int orig, int dest, grafo *pG);

/* Cancella l'arco (orig,dest) dal grafo *pG.
   Non verifica che l'arco esista! */
void canc_arco (int orig, int dest, grafo *pG);

/* Determina se l'arco (orig,dest) appartiene al grafo *pG */
boolean esiste_arco (int orig, int dest, grafo *pG);

posarco primoarco (grafo *pG);

boolean finearchi (posarco pa, grafo *pG);

posarco succarco (posarco pa, grafo *pG);

void leggeestremi (posarco pa, grafo *pG, nodo *porig, nodo *pdest);

#endif
