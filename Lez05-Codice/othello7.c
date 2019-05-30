/* othello.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scacchiera.h"

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie */

typedef char Scacchiera[BOARD_SIZE][BOARD_SIZE];

void InterpretaLineaComando (int argc, char *argv[], char *filedati);

/* Esegue le mosse indicate nel file filemosse */
void Othello (Scacchiera S, char *filemosse);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char filemosse[ROW_LENGTH];
  Scacchiera S;

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,filemosse);

  /* Inizializza la scacchiera */
  InizializzaScacchiera(S);

  /* Esegue le mosse indicate nel file filemosse */
  Othello(S,filemosse);

  /* Stampa la scacchiera finale */
  StampaScacchiera(S);

  /* Stampa l'esito del gioco */
  StampaEsito(S);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *filemosse)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(filemosse,argv[1]);
}


/* Esegue le mosse indicate nel file filemosse */
void Othello (Scacchiera S, char *filemosse)
{
  FILE *fp_filemosse;
  char mossa[MOVE_LENGTH+1];
  char giocatore;
  int r, c;


  fp_filemosse = fopen(filemosse,"r");
  if (fp_filemosse == NULL)
  {
    fprintf(stderr,"File %s non trovato!\n",filemosse);
    exit(EXIT_FAILURE);
  }

  giocatore = NERO;
  while (fscanf(fp_filemosse,"%s",mossa) == 1)
  {
    if (strcmp(mossa,"--") == 0)
      VerificaSospensione(S,giocatore);
    else
      EsegueMossa(mossa,giocatore,S);

    if (giocatore == BIANCO)
      giocatore = NERO;
    else
      giocatore = BIANCO;
  }

  fclose(fp_filemosse);
}
