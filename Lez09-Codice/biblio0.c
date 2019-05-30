/* biblio.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_libri, char *file_prestiti);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char file_libri[ROW_LENGTH];
  char file_prestiti[ROW_LENGTH];

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_libri,file_prestiti);

  /* Carica l'elenco dei libri disponibili */

  /* Crea le liste (vuote) dei libri prestati e resi */

  /* Esegue i movimenti riportati nel file dei prestiti e dei resi */

  /* Stampa l'elenco dei libri disponibili */

  /* Stampa le istruzioni per riporre i libri resi */

  /* Stampa l'elenco dei libri disponibili */

  /* Dealloca le strutture dinamiche */

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
