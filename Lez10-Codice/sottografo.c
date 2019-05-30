/* startgrafo.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo-ma.h"


#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie e definizione di tipi */

typedef int *vint;

/* Legge dalla linea di comando il file che contiene i dati
     e gli indici dei nodi del sottografo */
void InterpretaLineaComando (int argc, char *argv[], char *filedati, vint *pS, int *pns);

void LeggeListaNodi (int argc, char *argv[], vint *pS, int *pns);

void CaricaGrafo (char *filedati, grafo *pG);

void StampaGrafo (grafo *pG);

void StampaSottografoIndotto (grafo *pG, vint S, int ns);

boolean appartiene_tabella (int i, vint S, int ns);


/* Programma principale */
int main (int argc, char *argv[])
{
  char filedati[ROW_LENGTH];
  vint S;
  int ns;
  grafo G;


  /* Legge da linea di comando il file che contiene i dati
     e gli indici dei nodi del sottografo */
  InterpretaLineaComando(argc,argv,filedati,&S,&ns);

  /* Carica il grafo */
  CaricaGrafo(filedati,&G);

  /* Stampa a video il grafo */
  printf("Grafo: ");
  StampaGrafo(&G);

  /* Stampa a video il sottografo indotto */
  printf("Sottografo: ");
  StampaSottografoIndotto(&G,S,ns);

  /* Dealloca il grafo e la tabella degli indici del sottografo */
  distrugge_grafo(&G);
  free(S);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

/* Legge dalla linea di comando il file che contiene i dati
     e gli indici dei nodi del sottografo */
void InterpretaLineaComando (int argc, char *argv[], char *filedati, vint *pS, int *pns)
{
  if (argc <= 1)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(filedati,argv[1]);

  LeggeListaNodi(argc,argv,pS,pns);
}


void LeggeListaNodi (int argc, char *argv[], vint *pS, int *pns)
{
  int p;

  /* Determina il numero dei nodi del sottografo indotto */
  *pns = argc-2;

  /* Alloca la tabella dei nodi del sottografo indotto */
  *pS = (vint) calloc(*pns+1,sizeof(int));
  if (*pS == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore degli indici del sottografo!\n");
    exit(EXIT_FAILURE);
  }

  /* Riempie la tabella dei nodi del sottografo indotto */
  for (p = 1; p <= *pns; p++)
    if (sscanf(argv[p+1],"%d",&(*pS)[p]) != 1)
    {
      fprintf(stderr,"Errore nella lettura degli indici del sottografo: %s!\n",argv[p+1]);
      exit(EXIT_FAILURE);
    }
}


void CaricaGrafo (char *filedati, grafo *pG)
{
  FILE *fp;
  nodo i, j;
  int n;

  /* Apre il file */
  fp = fopen(filedati,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"File %s could not be opened!\n",filedati);
    exit(EXIT_FAILURE);
  }

  /* Conta i nodi del grafo */
  n = 0;
  while (fscanf(fp," (%d,%d)",&i,&j) == 2)
  {
    if (i > n) n = i;
    if (j > n) n = j;
  }

  /* Crea il grafo, assumendo che gli indici dei nodi vadano da 1 a n */
  crea_grafo(n,pG);

  /* Aggiunge gli archi al grafo */
  rewind(fp);
  while (fscanf(fp," (%d,%d)",&i,&j) == 2)
    ins_arco(i,j,pG);

  /* Chiude il file */
  fclose(fp);
}


void StampaGrafo (grafo *pG)
{
  nodo i, j;

  for (i = 1; i <= pG->n; i++)
    for (j = 1; j <= pG->n; j++)
      if (esiste_arco(i,j,pG)) printf("(%d,%d) ",i,j);

  printf("\n");
}


void StampaSottografoIndotto (grafo *pG, vint S, int ns)
{
  nodo i, j;

  /*  n ( f(ns) + n ( f(ns) + O(1) ) ) = O( n^2 f(ns) ) = O(n^2 ns) */
/*  for (i = 1; i <= pG->n; i++)
    if (appartiene_tabella(i,S,ns))
      for (j = 1; j <= pG->n; j++)
        if (appartiene_tabella(j,S,ns) && esiste_arco(i,j,pG))
          printf("(%d,%d) ",i,j);

*/

  /*  ns ns O(1) = O(ns^2) */
  for (i = 1; i <= ns; i++)
    for (j = 1; j <= ns; j++)
      if (esiste_arco(S[i],S[j],pG))
          printf("(%d,%d) ",S[i],S[j]);

  printf("\n");
}


boolean appartiene_tabella (int i, vint S, int ns)
{
  int k;

  for (k = 1; k <= ns; k++)
    if (S[k] == i) return TRUE;

  return FALSE;
}
