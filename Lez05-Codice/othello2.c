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

  /* Esegue le mosse indicate nel file filemosse */
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
  int r, c;

  for (r = 0; r < BOARD_SIZE; r++)
    for (c = 0; c < BOARD_SIZE; c++)
      S[r][c] = LIBERO;

  S[BOARD_SIZE/2-1][BOARD_SIZE/2-1] = S[BOARD_SIZE/2][BOARD_SIZE/2] = BIANCO;
  S[BOARD_SIZE/2-1][BOARD_SIZE/2] = S[BOARD_SIZE/2][BOARD_SIZE/2-1] = NERO;
}


/* Esegue le mosse indicate nel file filemosse */
void Othello (Scacchiera S, char *filemosse)
{
}


/* Stampa la scacchiera S */
void StampaScacchiera (Scacchiera S)
{
  int r, c;

  /* Stampa la cornice superiore */
  printf(" ");
  for (c = 0; c < BOARD_SIZE; c++)
    printf("%c",c+'a');
  printf("\n");

  /* Stampa la cornice laterale e la scacchiera */
  for (r = 0; r < BOARD_SIZE; r++)
  {
    printf("%d",r+1);
    for (c = 0; c < BOARD_SIZE; c++)
      printf("%c",S[r][c]);
    printf("\n");
  }
  printf("\n");
}


/* Stampa l'esito del gioco sulla scacchiera S */
void StampaEsito (Scacchiera S)
{
  int r, c;
  int nb, nn;


  /* Conta le pedine bianche e nere */
  nb = nn = 0;
  for (r = 0; r < BOARD_SIZE; r++)
    for (c = 0; c < BOARD_SIZE; c++)
      if (S[r][c] == BIANCO)
        nb++;
      else if (S[r][c] == NERO)
        nn++;

  /* Stampa il numero di pedine bianche e nere */
  printf("%d pedine bianche\n",nb);
  printf("%d pedine nere\n",nn);

  /* Stampa il risultato */
  if (nb > nn)
    printf("Vince il Bianco\n");
  else if (nn > nb)
    printf("Vince il Nero\n");
  else
    printf("Pareggio\n");
}
