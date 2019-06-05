#ifndef __automotive_h
#define __automotive_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


typedef int tempo;
typedef int pos;
typedef int cash;

#define INF INT_MAX

#define boolean int
#define TRUE  1
#define FALSE 0

/*
#define ROW_LENGTH 256

#define NO_ELEMENT NULL
#define NO_LIST    NULL
*/

typedef struct _Viaggio viaggio;
struct _Viaggio{
  pos Partenza;
  pos Arrivo;
  tempo OraPartenza;
  tempo OraArrivo;
  cash Premio;
  int *ElencoNodi; /*punta ad una regione di memoria allocata da DijkTragitto in cui la cella di indice 0 è la lunghezza del cammino minimo, successivamente ho
                    i nodi del tragitto più breve tra partenza e arrivo, compresi. L'array termina in -1, poichè la sua lunghezza non è nota a priori*/
  tempo Durata;
};

struct Chiamata {
  tempo Ora;
  char Nome[26];
  viaggio *Richiesta;
};
typedef struct Chiamata call;
typedef call *ptcall;

typedef struct Veicolo car;
struct Veicolo {
  int Numero;
  tempo AttivitaMax;
  tempo AutonomiaMax;
  tempo CAutonomia; /*complementare autonomia: tempo di movimento dall'ultima ricarica*/
  tempo TToTMovimento;
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

typedef struct _evento event;
struct _evento{
  tempo Ora;
  char  Tipo[50];
  int   Auto;
  char  Nome[26];
  viaggio *quest;
  event *next;
};
typedef event *ptevent;

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*Struttura per rappresentare un elemento dello heap*/
typedef struct ElemHeap{
    int Vertice;
    int Peso;
} ElementoHeap;

/*Heap. Se al Max o al min è deciso dalle funzioni usate su di esso*/
typedef struct _Heap{
    int NumElementi;      /* Numero di elementi dello Heap attualmente presenti*/
    int MaxElementi;  /*Numero massimo di elementi che lo Heap può contenere*/
    int *pos;     /*vettore di interi lungo MaxElementi needed for AggiornaDistanza()*/
    ElementoHeap **array;  /*puntatore alla regione di memoria dove terrò i puntatori
                            agli elementi dello heap. Così sarà più facile aggiornarlo*/
} Heap;




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

/*----------------------------------------------------------------------------*/

ElementoHeap* newElemHeap(int v, int dist);
Heap* NuovoHeap(int Num);
void ScambiaElemHeap(ElementoHeap** a, ElementoHeap** b);
void AggMinHeap(Heap* minHeap, int idx);
void BuildminHeap(Heap* minHeap);
void AggMaxHeap(Heap* maxHeap, int idx);
void BuildMaxHeap(Heap* maxHeap);
int isEmpty(Heap* Heap);
ElementoHeap* extractMin(Heap* minHeap);
ElementoHeap* extractMax(Heap* maxHeap);
void AggiornaDistanza(Heap* minHeap, int v, int dist);
boolean isInmHeap(Heap *minHeap, int v);
int dijkstra(grafo* graph, int src, int dst);
int *DijkTragitto(grafo *Rete, int src,int dst);
void PrintChiamViaggio (ptcall *Chiamate, grafo *Rete, int NumChiamate);
#endif
