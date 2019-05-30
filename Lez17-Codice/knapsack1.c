/* knapsack.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


#define ROW_LENGTH 256

#define boolean int
#define TRUE  1
#define FALSE 0

typedef long * vlong;
typedef boolean * vboolean;

typedef struct _KPdata KPdata;
struct _KPdata
{
  long n;
  long V;
  vlong phi;
  vlong v;
};

typedef struct _KPsolution KPsolution;
struct _KPsolution
{
  long f;
  vboolean x;
};


/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_dati);

/* Carica da file_dati l'istanza I del problema dello zaino */
void CaricaDati (char *file_dati, KPdata *pI);

/* Distrugge l'istanza I */
void DistruggeDati (KPdata *pI);

/* Crea una soluzione vuota */
void CreaSoluzione (int n, KPsolution *pS);

/* Distrugge la soluzione S */
void DistruggeSoluzione(KPsolution *pS);

/* Stampa la soluzione */
void StampaSoluzione (int n, KPsolution *pS);

/* Algoritmo ricorsivo per il problema dello zaino */
long AlgoritmoRicorsivoKP (long n, vlong phi, vlong v, long V);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char file_dati[ROW_LENGTH];
  KPdata I;
  KPsolution S;
  clock_t inizio, fine;
  double tempo;


  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_dati);

  /* Carica i dati */
  CaricaDati(file_dati,&I);

  /* Crea una soluzione vuota */
  CreaSoluzione(I.n,&S);

  /* Risolve il problema dello zaino */
  inizio = clock();
  S.f = AlgoritmoRicorsivoKP(I.n,I.phi,I.v,I.V);
  fine = clock();
  tempo = ((double) (fine - inizio))/CLOCKS_PER_SEC;

  /* Stampa il tempo di calcolo e la soluzione */
  printf("CPU = %lf\n",tempo);
  StampaSoluzione(I.n,&S);

  /* Dealloca le strutture dati dinamiche */
  DistruggeDati(&I);
  DistruggeSoluzione(&S);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_dati)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    fprintf(stderr,"%s [file_dati]\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  strcpy(file_dati,argv[1]);
}


/* Carica i dati contenuti nel file */
void CaricaDati (char *file_dati, KPdata *pI)
{
  FILE *fp;
  int i;


  fp = fopen(file_dati,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"Errore nell'apertura del file %s!\n",file_dati);
    exit(EXIT_FAILURE);
  }

  if (fscanf(fp,"%*s %*s %ld",&pI->n) != 1)
  {
    fprintf(stderr,"Errore nella lettura del numero di elementi!\n");
    exit(EXIT_FAILURE);
  }

  pI->phi = (long *) calloc(pI->n+1,sizeof(long));
  if (pI->phi == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore dei premi!\n");
    exit(EXIT_FAILURE);
  }

  pI->v = (long *) calloc(pI->n+1,sizeof(long));
  if (pI->v == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione del vettore dei volumi!\n");
    exit(EXIT_FAILURE);
  }

  /* Salta "phi =" */
  fscanf(fp,"%*s %*s");
  for (i = 1; i <= pI->n; i++)
    if (fscanf(fp,"%ld",&pI->phi[i]) != 1)
    {
      fprintf(stderr,"Errore nella lettura del vettore dei premi!\n");
      exit(EXIT_FAILURE);
    }

  /* Salta "v =" */
  fscanf(fp,"%*s %*s");
  for (i = 1; i <= pI->n; i++)
    if (fscanf(fp,"%ld",&pI->v[i]) != 1)
    {
      fprintf(stderr,"Errore nella lettura del vettore dei volumi!\n");
      exit(EXIT_FAILURE);
    }

  if (fscanf(fp,"%*s %*s %ld",&pI->V) != 1)
  {
    fprintf(stderr,"Errore nella lettura della capacita'!\n");
    exit(EXIT_FAILURE);
  }

  fclose(fp);
}


/* Dealloca le strutture dati dinamiche */
void DistruggeDati (KPdata *pI)
{
  free(pI->phi);
  free(pI->v);
  pI->n = 0;
  pI->phi = NULL;
  pI->v = NULL;
  pI->V = 0;
}


/* Crea una soluzione vuota */
void CreaSoluzione (int n, KPsolution *pS)
{
  pS->f = 0L;
  pS->x = (vboolean) calloc(n+1,sizeof(boolean));
  if (pS->x == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione della soluzione!\n");
    exit(EXIT_FAILURE);
  }
}


/* Distrugge la soluzione S */
void DistruggeSoluzione(KPsolution *pS)
{
  pS->f = 0L;
  free(pS->x);
  pS->x = NULL;
}


/* Stampa la soluzione */
void StampaSoluzione (int n, KPsolution *pS)
{
  int i;

  printf("f = %ld\n",pS->f);
  printf("x = (");
  for (i = 1; i <= n; i++)
    if (pS->x[i] == TRUE)
      printf(" %d",i);
  printf(" )\n");
}


/* Algoritmo ricorsivo per il problema dello zaino */
long AlgoritmoRicorsivoKP (long n, vlong phi, vlong v, long V)
{
  long phi0, phi1;

  if (n == 0)
    return 0L;
  else if (v[n] > V)
    return AlgoritmoRicorsivoKP(n-1,phi,v,V);
  else /* (v[n] <= V) */
  {
    phi0 = AlgoritmoRicorsivoKP(n-1,phi,v,V);
    phi1 = phi[n] + AlgoritmoRicorsivoKP(n-1,phi,v,V-v[n]);
    return ( (phi0 > phi1) ? phi0 : phi1 );
  }
}
