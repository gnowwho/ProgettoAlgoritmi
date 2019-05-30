/* othello.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROW_LENGTH 256

typedef int boolean;
#define TRUE  1
#define FALSE 0

#define BOARD_SIZE   8
#define BIANCO      'B'
#define NERO        'N'
#define LIBERO      '.'
#define MOVE_LENGTH  2


/* Prototipi delle procedure secondarie */

typedef char Scacchiera[BOARD_SIZE][BOARD_SIZE];

void InterpretaLineaComando (int argc, char *argv[], char *filedati);

/* Costruisce la configurazione iniziale della scacchiera S */
void InizializzaScacchiera (Scacchiera S);

/* Esegue le mosse indicate nel file filemosse */
void Othello (Scacchiera S, char *filemosse);

/* Stampa la scacchiera S */
void StampaScacchiera (Scacchiera S);

/* Stampa l'esito del gioco sulla scacchiera S */
void StampaEsito (Scacchiera S);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char filemosse[ROW_LENGTH];
  Scacchiera S;

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,filemosse);

  /* Costruisce la configurazione iniziale della scacchiera */
  InizializzaScacchiera(S);

  /* Esegue le mosse indicate nel file */
  Othello(S,filemosse);

  /* Stampa la scacchiera finale */
  StampaScacchiera(S);

  /* Stampa l'esito del gioco */
  StampaEsito(S);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *filemosse){
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(filemosse,argv[1]);
}


/* Costruisce la configurazione iniziale della scacchiera S */
void InizializzaScacchiera (Scacchiera S)
{
}


/* Esegue le mosse indicate nel file filemosse */
void Othello (Scacchiera S, char *filemosse)
{
}


/* Stampa la scacchiera S */
void StampaScacchiera (Scacchiera S)
{
}


/* Stampa l'esito del gioco sulla scacchiera S */
void StampaEsito (Scacchiera S)
{
}
