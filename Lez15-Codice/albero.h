#ifndef __lista_h
#define __lista_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_LENGTH 256

#define boolean int
#define TRUE  1
#define FALSE 0


#define NO_OP  ' '
#define NO_VAL 0.0

typedef char   Operatore;
typedef double Operando;


#define NO_NODE  NULL
#define NO_TREE  NULL

typedef struct _nodo nodo;
typedef nodo *albero;
typedef nodo *posizione;

struct _nodo
{
  Operatore op;
  Operando  val;
  posizione padre;
  posizione Ts;
  posizione Td;
};

albero creaalbero ();

void distruggealbero (albero *pT);

boolean alberovuoto (albero T);

void leggenodo (albero T, posizione p, Operatore *pop, Operando *pval);

void scrivenodo (albero T, posizione p, Operatore op, Operando val);

posizione radice (albero T);

posizione figliosinistro (albero T, posizione p);

posizione figliodestro (albero T, posizione p);

posizione padre (albero T, posizione p);

albero costruiscealbero (Operatore op, Operando val, albero Ts, albero Td);

albero cancsottoalbero (albero T, posizione *pp);

#endif
