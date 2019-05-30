/* compconn.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo-fs.h"


#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie e definizione di tipi */

typedef int *vint;

/* Legge dalla linea di comando il file che contiene il grafo da visitare */
void InterpretaLineaComando (int argc, char *argv[], char *filedati);

void CaricaGrafo (char *filedati, grafo *pG, boolean Orientato);

void StampaGrafo (grafo *pG);


/* Programma principale */
int main (int argc, char *argv[])
{
  char filedati[ROW_LENGTH];
  grafo G;


  /* Legge da linea di comando il file che contiene il grafo da visitare */
  InterpretaLineaComando(argc,argv,filedati);

  /* Carica il grafo */
  CaricaGrafo(filedati,&G,FALSE);

  /* Stampa a video il grafo */
  printf("Grafo: ");
  StampaGrafo(&G);

  /* Determina le nc componenti connesse del grafo G nel vettore C */

  /* Stampa le componenti connesse */

  /* Dealloca le strutture dati dinamiche */
  distrugge_grafo(&G);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

/* Legge dalla linea di comando il file che contiene i dati
     e gli indici dei nodi del sottografo */
void InterpretaLineaComando (int argc, char *argv[], char *filedati)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(filedati,argv[1]);
}


void CaricaGrafo (char *filedati, grafo *pG, boolean Orientato)
{
  FILE *fp;
  int i, j, n;

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

  /* Aggiunge gli archi del grafo */
  rewind(fp);
  while (fscanf(fp," (%d,%d)",&i,&j) == 2)
  {
    ins_arco(i,j,pG);
    if (!Orientato) ins_arco(j,i,pG);
  }

  /* Chiude il file */
  fclose(fp);
}


void StampaGrafo (grafo *pG)
{
  int i, j;
  posarco p;


  for (i = 1; i <= pG->n; i++)
    for (p = primoarcoFS(pG,i); !finearchiFS(pG,i,p); p = succarcoFS(pG,i,p))
    {
      j = leggedest(pG,i,p);
      printf("(%d,%d) ",i,j);
    }
  printf("\n");
}
