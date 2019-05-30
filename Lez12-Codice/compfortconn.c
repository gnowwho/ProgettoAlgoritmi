/* compconn.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "grafo-fs.h"
#include "intqueue.h"
#include "intstack.h"


#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie e definizione di tipi */

typedef int *vint;

/* Legge dalla linea di comando il file che contiene il grafo da visitare */
void InterpretaLineaComando (int argc, char *argv[], char *filedati);

void CaricaGrafo (char *filedati, grafo *pG, boolean Orientato);

void StampaGrafo (grafo *pG);

/* Calcola le nc componenti connesse del grafo G e le salva in C */
void ComponentiConnesse (grafo *pG, vint C, int *pnc);

/* Stampa le nc componenti connesse conservate nel vettore C di lunghezza n */
void StampaComponentiConnesse (int nc, vint C, int n);

/* Visita in ampiezza la componente nc del grafo G partendo dal nodo s
   e marcando gli elementi nel vettore C */
void BFS (grafo *pG, int s, vint C, int nc);

/* Visita in profondita' la componente nc del grafo G partendo dal nodo s
   e marcando gli elementi nel vettore C */
void DFS (grafo *pG, int s, vint C, int nc);

/* Visita in profondita' in modo ricorsivo la componente nc del grafo G partendo dal nodo s
   e marcando gli elementi nel vettore C */
void DFSricorsiva (grafo *pG, int s, vint C, int c);

/* Calcola le nc componenti fortemente connesse del grafo G e le salva in C */
void ComponentiFortementeConnesse (grafo *pG, vint C, int *pnc);

void CreaGrafoTrasposto (grafo *pG, grafo *pGT);

void Intersezione (int n, vint FC, vint BC, int c, vint C);


