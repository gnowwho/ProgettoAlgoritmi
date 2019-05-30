#ifndef __grafofs_h
#define __grafofs_h

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
  listaarchi *FS;
};


/* Crea un grafo *pG di n nodi, senza archi */
void crea_grafo (int n, grafo *pG);

/* Distrugge il grafo *pG */
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

boolean finearchi (grafo *pG, posarco pa);

posarco succarco (grafo *pG, posarco pa);

void leggeestremi (grafo *pG, posarco pa, nodo *porig, nodo *pdest);

posarco primoarcoFS (grafo *pG, nodo orig);

boolean finearchiFS (grafo *pG, nodo orig, posarco pa);

posarco succarcoFS (grafo *pG, nodo orig, posarco pa);

nodo leggedest (grafo *pG, nodo orig, posarco pa);

#endif
