/* heapsort.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie e definizione di tipi */

typedef int* vint;

void InterpretaLineaComando (int argc, char *argv[], char *filemosse);

void CaricaVettoreInteri (char *filedati, vint *V, int *pn);

void StampaVettoreInteri (vint V, int n);

void HeapSort (vint V, int n);

void BuildHeap (vint V, int n);

void Scambia (int *pa, int *pb);

void AggiornaHeap (vint V, int n, int i);

int LeftRoot (vint V, int i);

int RightRoot (vint V, int i);


/* Programma principale */
int main (int argc, char *argv[])
{
  char filedati[ROW_LENGTH];
  vint V;
  int n;


  /* Legge da linea di comando il file che contiene i dati */
  InterpretaLineaComando(argc,argv,filedati);

  /* Legge la lunghezze e gli elementi del vettore */
  CaricaVettoreInteri(filedati,&V,&n);

  /* Ordina il vettore */
  HeapSort(V,n);

  /* Stampa il vettore ordinato */
  StampaVettoreInteri(V,n);

  /* Dealloca il vettore (anche per controllare eventuali errori di accesso) */
  free(V);
  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

/* Legge dalla linea di comando il file che contiene i dati */
void InterpretaLineaComando (int argc, char *argv[], char *filedati)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(filedati,argv[1]);
}


/* Carica dal file filedati nel vettore V di dimensione n i numeri da ordinare */
void CaricaVettoreInteri (char *filedati, vint *pV, int *pn)
{
  FILE *fp;
  int i;


  fp = fopen(filedati,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"File %s non apribile!\n",filedati);
    exit(EXIT_FAILURE);
  }

  *pn = 0;
  while (fscanf(fp,"%d",&i) == 1)
    (*pn)++;


  *pV = (int *) calloc(*pn+1,sizeof(int));
  if (*pV == NULL)
  {
    fprintf(stderr,"Allocazione fallita!\n");
    exit(EXIT_FAILURE);
  }

  rewind(fp);

  i = 1;
  while (fscanf(fp,"%d",&(*pV)[i]) == 1)
    i++;

  fclose(fp);
}


/* Stampa il vettore */
void StampaVettoreInteri (vint V, int n)
{
  int i;

  for (i = 1; i <= n; i++)
    printf("%d ",V[i]);
  printf("\n");
}

void HeapSort (vint V, int n)
{
  int j;


  BuildHeap(V,n);
  for (j = n; j > 1; j--)
  {
    Scambia(&V[1],&V[j]);
    AggiornaHeap(V,j-1,1);
  }
}


void BuildHeap (vint V, int n)
{
  int i;

  for (i = n/2; i >= 1; i--)
    AggiornaHeap(V,n,i);
}


void Scambia (int *pa, int *pb)
{
  int temp;

  temp = *pa;
  *pa = *pb;
  *pb = temp;
}

void AggiornaHeap (vint V, int n, int i)
{
  int s, d;
  int iMax;

  s = LeftRoot(V,i);  /* 2*i */
  d = RightRoot(V,i); /* 2*i+1 */

  iMax = i;
  if ( (s <= n) && (V[iMax] < V[s]) ) iMax = s;
  if ( (d <= n) && (V[iMax] < V[d]) ) iMax = d;

  if (iMax != i)
  {
    Scambia(&V[iMax],&V[i]);
    AggiornaHeap(V,n,iMax);
  }
}


int LeftRoot (vint V, int i)
{
  return 2*i;
}


int RightRoot (vint V, int i)
{
  return 2*i+1;
}







