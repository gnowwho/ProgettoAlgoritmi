#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "scacchiera.h"


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


/* Esegue, se possibile, la mossa per il giocatore sulla scacchiera */
void EsegueMossa (char *mossa, char giocatore, Scacchiera S)
{
  boolean mossa_lecita;
  int r, c, dr, dc;


  r = mossa[1] - '1';
  c = mossa[0] - 'a';

  mossa_lecita = FALSE;
  if (S[r][c] == LIBERO)
    for (dr = -1; dr <= 1; dr++)
      for (dc = -1; dc <= 1; dc++)
        if ( (dr != 0) || (dc != 0) )
        {
          if (ValutaDirezione(r,c,dr,dc,giocatore,S) == TRUE)
          {
            mossa_lecita = TRUE;
            EsegueDirezione(r,c,dr,dc,giocatore,S);
          }
        }

  if (mossa_lecita == TRUE)
    S[r][c] = giocatore;
  else
  {
    fprintf(stderr,"La mossa %s non e' lecita per il giocatore %c!\n",mossa,giocatore);
    exit(EXIT_FAILURE);
  }
}


/* Valuta la mossa (r,c) in direzione (dr,dc) per il giocatore sulla scacchiera */
boolean ValutaDirezione (int r, int c, int dr, int dc, char giocatore, Scacchiera S)
{
  int rf, cf;
  char avversario;

  if (giocatore == BIANCO)
    avversario = NERO;
  else
    avversario = BIANCO;

  // Trova la casella iniziale in direzione (dr,dc)
  rf = r+dr;
  cf = c+dc;

  /* Si sposta nella casella adiacente finche' e' nella scacchiera e occupata dall'avversario */
  while ( (0 <= rf) && (rf < BOARD_SIZE) && (0 <= cf) && (cf < BOARD_SIZE) && (S[rf][cf] == avversario) )
  {
    rf += dr;
    cf += dc;
  }

  // La mossa e' lecita se la casella seguente e' nella scacchiera, occupata dal giocatore e non adiacente a quella iniziale
  return ( (0 <= rf) && (rf < BOARD_SIZE) && (0 <= cf) && (cf < BOARD_SIZE) &&
           (S[rf][cf] == giocatore) &&
           ( (rf != r+dr) || (cf != c+dc) ) );
}


/* Esegue la mossa (r,c) in direzione (dr,dc) per il giocatore sulla scacchiera */
void EsegueDirezione (int r, int c, int dr, int dc, char giocatore, Scacchiera S)
{
  int rf, cf;
  char avversario;

  if (giocatore == BIANCO)
    avversario = NERO;
  else
    avversario = BIANCO;

  /* Trova la casella iniziale in direzione (dr,dc) */
  rf = r+dr;
  cf = c+dc;

  /* Si sposta nella casella adiacente marcando le celle */
  while ( (0 <= rf) && (rf < BOARD_SIZE) && (0 <= cf) && (cf < BOARD_SIZE) &&
           (S[rf][cf] == avversario) )
  {
    S[rf][cf] = giocatore;
    rf += dr;
    cf += dc;
  }
}


/* Verifica se la posizione corrente sulla scacchiera S e' davvero di sospensione per il giocatore */
void VerificaSospensione (Scacchiera S, char giocatore)
{
  int r, c, dr, dc;

  for (r = 0; r < BOARD_SIZE; r++)
    for (c = 0; c < BOARD_SIZE; c++)
      if (S[r][c] == LIBERO)
        for (dr = -1; dr <= 1; dr++)
          for (dc = -1; dc <= 1; dc++)
            if ( (dr != 0) || (dc != 0) )
              if (ValutaDirezione(r,c,dr,dc,giocatore,S) == TRUE)
              {
                fprintf(stderr,"Il giocatore %s non e' bloccato!\n",giocatore);
                exit(EXIT_FAILURE);
              }
}


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
