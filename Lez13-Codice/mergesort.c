/* mergesort.c */

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

void MergeSort (vint V, int s, int d);

void Merge (vint V, int s, int m, int d);


/* Programma principale------------------------------------------------------------------------------------------------------------------- */
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
  MergeSort(V,1,n);

  /* Stampa il vettore ordinato */
  StampaVettoreInteri(V,n);

  /* Dealloca il vettore (anche per controllare eventuali errori di accesso) */
  free(V);
  return EXIT_SUCCESS;
}
/*-----------------------------------------------------------------------------------------------------------------------------------------*/


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


/* Ordina gli elementi del vettore V con indici compresi fra s e d */
void MergeSort (vint V, int s, int d)
{
  int m;

  if (s < d)
  {
    m = (int) (s+d)/2;
    MergeSort(V,s,m);
    MergeSort(V,m+1,d);
    Merge(V,s,m,d);
  }
}


/* Fonde i sottovettori ordinati V[s,m] e V[m+1,d] in un vettore ordinato V[s,d] */
void Merge (vint A, int s, int m, int d)
{
  int is, id, i;
  vint B;

  B = (vint) calloc(d-s+2,sizeof(int));
  if (B == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione di B!\n");
    exit(EXIT_FAILURE);
  }

  is = s;
  id = m+1;
  i = 1;
  while ( (is <= m) && (id <= d) )                 /*FASE 1: copio il minore elemento iniziale dei due vettori in B finchè uno non è vuoto*/
  {
    if (A[is] <= A[id])
    {
      B[i] = A[is];
      is++;
    }
    else
    {
      B[i] = A[id];
      id++;
    }
    i++;
  }
                                                /*FASE 2*/
  while (is <= m)                               /*se il primo non è vuoto lo copio in coda a B*/
  {
    B[i] = A[is];
    is++;
    i++;
  }

  while (id <= d)                               /*se il secondo non è vuoto lo copio in coda a B*/
  {
    B[i] = A[id];
    id++;
    i++;
  }

  for (i = s; i <= d; i++)                      /*FASE 3: copio B dove devo metterlo davvero, ossia in A*/
    A[i] = B[i-s+1];

  free(B);
}
