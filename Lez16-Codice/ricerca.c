/* ricerca.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "abr.h"

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_dati);

/* Carica i dati contenuti nel file in un ABR */
ABR CaricaABR (char *file_dati);

/* Esegue i comandi forniti da tastiera */
ABR EsegueComandi (ABR T);


/* Programma principale */
int main (int argc, char *argv[])
{
  ABR T;

  /* Parte dichiarativa */
  char file_dati[ROW_LENGTH];

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_dati);

  /* Carica i dati contenuti nel file in un ABR */
  T = CaricaABR(file_dati);

  /* Esegue i comandi forniti da tastiera */
  T = EsegueComandi(T);

  distruggeABR(&T);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_dati)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    fprintf(stderr,"%s [file_dati]\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  strcpy(file_dati,argv[1]);
}

/* Carica i dati contenuti nel file in un ABR */
ABR CaricaABR (char *file_dati)
{
  FILE *fp;
  ABR T;
  int i;


  fp = fopen(file_dati,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"Errore nell'apertura del file %s!\n",file_dati);
    exit(EXIT_FAILURE);
  }

  T = creaABR();
  while (fscanf(fp,"%d",&i) == 1)
    T = insertABR(i,T);

  fclose(fp);

  return T;
}


/* Esegue i comandi forniti da tastiera */
ABR EsegueComandi (ABR T)
{
  boolean fine;
  char Comando[ROW_LENGTH];
  char op[ROW_LENGTH];
  int val;
  int cont;


  fine = FALSE;
  do
  {
    /* Stampa un prompt per segnalare che si attende un comando */
    printf("> ");

    /* Legge il comando dell'utente */
    fgets(Comando,ROW_LENGTH,stdin);

    /* Identifica il comando dell'utente e lo esegue */
    cont = sscanf(Comando,"%s %d",op,&val);
    if (cont == 2)
    {
      if (strcmp(op,"member") == 0)
      {
        if (memberABR(val,T))
          printf("%d in T\n",val);
        else
          printf("%d non in T\n",val);
      }
      else if (strcmp(op,"insert") == 0)
        T = insertABR(val,T);
      else if (strcmp(op,"delete") == 0)
        T = deleteABR(val,T);
      else
      {
        fprintf(stderr,"Comando non riconosciuto!\n");
        exit(EXIT_FAILURE);
      }
    }
    else if (cont == 1)
    {
      if (strcmp(op,"print") == 0)
      {
        stampaABR(T);
        printf("\n");
      }
      else if (strcmp(op,"empty") == 0)
      {
        if (ABRvuoto(T))
          printf("T e' vuoto\n");
        else
          printf("T non e' vuoto\n");
      }
      else if (strcmp(op,"min") == 0)
        printf("min(T) = %d\n",minABR(T));
      else if (strcmp(op,"max") == 0)
        printf("max(T) = %d\n",maxABR(T));
      else if (strcmp(op,"exit") == 0)
        fine = TRUE;
      else
      {
        fprintf(stderr,"Comando non riconosciuto!\n");
        exit(EXIT_FAILURE);
      }
    }
    else
    {
      fprintf(stderr,"Comando non riconosciuto!\n");
      exit(EXIT_FAILURE);
    }
  } while (!fine);

  return T;
}
