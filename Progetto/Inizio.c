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
int main (int argc, char *argv[]){
  /* Parte dichiarativa */
  char chiamate[ROW_LENGTH],rete[ROW_LENGTH],veicoli[ROW_LENGTH];
  FILE *fcall, *fnet, *fcar;
  ptcall *ListaChiamate;
  int NumChiamate;
  ptevent ListaEventi;
  grafo *ReteStradale;
  ParcoAuto *ListaTaxi;


  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,chiamate,rete,veicoli);

  /*apertura dei file di dati ed handling dell'eventuale errore. Ricordiamo che exit chiude ga solo i file aperti prima di terminare l'esecuzione*/
  fcall= fopen(chiamate,"r");
    if(fcall == NULL){
       printf("Errore apertura file chiamate: %s\n", strerror(errno));
       exit(EXIT_FAILURE);
     }

  fnet= fopen(rete,"r");
    if(fnet == NULL){
      printf("Errore apertura file rete stradale: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

  fcar= fopen(veicoli,"r");
    if(fcar == NULL) {
      printf("Errore apertura file veicoli: %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }

/*Inizializza e produce l'elenco delle chiamate*/
  ListaChiamate=getcalls(fcall,&NumChiamate);
  fclose(fcall);
/*acquisisce il grafo in liste di adiacenza*/
  ReteStradale=getgraph(fnet);
  fclose(fnet);
/*Inizia a creare gli eventi generati dalle chiamate e li salva in una lista linkata. questa è pensata con rimozioni solo in testa e inserimenti ovunque*/
  ListaEventi=ImportaEventoChiamate (ListaChiamate,NumChiamate);
/*ora che l'ordine delle chiamate in ListaChiamate utile alla simulazione è stato salvato importando in tale ordine gli eventi, si ordinano
in ordine alfabetico per il nome dei clienti, lo si stampa, e si libera la memoria: non ci serviranno più le chiamate in questa forma.*/
  QuickSort(ListaChiamate,0,NumChiamate-1);
  printclients(ListaChiamate,NumChiamate);

/*ora devo ordinare le chiamate per durata del tragitto associato e stamparle. */
  PrintChiamViaggio(ListaChiamate,ReteStradale,NumChiamate);

/*Devo creare e distribuire le auto, poi stampare la loro posizione*/
  ListaTaxi=CreaAutomobili(fcar);
  fclose(fcar);
  PlaceCar(ReteStradale,ListaTaxi);
  StampaPosAuto(ListaTaxi);

/*a questo punto viene eseguita la simulazione vera e propria, elaborando dalla testa la lista eventi.*/
  ElaboraListaEventi(ListaTaxi, ReteStradale, ListaEventi);

  printf("UB: %d\n",StimaGuadagnoGreedy(ListaChiamate,NumChiamate,ListaTaxi));
  /*printf("UB banale: %d\n",StimaBanale(ListaChiamate,NumChiamate) );    questa funzione è stata rimossa in quanto il suo output non è esplicitamente richiesto dalle specifiche ed è scomparso dalle soluzioni dopo l'11/06/19*/

  Rottama(ListaTaxi);/*Dealloca il parco macchine e le macchine*/
  freecalls(ListaChiamate,NumChiamate);/*dealloca anche i viaggi associati*/

  return EXIT_SUCCESS;
}



/* Definizione delle procedure secondarie --------------------------------------*/

void InterpretaLineaComando (int argc, char *argv[], char *chiamate, char *rete, char *veicoli){
  if (argc != 4)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(chiamate,argv[1]);
  strcpy(rete,argv[2]);
  strcpy(veicoli,argv[3]);
}
