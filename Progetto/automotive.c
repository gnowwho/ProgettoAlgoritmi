#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include"automotive.h"

/*legge il numero di chiamate, alloca dinamicamente il vettore dei pointer alle chiamate, e poi memoria per una chiamata per ognuno di quei puntatori. questo approccio migliora il sorting*/
ptcall *getcalls (FILE *fp, int *hmcalls){
  int i;
  ptcall *vettcall;

  fscanf(fp, "%d", hmcalls);

  vettcall = malloc(*hmcalls*sizeof(ptcall));

  if(vettcall == NULL){
    printf("Errore allocazione lista: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  for(i=0;i<*hmcalls;i++){
    vettcall[i] = malloc(sizeof(call));
    if(vettcall[i] == NULL){
      printf("Errore allocazione chiamata %d: %s\n",i,strerror(errno));
      exit(EXIT_FAILURE);
    }
    fscanf(fp,"%d %s %d %d %d %d %d\n", &vettcall[i]->Ora,
                                        vettcall[i]->Nome,
                                        &vettcall[i]->Partenza,
                                        &vettcall[i]->Arrivo,
                                        &vettcall[i]->OraPartenza,
                                        &vettcall[i]->OraArrivo,
                                        &vettcall[i]->Premio);
    vettcall[i]->indice=i+1;
  }
 return vettcall;

}

/*Stampa un elenco di k chiamate nell'ordine che i loro puntatori occupano in memoria. per iniziare da una posizione diversa dalla prima basta passare il puntatore
spostato di alcune posizioni: per aritmetica dei puntatori basta sommarvi un intero*/
void printcalls (ptcall *vettcall, int k){
  int i;

  for(i=0;i<k;i++){
    printf("%d %s %d %d %d %d %d %d\n", vettcall[i]->Ora,
                                        vettcall[i]->Nome,
                                        vettcall[i]->Partenza,
                                        vettcall[i]->Arrivo,
                                        vettcall[i]->OraPartenza,
                                        vettcall[i]->OraArrivo,
                                        vettcall[i]->Premio,
                                        vettcall[i]->indice);
  }

}

/*Stampa l'elenco clienti di k chiamate nell'ordine che i loro puntantori occupano in memoria. Vedi sopra per altro.*/
void printclients (ptcall *vettcall, int k){
  int i;

  printf("Clienti:\n");

  for(i=0;i<k;i++){
    printf("%s\n",vettcall[i]->Nome);
  }

}

/* Ordina gli elementi del vettore V con indici compresi fra s e d */
void QuickSort (ptcall *V, int s, int d){
  int q;

  q=0;
  if (s < d)
  {
    /* Nota: scelta brutale di V[s] come elemento pivot */
    q = Partition(V,s,d);
    QuickSort(V,s,q-1);
    QuickSort(V,q+1,d);
  }

}

/* Raccoglie nella prima parte del vettore V[s,d] gli elementi <= V[s],
  nella seconda quelli > V[s], e restituisce l'indice dell'elemento separatore */
int Partition (ptcall *V, int s, int d){
  int d1, s2; /* Prima posizione esterna ai sottovettori 1 e 2 */

  d1 = s+1; /* V[s] e' certamente <= elemento pivot, perche' e' l'elemento pivot */
  s2 = d;

  while (d1 <= s2 )
  {
    /* Si lasciano nel sottovettore 2 gli elementi > pivot
       Nota: non occorre il test (s2 > s) perche' per s2 = s, (V[s2] > V[s]) e' falso */
    while (strcmp(V[s2]->Nome,V[s]->Nome) > 0) s2--;

    /* Lasciare in cima al vettore gli elementi <= pivot
       Nota: qui occorre il test (d1 <= s2) perche' con s2 = d si rischia di sforare */
    while ( (strcmp(V[d1]->Nome,V[s]->Nome)<=0) && (d1 <= s2) ) d1++;

    if (d1 < s2)
    {
      ScambiaChiamate(&V[d1],&V[s2]);
      d1++;
      s2--;
    }
  }

  /* Il pivot si sposta dalla posizione s alla posizione intermedia, d1-1 ovvero s2 */
  ScambiaChiamate(&V[s],&V[s2]);
  return s2;
}

void ScambiaChiamate (ptcall *pa, ptcall *pb){
  ptcall temp;

  temp = *pa;
  *pa = *pb;
  *pb = temp;
}

/*restituisce il puntatore al primo elemento della lista dell'elemento puntato da pne seguito da tutti quelli puntati da poe
assegnando il suo valore alla variabile da cui si è copiato pne si effettua l'inserimento di un elemento tra quello ed il successivo nella lista*/
ptevent AddEvent (ptevent pne, ptevent poe){
  pne->next=poe;
  return pne;
}

ptevento CallToEvent (ptcall tel){
  ptevento evchiamata;

  evchiamata=malloc(sizeof(evento));
  evchiamata->Ora=tel->Ora;

}

/*Importa la lista delle chiamate, opportunamente in eventi, nell'ordine che i loro puntatori occupano in memoria*/
/*ptevent ImportaEventoChiamate (ptcall *chiamate, int num){
int i;
ptevent evlist, scorri;

evlist=scorri;

for (i=0;i<num;i++){
    scorri=malloc(sizeof(evento));
    scorri->Ora=chiamate[i]->Ora;
    scorri=scorri->next
}
scorri->next=NULL;
return evlist;
}
*/
