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

/* Esegue, se possibile, la mossa per il giocatore sulla scacchiera */
void EsegueMossa (char *mossa, char giocatore, Scacchiera S);

/* Valuta la mossa (r,c) in direzione (dr,dc) per il giocatore sulla scacchiera */
boolean ValutaDirezione (int r, int c, int dr, int dc, char giocatore, Scacchiera S);

/* Esegue la mossa (r,c) in direzione (dr,dc) per il giocatore sulla scacchiera */
void EsegueDirezione (int r, int c, int dr, int dc, char giocatore, Scacchiera S);

/* Verifica se la posizione corrente sulla scacchiera S e' davvero di sospensione per il giocatore */
void VerificaSospensione (Scacchiera S, char giocatore);

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

void InterpretaLineaComando (int argc, char *argv[], char *filemosse)
{
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

  // Si sposta nella casella adiacente finche' e' nella scacchiera e occupata dall'avversario
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
}


/* Esegue le mosse indicate nel file filemosse */
void Othello (Scacchiera S, char *filemosse)
{
  FILE *fp_filemosse;
  char mossa[MOVE_LENGTH+1];
  char giocatore;


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
