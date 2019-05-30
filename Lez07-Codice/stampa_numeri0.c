/* stampa_numeri.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie e definizione di tipi */

typedef int* vint;

void InterpretaLineaComando (int argc, char *argv[], char *filemosse);


/* Programma principale */
int main (int argc, char *argv[])
{
  char filedati[ROW_LENGTH];
  vint V;
  int n;


  /* Legge da linea di comando il file che contiene i dati */
  InterpretaLineaComando(argc,argv,filedati);

  /* Legge la lunghezze e gli elementi del vettore */

  /* Stampa il vettore ordinato */

  /* Dealloca il vettore (anche per controllare eventuali errori di accesso) */
  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

/* Legge dalla linea di comando il file che contiene i dati */
void InterpretaLineaComando (int argc, char *argv[], char *filedati)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(filedati,argv[1]);
}
