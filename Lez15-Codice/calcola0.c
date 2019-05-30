/* calcola.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_espressioni);

/* Elabora le espressioni contenute nel file file_espressioni */
void ElaboraEspressioni (char *file_espressioni);


/* Programma principale -----------------------------------------------------*/
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char file_espressioni[ROW_LENGTH];

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_espressioni);

  /* Elabora le espressioni nel file */
  ElaboraEspressioni(file_espressioni);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie -----------------------------------*/

void InterpretaLineaComando (int argc, char *argv[], char *file_espressioni)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    fprintf(stderr,"%s [file_espressioni]\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  strcpy(file_espressioni,argv[1]);
}


void ElaboraEspressioni (char *file_espressioni)
{
  FILE *fp;
  char Espressione[ROW_LENGTH];


  fp = fopen(file_espressioni,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"Errore nell'apertura del file %s!\n",file_espressioni);
    exit(EXIT_FAILURE);
  }

  /* Legge il file una riga alla volta */
  while (fscanf(fp,"%[^\n]\n",Espressione) == 1)    /*legge la riga e toglie l'a-capo*/
  {
    /* Carica l'espressione corrente dalla stringa in un albero */

    /* Calcola il valore dell'espressione rappresentata dall'albero */

    /* Stampa in notazione infissa l'espressione rappresentata dall'albero */

    /* Calcola la profondita' dell'albero */

    /* Distrugge l'albero */

    printf("\n");
  }

  fclose(fp);
}
