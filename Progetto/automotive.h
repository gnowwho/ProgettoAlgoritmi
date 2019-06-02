#ifndef __automotive_h
#define __automotive_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef int tempo;
typedef int pos;
typedef int cash;

#define boolean int
#define TRUE  1
#define FALSE 0

/*
#define ROW_LENGTH 256

#define NO_ELEMENT NULL
#define NO_LIST    NULL
*/


struct Chiamata {
  tempo Ora;
  char Nome[50];
  pos Partenza;
  pos Arrivo;
  tempo OraPartenza;
  tempo OraArrivo;
  cash Premio;

};
typedef struct Chiamata call;
typedef call *ptcall;

typedef struct Veicolo car;
struct Veicolo {
  int Numero;
  tempo AttivitaMax;
  tempo AutonomiaMax;
  tempo CAutonomia; /*complementare autonomia: tempo di movimento dall'ultima ricarica*/
  pos posizione;
  boolean Libera;
};

typedef struct _Arco arco;
struct _Arco{
  int indice; /*indice di questo nodo, l'indice del padre è la sua posizione nel grafo+1 (array starts at 0)*/
  tempo peso; /*distanza tra il padre e questo nodo*/
  arco *fratello; /*fratello successivo in nessun ordine particolare*/
};

typedef arco *nodo;
typedef struct _grafo{
   nodo *ListaNodi;
   int NumeroNodi;
 } grafo;

typedef struct _Viaggio viaggio;
struct _Viaggio{
  pos Partenza;
  pos Arrivo;
  tempo OraPartenza;
  tempo OraArrivo;
  cash Premio;
  int *ElencoNodi;
  tempo Durata;
};

typedef struct _evento event;
struct _evento{
  tempo Ora;
  char  Tipo[50];
  int   Auto;
  char  Nome[50];
  viaggio *quest;
  event *next;
};
typedef event *ptevent;

ptcall *getcalls (FILE *fp, int *hmcalls);

void printcalls (ptcall *vettcall, int k);

void freecalls (ptcall *vettcall, int k);

void printclients (ptcall *vettcall, int k);

void QuickSort (ptcall *V, int s, int d);
int Partition (ptcall *V, int s, int d);
void ScambiaChiamate (ptcall *pa, ptcall *pb);

ptevent AddEvent (ptevent pne, ptevent poe);

ptevent CallToEvent (ptcall tel);

ptevent ImportaEventoChiamate (ptcall *chiamate, int num);

void printevent(ptevent primo);

grafo *NewGraph (int NumNodi);

nodo NuovoArcoSlegato (int destinazione, int peso);

void NuovoArco (int part, int fine, int peso, grafo *Rete);

grafo *getgraph (FILE *fp);

void printgraph (grafo *rete);

#endif
