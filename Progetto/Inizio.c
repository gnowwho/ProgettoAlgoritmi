/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "automotive.h"

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie ----------------------------------------*/

void InterpretaLineaComando (int argc, char *argv[], char *chiamate, char *rete, char *veicoli);




/* Programma principale --------------------------------------------------------*/
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char chiamate[ROW_LENGTH],rete[ROW_LENGTH],veicoli[ROW_LENGTH];
  FILE *fcall, *fnet, *fcar;
  /*grafo *graph;*/
  ptcall *ListaChiamate;
  int NumChiamate;

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,chiamate,rete,veicoli);

  /*apertura dei file di dati ed handling dell'eventuale errore*/
  fcall= fopen(chiamate,"r");
    if(fcall == NULL){
       printf("Errore apertura file chiamate: %s\n", strerror(errno));
       exit(EXIT_FAILURE);
     }

  fnet= fopen(rete,"r");
    if(fnet == NULL){
      printf("Errore apertura file rete stradale: %s\n", strerror(errno));
      fclose(fcall);
      exit(EXIT_FAILURE);
    }

  fcar= fopen(veicoli,"r");
    if(fcar == NULL) {
      printf("Errore apertura file veicoli: %s\n", strerror(errno));
      fclose(fcall);
      fclose(fnet);
      exit(EXIT_FAILURE);
    }

  ListaChiamate=NULL; /*Inizializzato perchè -Wall sorride di più così*/
  ListaChiamate=getcalls(fcall,&NumChiamate);

  QuickSort(ListaChiamate,0,NumChiamate-1);

  printclients(ListaChiamate,NumChiamate);

  fclose(fcall);
  fclose(fnet);
  fclose(fcar);


  return EXIT_SUCCESS;
}




/* Definizione delle procedure secondarie --------------------------------------*/

void InterpretaLineaComando (int argc, char *argv[], char *chiamate, char *rete, char *veicoli)
{
  if (argc != 4)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(chiamate,argv[1]);
  strcpy(rete,argv[2]);
  strcpy(veicoli,argv[3]);
}
