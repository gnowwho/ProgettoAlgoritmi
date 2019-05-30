#ifndef __grafo_h
#define __grafo_h

#include <stdio.h>
#include <stdlib.h>


#define TRUE  1
#define FALSE 0
#define boolean int


typedef int nodo;
typedef struct _grafo grafo;

struct _grafo
{
  int n;
  int m;
  boolean **MatriceAdiacenza;
};


/* Crea un grafo *pG di n nodi senza archi */
void crea_grafo (int n, grafo *pG);

/* Distrugge il grafo *pG */
void distrugge_grafo (grafo *pG);

/* Aggiunge l'arco (orig,dest) al grafo *pG.
   Non verifica che l'arco non esista gia'! */
void ins_arco (nodo orig, nodo dest, grafo *pG);

/* Cancella l'arco (orig,dest) dal grafo *pG.
   Non verifica che l'arco esista! */
void canc_arco (nodo orig, nodo dest, grafo *pG);

/* Determina se l'arco (orig,dest) appartiene al grafo *pG */
boolean esiste_arco (nodo orig, nodo dest, grafo *pG);

#endif