/* Programma principale */
int main (int argc, char *argv[])
{
  char filedati[ROW_LENGTH];
  grafo G;
  vint C;
  int nc;


  /* Legge da linea di comando il file che contiene il grafo da visitare */
  InterpretaLineaComando(argc,argv,filedati);

  /* Carica il grafo */
  CaricaGrafo(filedati,&G,TRUE);

  /* Stampa a video il grafo */
  printf("Grafo: ");
  StampaGrafo(&G);

  /* Determina le componenti connesse */
  C = (vint) calloc(G.n+1,sizeof(int));
  if (C == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore C!\n");
    exit(EXIT_FAILURE);
  }
  ComponentiFortementeConnesse(&G,C,&nc);

  /* Stampa le componenti connesse */
  StampaComponentiConnesse(nc,C,G.n);

  /* Dealloca le strutture dati dinamiche */
  distrugge_grafo(&G);
  free(C);

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


/* Calcola le nc componenti connesse del grafo G e le salva in C */
void ComponentiConnesse (grafo *pG, vint C, int *pnc)
{
  int v;

  *pnc = 0;
  for (v = 1; v <= pG->n; v++)
    C[v] = 0;

  for (v = 1; v <= pG->n; v++)
    if (C[v] == 0)
    {
      (*pnc)++;
      BFS(pG,v,C,*pnc);
      //DFS(pG,v,C,*pnc);
      //DFSricorsiva(pG,v,C,*pnc);
    }
}


/* Stampa le nc componenti connesse conservate nel vettore C di lunghezza n */
void StampaComponentiConnesse (int nc, vint C, int n)
{
  int v, c;

  printf("Numero componenti: %d\n",nc);
  printf("C = [ ");
  for (v = 1; v <= n; v++)
    printf("%d ",C[v]);
  printf("]\n");

  for (c = 1; c <= nc; c++)
  {
    printf("U[%d] = ",c);
    for (v = 1; v <= n; v++)
      if (C[v] == c) printf("%d ",v);
    printf("\n");
  }
}


/* Visita in ampiezza la componente nc del grafo G partendo dal nodo s
   e marcando gli elementi nel vettore C */
void BFS (grafo *pG, int s, vint C, int nc)
{
  intqueue Q;
  posarco pa;
  int v, w;


  /*Crea una coda vuota */
  CreateQueue(&Q,pG->n);

  /* Inserire v nella coda e marcarlo con nc */
  C[s] = nc;
  Enqueue(s,&Q);

  /* Finche' la coda non e' vuota... */
  while (!IsEmptyQueue(&Q))
  {
    /* Estrarre l'elemento in testa */
    v = Front(&Q);
    Dequeue(&Q);

    /* Per tutti i vertici adiacenti */
    for (pa = primoarcoFS(pG,v); !finearchiFS(pG,v,pa); pa = succarcoFS(pG,v,pa))
    {
      w = leggedest(pG,v,pa);
      if (C[w] == 0)
      {
        /* Marcarli con nc e inserirli nella coda */
        C[w] = nc;
        Enqueue(w,&Q);
      }
    }
  }

  DestroyQueue(&Q);
}


/* Visita in profondita' la componente nc del grafo G partendo dal nodo s
   e marcando gli elementi nel vettore C */
void DFS (grafo *pG, int s, vint C, int nc)
{
  intstack S;
  posarco pa;
  int v, w;


  /*Crea una pila vuota */
  CreateStack(&S,pG->n);

  /* Inserire v nella pila e marcarlo con nc */
  C[s] = nc;
  Push(s,&S);

  /* Finche' la pila non e' vuota... */
  while (!IsEmptyStack(&S))
  {
    /* Estrarre l'elemento in testa */
    v = Top(&S);
    Pop(&S);

    /* Per tutti i vertici adiacenti */
    for (pa = primoarcoFS(pG,v); !finearchiFS(pG,v,pa); pa = succarcoFS(pG,v,pa))
    {
      w = leggedest(pG,v,pa);
      if (C[w] == 0)
      {
        /* Marcarli con nc e inserirli nella coda */
        C[w] = nc;
        Push(w,&S);
      }
    }
  }

  DestroyStack(&S);
}


/* Visita in profondita' in modo ricorsivo la componente nc del grafo G partendo dal nodo v
   e marcando gli elementi nel vettore C */
void DFSricorsiva (grafo *pG, int v, vint C, int c)
{
  int w;
  posarco pa;

  C[v] = c;
  for (pa = primoarcoFS(pG,v); !finearchiFS(pG,v,pa); pa = succarcoFS(pG,v,pa))
  {
    w = leggedest(pG,v,pa);
    if (C[w] == 0)
      DFSricorsiva(pG,w,C,c);
  }
}


/* Calcola le nc componenti fortemente connesse del grafo G e le salva in C */
void ComponentiFortementeConnesse (grafo *pG, vint C, int *pnc)
{
  vint FC, BC;
  grafo GT;
  int v, i;

  *pnc = 0;
  for (v = 1; v <= pG->n; v++)
    C[v] = 0;

  FC = (vint) calloc(pG->n+1,sizeof(int));
  if (C == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore FC!\n");
    exit(EXIT_FAILURE);
  }

  BC = (vint) calloc(pG->n+1,sizeof(int));
  if (C == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore BC!\n");
    exit(EXIT_FAILURE);
  }

  CreaGrafoTrasposto(pG,&GT);

  for (v = 1; v <= pG->n; v++)
    if (C[v] == 0)
    {
      (*pnc)++;

      for (i = 1; i <= pG->n; i++)
        FC[i] = 0;
      BFS(pG,v,FC,*pnc);

      for (i = 1; i <= pG->n; i++)
        BC[i] = 0;
      BFS(&GT,v,BC,*pnc);

      Intersezione(pG->n,FC,BC,*pnc,C);
    }

  free(FC);
  free(BC);
  distrugge_grafo(&GT);
}


void CreaGrafoTrasposto (grafo *pG, grafo *pGT)
{
  int i, j;
  posarco p;


  crea_grafo(pG->n,pGT);
  for (i = 1; i <= pG->n; i++)
    for (p = primoarcoFS(pG,i); !finearchiFS(pG,i,p); p = succarcoFS(pG,i,p))
    {
      j = leggedest(pG,i,p);
      ins_arco(j,i,pGT);
    }
  printf("\n");
}


void Intersezione (int n, vint FC, vint BC, int c, vint C)
{
  int i;

  for (i = 1; i <= n; i++)
    if ( (FC[i] == c) && (BC[i] == c) ) C[i] = c;
}
