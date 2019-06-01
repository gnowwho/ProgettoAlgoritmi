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
  ptevent ListaEventi;
  grafo *ReteStradale;

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

/*Inizializza e produce l'elenco delle chiamate*/
  ListaChiamate=NULL; /*Inizializzato perchè -Wall sorride di più così*/
  ListaChiamate=getcalls(fcall,&NumChiamate);
  ReteStradale=getgraph(fnet);
/*Inizia a creare gli eventi generati dalle chiamate e li salva nella struttura dati adeguata*/
  ListaEventi=ImportaEventoChiamate (ListaChiamate,NumChiamate);
/*ora che l'ordine delle chiamate in ListaChiamate utile alla simulazione è stato salvato importando in tale ordine gli eventi, si ordinano
in ordine alfabetico per il nome dei clienti, lo si stampa, e si libera la memoria: non ci serviranno più le chiamate in questa forma.*/
  QuickSort(ListaChiamate,0,NumChiamate-1);
//  printclients(ListaChiamate,NumChiamate);
  freecalls (ListaChiamate,NumChiamate);

  printevent(ListaEventi);

  printgraph(ReteStradale);



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
