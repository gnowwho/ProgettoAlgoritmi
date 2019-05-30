/* biblio.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listalibri.h"


/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_libri, char *file_prestiti);

listalibri CaricaLibri (char *file_libri);

/* Esegue i movimenti riportati nel file dei prestiti e dei resi */
void EsegueMovimenti (char *file_prestiti, listalibri *Scaffale,
                      listalibri *Prestiti, listalibri *Resi);

/* Stampa l'elenco dei libri disponibili */
void StampaLibri (listalibri Scaffale);

/* Stampa le istruzioni per riporre i libri resi */
void StampaIstruzioniRiordino (listalibri *Resi, listalibri *Scaffale);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char file_libri[ROW_LENGTH];
  char file_prestiti[ROW_LENGTH];
  listalibri Scaffale;
  listalibri Prestiti;
  listalibri Resi;

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_libri,file_prestiti);

  /* Carica l'elenco dei libri disponibili */
  Scaffale = CaricaLibri(file_libri);

  /* Crea le liste (vuote) dei libri prestati e resi */
  Prestiti = crealistalibri();
  Resi = crealistalibri();

  /* Esegue i movimenti riportati nel file dei prestiti e dei resi */
  EsegueMovimenti(file_prestiti,&Scaffale,&Prestiti,&Resi);

  /* Stampa l'elenco dei libri disponibili */
  StampaLibri(Scaffale);

  /* Stampa le istruzioni per riporre i libri resi */
  StampaIstruzioniRiordino(&Resi,&Scaffale);

  /* Stampa l'elenco dei libri disponibili */
  StampaLibri(Scaffale);

  /* Dealloca le strutture dinamiche */
  distruggelista(&Scaffale);
  distruggelista(&Prestiti);
  distruggelista(&Resi);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_libri, char *file_prestiti)
{
  if (argc != 3)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(file_libri,argv[1]);
  strcpy(file_prestiti,argv[2]);
}


  /* Carica l'elenco dei libri disponibili */
listalibri CaricaLibri (char *file_libri)
{
  return crealistalibri();
}

/* Esegue i movimenti riportati nel file dei prestiti e dei resi */
void EsegueMovimenti (char *file_prestiti, listalibri *pScaffale,
                      listalibri *pPrestiti, listalibri *pResi)
{
}


/* Stampa l'elenco dei libri disponibili */
void StampaLibri (listalibri L)
{
}


/* Stampa le istruzioni per riporre i libri resi */
void StampaIstruzioniRiordino (listalibri *pResi, listalibri *pScaffale)
{
}
