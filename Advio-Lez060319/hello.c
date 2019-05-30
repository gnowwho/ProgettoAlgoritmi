/* hello.c */

/* Direttive */
#include <stdlib.h>
#include "advio.h"

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie */

void AcquisisceSaluto (char *saluto);

char AcquisisceCornice ();

void StampaSaluto (char *saluto, char cornice);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char saluto[ROW_LENGTH];
  char cornice;


  /* Parte esecutiva */

  /* Acquisisce il carattere che si vuole usare per la cornice */
  cornice = AcquisisceCornice();

  /* Acquisisce il saluto che si vuole stampare */
  AcquisisceSaluto(saluto);

  /* Stampa il saluto incorniciato */
  StampaSaluto(saluto,cornice);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */
void AcquisisceSaluto (char *saluto)
{
  StampaStringa("Inserire il saluto da stampare: ");
  LeggeRiga(saluto);
}


char AcquisisceCornice ()
{
  char cornice;

  StampaStringa("Inserire il carattere da usare per la cornice: ");
  cornice = LeggeCarattere();
  return cornice;
}


void StampaSaluto (char *saluto, char cornice)
{
  int larghezza;


  /* Determina la larghezza della cornice */
  larghezza = LunghezzaStringa(saluto) + 4;

  /* Stampa la cornice superiore */
  StampaCarattere(cornice,larghezza);

  /* Va a capo */
  ACapo();

  /* Stampa la cornice sinistra */
  StampaCarattere(cornice,1);

  /* Stampa uno spazio */
  StampaCarattere(' ',1);

  /* Stampa il saluto */
  StampaStringa(saluto);

  /* Stampa uno spazio */
  StampaCarattere(' ',1);

  /* Stampa la cornice destra */
  StampaCarattere(cornice,1);

  /* Va a capo */
  ACapo();

  /* Stampa la cornice inferiore */
  StampaCarattere(cornice,larghezza);
}
















