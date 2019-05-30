#ifndef __abr_h
#define __abr_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define ROW_LENGTH 256

#define boolean int
#define TRUE  1
#define FALSE 0


#define NO_VAL   0.0
#define NO_TREE  NULL

typedef struct _nodo nodo;
typedef nodo *ABR;

struct _nodo
{
  int val;
  nodo *padre;
  nodo *Ts;
  nodo *Td;
};

ABR creaABR ();

void distruggeABR (ABR *pT);

boolean ABRvuoto (ABR T);

boolean memberABR (int i, ABR T);

ABR costruisceABR (int i, ABR Ts, ABR  Td);

ABR insertABR (int i, ABR T);

ABR deleteABR (int i, ABR T);

int minABR (ABR T);

int maxABR (ABR T);

void stampaABR (ABR T);

#endif
