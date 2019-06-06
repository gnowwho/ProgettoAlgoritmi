#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include"automotive.h"

/*legge il numero di chiamate, alloca dinamicamente il vettore dei pointer alle chiamate, e poi memoria per una chiamata per ognuno di quei puntatori.
questo approccio migliora il sorting*/
ptcall *getcalls (FILE *fp, int *hmcalls){
  int i;
  ptcall *vettcall;

  /*Leggo il numero di chiamate e alloco altrettanti puntatori a chiamata*/
  fscanf(fp, "%d", hmcalls);

  vettcall = malloc(*hmcalls*sizeof(ptcall));
  if(vettcall == NULL){
    printf("Errore allocazione lista: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  /*per ogni puntatore a chiamata alloco una chiamata e la leggo da file*/
  for(i=0;i<*hmcalls;i++){
    vettcall[i] = malloc(sizeof(call));
    if(vettcall[i] == NULL){
      printf("Errore allocazione chiamata %d: %s\n",i,strerror(errno));
      exit(EXIT_FAILURE);
    }

    vettcall[i]->Richiesta = malloc(sizeof(viaggio));
    if(vettcall[i] == NULL){
      printf("Errore allocazione Viaggio associato alla chiamata %d: %s\n",i,strerror(errno));
      exit(EXIT_FAILURE);
    }

    fscanf(fp,"%d %s %d %d %d %d %d\n", &vettcall[i]->Ora,
                                        vettcall[i]->Nome,
                                        &vettcall[i]->Richiesta->Partenza,
                                        &vettcall[i]->Richiesta->Arrivo,
                                        &vettcall[i]->Richiesta->OraPartenza,
                                        &vettcall[i]->Richiesta->OraArrivo,
                                        &vettcall[i]->Richiesta->Premio);
  }
 return vettcall;

}

/*Stampa un elenco di k chiamate nell'ordine che i loro puntatori occupano in memoria. per iniziare da una posizione diversa dalla prima basta passare il puntatore
spostato di alcune posizioni: per aritmetica dei puntatori basta sommarvi un intero*/
void printcalls (ptcall *vettcall, int k){
  int i;

  for(i=0;i<k;i++){
    printf("%d %s %d %d %d %d %d\n",  vettcall[i]->Ora,
                                      vettcall[i]->Nome,
                                      vettcall[i]->Richiesta->Partenza,
                                      vettcall[i]->Richiesta->Arrivo,
                                      vettcall[i]->Richiesta->OraPartenza,
                                      vettcall[i]->Richiesta->OraArrivo,
                                      vettcall[i]->Richiesta->Premio);
  }

}

void freecalls (ptcall *vettcall, int k){
  int i;

  /*prima dealloco la memoria occupata dai viaggi, poi dalle chiamate*/
  for(i=0; i<k; i++){
    free(vettcall[i]->Richiesta->ElencoNodi); /*ATTENZIONE: .Richiesta viene passato agli Eventi quando si trasforma una chiamata in evento. la deallocazione
                                                elimina anche la memoria puntata degli eventi CHIAMATA*/
    free(vettcall[i]->Richiesta);
    free(vettcall[i]);
  }
  /*poi quella occupata dai puntatori ad esse. quella occupata dal puntatore a questi puntatori viene deallocata automaticamente perchè in una variabile staticamente dichiarata*/
  free(vettcall);

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
  nella seconda quelli > V[s], e restituisce l'indice dell'elemento separatore
  usa strcmp per l'ordinamento*/
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

/*semplice routine di scambio tra chiamate per indirizzo*/
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

/*Crea un evento vuoto non inizializzato*/
ptevent NewEvent(){
  ptevent ev;

  ev=malloc(sizeof(event));
  if(ev == NULL){
    printf("Errore allocazione Evento: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  return ev;
}

/*a partire da un puntatore a chiamata genera un evento corrispondente a quella chiamata, e restituisce un puntatore ad esso*/
ptevent CallToEvent (ptcall tel){
  ptevent evchiamata;

  evchiamata=NewEvent();
  evchiamata->quest=tel->Richiesta;
  evchiamata->Ora=tel->Ora;
  strcpy(evchiamata->Tipo,"CHIAMATA");
  evchiamata->Auto=0;
  strcpy(evchiamata->Nome,tel->Nome);
  evchiamata->next=NULL;

  return evchiamata;
}

/*Importa la lista delle prime num chiamate, opportunamente in eventi, nell'ordine che i loro puntatori occupano in memoria*/
ptevent ImportaEventoChiamate (ptcall *chiamate, int num){
  int i;
  ptevent evlist, *scorri;

  /*Questo approccio mi permette di non trattare in modo sintatticamente diverso il primo elemento dagli altri*/
  scorri=&evlist;

  for (i=0;i<num;i++){
    *scorri=CallToEvent(chiamate[i]);
    scorri=&((*scorri)->next);
  }

  return evlist;
}

/*Stampa tutta la lista eventi nella formattazione richiesta*/
void printevent(ptevent primo){

  printf("Eventi:\n");
  for(;primo!=NULL;primo=primo->next){
    printf("%d %s %d %s\n",primo->Ora,primo->Tipo,primo->Auto,primo->Nome);
  }
}

/*crea un grafo di NumNodi vertici senza lati. é preferibile passarne l'indirizzo per muovere meno memoria*/
grafo *NewGraph (int NumNodi){
  grafo *Rete;
  int i;

  Rete=malloc(sizeof(grafo));
  if(Rete == NULL){
    printf("Errore allocazione grafo: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  Rete->NumeroNodi = NumNodi;

  Rete->ListaNodi = malloc(NumNodi*sizeof(nodo));
  if(Rete->ListaNodi == NULL){
    printf("Errore allocazione lista nodi: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  /*inizializzo a NULL*/
  for(i=0;i<Rete->NumeroNodi;i++){
    Rete->ListaNodi[i]=NULL;
}

  return Rete;
}

/*Crea un arco verso destinazione di peso peso e restituisce un puntatore ad esso*/
nodo NuovoArcoSlegato (int destinazione, int peso){
  nodo Nuovo;

  Nuovo=malloc(sizeof(arco));
  if(Nuovo == NULL){
    printf("Errore allocazione Nuovo Arco: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  Nuovo->indice=destinazione;
  Nuovo->peso=peso;
  /*avere campo fratello inizializzato a NULL mi permette di trovare la fine delle liste di adiacenza*/
  Nuovo->fratello=NULL;

  return Nuovo;
}

/*Aggiunge un arco non orientato da part a fine al grafo Rete passato per indirizzo*/
void NuovoArco (int part, int fine, int peso, grafo *Rete){
  nodo new;

  new=NuovoArcoSlegato (fine, peso);
  new->fratello=Rete->ListaNodi[part-1];
  Rete->ListaNodi[part-1]=new;

  /*dato che l'arco non è orientato lo aggiungo a entrambe le liste di adiacenza*/
  new=NuovoArcoSlegato (part, peso);
  new->fratello=Rete->ListaNodi[fine-1];
  Rete->ListaNodi[fine-1]=new;
}

/*Funzione di acquisizione dei grafi da file*/
grafo *getgraph (FILE *fp){
  grafo *Rete;
  int i,hmnodes,hmedges,A,B,P;

  /*acquisisco il numero di nodi e di archi da file*/
  fscanf(fp, "%d %d", &hmnodes, &hmedges);
  Rete=NewGraph(hmnodes);

  /*Leggo le righe e aggiungo l'arco corrispondente*/
  for(i=0;i<hmedges;i++){
    fscanf(fp, "%d %d %d\n", &A, &B, &P);
    NuovoArco(A,B,P,Rete);
  }

return Rete;
}

/*funzione di test per stampare le liste di adiacenza*/
void printgraph (grafo *rete){
  int i;
  nodo temp;


  for(i=0;i<rete->NumeroNodi;i++){
    temp=rete->ListaNodi[i];
    printf ("%d ->",i+1);
    while(temp!=NULL){
      printf(" %d",temp->indice);
      temp=temp->fratello;
    }
    printf("\n");
  }

}

/*Funzione di creazione di singoli elementi per lo Heap*/
ElementoHeap* newElemHeap(int v, int dist){
  ElementoHeap* ElemHeap;

  ElemHeap = malloc(sizeof(ElementoHeap));
  if(ElemHeap == NULL){
    printf("Errore allocazione Elemento dello Heap: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  ElemHeap->Vertice = v;
  ElemHeap->Peso = dist;
  return ElemHeap;
}

/*Predispone uno Heap di capacità massima Num*/
Heap* NuovoHeap(int Num){
  Heap* minHeap;

  minHeap=malloc(sizeof(Heap));
  if(minHeap == NULL){
    printf("Errore allocazione Heap: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  minHeap->pos=malloc(Num * sizeof(int));
  if(minHeap->pos == NULL){
    printf("Errore allocazione campo Heap: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  minHeap->NumElementi=0;
  minHeap->MaxElementi=Num;
  minHeap->array=malloc(Num*sizeof(ElementoHeap*));
  if(minHeap->pos == NULL){
    printf("Errore allocazione memoria Heap: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  return minHeap;
}

/*Semplice routine di scambio per elementi dello heap*/
void ScambiaElemHeap(ElementoHeap** a, ElementoHeap** b){
  ElementoHeap* t;

  t = *a;
  *a = *b;
  *b = t;
}

/*Aggiorna il sottoheap AL MINIMO dalla posizione idx, e tiene conto delle modifiche al posizionamento degli elementi, utile per AggiornaDistanza()*/
void AggMinHeap(Heap* minHeap, int idx){
  int minimo, sin, dex;
  ElementoHeap *ElemMin;
  ElementoHeap *Elemidx;


  minimo = idx;
  sin = 2 * idx + 1;
  dex = 2 * idx + 2;

  /*La prima condizione serve ad assicurarsi che l'elemento in posizione sin sia un figlio e non sia esterno allo heap*/
  if ((sin < minHeap->NumElementi)&&(minHeap->array[sin]->Peso < minHeap->array[minimo]->Peso)){
    minimo = sin;
  }
  /*Simile a sopra*/
  if ((dex < minHeap->NumElementi)&&(minHeap->array[dex]->Peso < minHeap->array[minimo]->Peso)){
      minimo = dex;
    }

  /*Se questa porzione di heap è già corretta non aggiorna nè questa nè i suoi discendenti*/
  if (minimo != idx){
      /*copio gli elementi da scambiare*/
      ElemMin = minHeap->array[minimo];
      Elemidx = minHeap->array[idx];

      /*scambio le loro posizioni*/
      minHeap->pos[ElemMin->Vertice] = idx;
      minHeap->pos[Elemidx->Vertice] = minimo;

      /*li scambio effettivamente*/
      ScambiaElemHeap(&minHeap->array[minimo], &minHeap->array[idx]);
      /*richiamo la funzione nel nodo eventualmente modificato*/
      AggMinHeap(minHeap, minimo);
    }
}

/*Versione modificata della funzione precedente che tiene conto dell'indice degli elementi nello heap come secondo criterio di ordinamento (più basso ha priorità)*/
void AggminHeapAuto(Heap* minHeap, int idx){
  int minimo, sin, dex;
  ElementoHeap *ElemMin;
  ElementoHeap *Elemidx;

  minimo = idx;
  sin = 2 * idx + 1;
  dex = 2 * idx + 2;

  /*La prima condizione serve ad assicurarsi che l'elemento in posizione sin sia un figlio e non sia esterno allo heap*/
  if (  (sin < minHeap->NumElementi)&&
        (     (minHeap->array[sin]->Peso < minHeap->array[minimo]->Peso)||
              (     (minHeap->array[sin]->Peso == minHeap->array[minimo]->Peso)&&
                    (minHeap->array[sin]->Vertice < minHeap->array[minimo]->Vertice)
              )
        )
      ){

    minimo = sin;
  }

  /*Simile a sopra*/
  if (  (dex < minHeap->NumElementi)&&
        (     (minHeap->array[dex]->Peso < minHeap->array[minimo]->Peso)||
              (     (minHeap->array[dex]->Peso == minHeap->array[minimo]->Peso)&&
                    (minHeap->array[dex]->Vertice < minHeap->array[minimo]->Vertice)
              )
        )
      ){

      minimo = dex;
    }

  /*Se questa porzione di heap è già corretta non aggiorna nè questa nè i suoi discendenti*/
  if (minimo != idx){
      /*copio gli elementi da scambiare*/
      ElemMin = minHeap->array[minimo];
      Elemidx = minHeap->array[idx];

      /*scambio le loro posizioni*/
      minHeap->pos[ElemMin->Vertice] = idx;
      minHeap->pos[Elemidx->Vertice] = minimo;

      /*li scambio effettivamente*/
      ScambiaElemHeap(&minHeap->array[minimo], &minHeap->array[idx]);
      /*richiamo la funzione nel nodo eventualmente modificato*/
      AggminHeapAuto(minHeap, minimo);
    }
}

/*Rende un array salvato in un Heap uno Heap al Minimo. Usa come lunghezza quella "Attiva"*/
void BuildminHeap(Heap* minHeap){
  int i;

  /*Lo Heap è supposto di lunghezza positiva: la divisione intera quindi arrotonda per difetto*/
  for(i=minHeap->NumElementi/2;i>0;i--){
    AggMinHeap(minHeap,i);
  }
}

/*Versione modificata della precedente funzione per tener conto del secondo criterio di ordinamento sugli indici dei vertici degli elementi*/
void BuildminHeapAuto(Heap* minHeap){
  int i;

  /*Lo Heap è supposto di lunghezza positiva: la divisione intera quindi arrotonda per difetto*/
  for(i=(minHeap->NumElementi-1)/2;i>=0;i--){     /*tolgo 1 perchè gli indici dei vettori contano da 0*/
    AggminHeapAuto(minHeap,i);
  }
}

/*Aggiorna il sottoheap al MASSIMO dalla posizione idx e tiene conto delle modifiche al posizionamento degli elementi*/
void AggMaxHeap(Heap* maxHeap, int idx){
  int massimo, sin, dex;
  ElementoHeap *ElemMax;
  ElementoHeap *Elemidx;

  massimo = idx;
  sin = 2 * idx + 1;
  dex = 2 * idx + 2;

  /*La prima condizione serve ad assicurarsi che l'elemento in posizione sin sia un figlio e non sia esterno allo heap*/
  if ((sin < maxHeap->NumElementi)&&(maxHeap->array[sin]->Peso > maxHeap->array[massimo]->Peso)){
    massimo = sin;
  }
  /*Simile a sopra*/
  if ((dex < maxHeap->NumElementi)&&(maxHeap->array[dex]->Peso > maxHeap->array[massimo]->Peso)){
      massimo = dex;
    }

  /*Se questa porzione di heap è già corretta non aggiorna nè questa nè i suoi discendenti*/
  if (massimo != idx){
      /*copio gli elementi da scambiare*/
      ElemMax = maxHeap->array[massimo];
      Elemidx = maxHeap->array[idx];

      /*scambio le loro posizioni*/
      maxHeap->pos[ElemMax->Vertice] = idx;
      maxHeap->pos[Elemidx->Vertice] = massimo;

      /*li scambio effettivamente*/
      ScambiaElemHeap(&maxHeap->array[massimo], &maxHeap->array[idx]);
      /*richiamo la funzione nel nodo eventualmente modificato*/
      AggMaxHeap(maxHeap, massimo);
    }
}

/*Versione modificata della funzione precedente che tiene conto del secondo criterio dell'ora di chiamata crescente*/
void AggMaxHeapViaggi(Heap* maxHeap, int idx,ptcall *chiamate){
  int massimo, sin, dex;
  ElementoHeap *ElemMax;
  ElementoHeap *Elemidx;
  int check;

  massimo = idx;
  sin = 2 * idx + 1;
  dex = 2 * idx + 2;

  /*La prima condizione serve ad assicurarsi che l'elemento in posizione sin sia un figlio e non sia esterno allo heap*/
  if (  (sin < maxHeap->NumElementi)&&
        (     (maxHeap->array[sin]->Peso > maxHeap->array[massimo]->Peso)||
              (     (maxHeap->array[sin]->Peso == maxHeap->array[massimo]->Peso)&&
                    (chiamate[maxHeap->array[sin]->Vertice]->Ora < chiamate[maxHeap->array[massimo]->Vertice]->Ora)
              )
        )
      ){
    check=maxHeap->array[massimo]->Vertice;
    check=chiamate[maxHeap->array[massimo]->Vertice]->Ora;
    check=maxHeap->array[sin]->Vertice;
    check=chiamate[maxHeap->array[sin]->Vertice]->Ora;


    massimo = sin;
  }

  /*Simile a sopra*/
  if (  (dex < maxHeap->NumElementi)&&
        (     (maxHeap->array[dex]->Peso > maxHeap->array[massimo]->Peso)||
              (     (maxHeap->array[dex]->Peso == maxHeap->array[massimo]->Peso)&&
                    (chiamate[maxHeap->array[dex]->Vertice]->Ora < chiamate[maxHeap->array[massimo]->Vertice]->Ora)
              )
        )
      ){
      check=chiamate[maxHeap->array[massimo]->Vertice]->Ora;
      check=maxHeap->array[dex]->Vertice;
      check=chiamate[maxHeap->array[dex]->Vertice]->Ora;

      massimo = dex;
    }

  /*Se questa porzione di heap è già corretta non aggiorna nè questa nè i suoi discendenti*/
  if (massimo != idx){
      /*copio gli elementi da scambiare*/
      ElemMax = maxHeap->array[massimo];
      Elemidx = maxHeap->array[idx];

      /*scambio le loro posizioni*/
      maxHeap->pos[ElemMax->Vertice] = idx;
      maxHeap->pos[Elemidx->Vertice] = massimo;

      /*li scambio effettivamente*/
      ScambiaElemHeap(&maxHeap->array[massimo], &maxHeap->array[idx]);
      /*richiamo la funzione nel nodo eventualmente modificato*/
      AggMaxHeapViaggi(maxHeap, massimo, chiamate);
    }
}

/*Versione modificata della funzione precedente che tiene conto del secondo criterio dell'indice del nodo nel posizionamento delle auto
OSS: Va bene ogni qual volta l'indice degli elementi nello heap è il secondo criterio di ordinamento (più basso ha priorità)*/
void AggMaxHeapAuto(Heap* maxHeap, int idx){
  int massimo, sin, dex;
  ElementoHeap *ElemMax;
  ElementoHeap *Elemidx;

  massimo = idx;
  sin = 2 * idx + 1;
  dex = 2 * idx + 2;

  /*La prima condizione serve ad assicurarsi che l'elemento in posizione sin sia un figlio e non sia esterno allo heap*/
  if (  (sin < maxHeap->NumElementi)&&
        (     (maxHeap->array[sin]->Peso > maxHeap->array[massimo]->Peso)||
              (     (maxHeap->array[sin]->Peso == maxHeap->array[massimo]->Peso)&&
                    (maxHeap->array[sin]->Vertice < maxHeap->array[massimo]->Vertice)
              )
        )
      ){

    massimo = sin;
  }

  /*Simile a sopra*/
  if (  (dex < maxHeap->NumElementi)&&
        (     (maxHeap->array[dex]->Peso > maxHeap->array[massimo]->Peso)||
              (     (maxHeap->array[dex]->Peso == maxHeap->array[massimo]->Peso)&&
                    (maxHeap->array[dex]->Vertice < maxHeap->array[massimo]->Vertice)
              )
        )
      ){

      massimo = dex;
    }

  /*Se questa porzione di heap è già corretta non aggiorna nè questa nè i suoi discendenti*/
  if (massimo != idx){
      /*copio gli elementi da scambiare*/
      ElemMax = maxHeap->array[massimo];
      Elemidx = maxHeap->array[idx];

      /*scambio le loro posizioni*/
      maxHeap->pos[ElemMax->Vertice] = idx;
      maxHeap->pos[Elemidx->Vertice] = massimo;

      /*li scambio effettivamente*/
      ScambiaElemHeap(&maxHeap->array[massimo], &maxHeap->array[idx]);
      /*richiamo la funzione nel nodo eventualmente modificato*/
      AggMaxHeapAuto(maxHeap, massimo);
    }
}

/*Rende un array salvato in un Heap uno Heap al Massimo. Usa come lunghezza quella "Attiva"*/
void BuildMaxHeap(Heap* maxHeap){
  int i;

  /*Lo Heap è supposto di lunghezza positiva: la divisione intera quindi arrotonda per difetto*/
  for(i=maxHeap->NumElementi/2;i>=0;i--){
    AggMaxHeap(maxHeap,i);
  }
}

/*Versione modificata della precedente funzione per tener conto del secondo criterio di ordinamento sull'arrivo della chiamata*/
void BuildMaxHeapViaggi(Heap* maxHeap, ptcall *chiamate){
  int i;

  /*Lo Heap è supposto di lunghezza positiva: la divisione intera quindi arrotonda per difetto*/
  for(i=(maxHeap->NumElementi-1)/2;i>=0;i--){     /*tolgo 1 perchè gli indici dei vettori contano da 0*/
    AggMaxHeapViaggi(maxHeap,i,chiamate);
  }
}

/*Versione modificata della precedente funzione per tener conto del secondo criterio di ordinamento nella distribuzione delle auto*/
void BuildMaxHeapAuto(Heap* maxHeap){
  int i;

  /*Lo Heap è supposto di lunghezza positiva: la divisione intera quindi arrotonda per difetto*/
  for(i=(maxHeap->NumElementi-1)/2;i>=0;i--){     /*tolgo 1 perchè gli indici dei vettori contano da 0*/
    AggMaxHeapAuto(maxHeap,i);
  }
}

/*indovina*/
int isEmpty(Heap *Heap){
    return Heap->NumElementi == 0;
}

/*estrae il minimo dallo heap, restituisce un puntatore ad esso e lo rimuove concettualmente dallo heap.
 Non fisicamente, in quanto quella regione di memoria è ancora puntata.
 Quando chiamo questa funzione più puntatori (solo tra quelli scartati ho doppi) puntano alle stesse celle di memoria*/
ElementoHeap* extractMin(Heap* minHeap){
  ElementoHeap* Primo;
  ElementoHeap* Ultimo;

    if (isEmpty(minHeap))
        return NULL;

    /*salva il primo elemento*/
    Primo = minHeap->array[0];

    /*Lo sostituisce con l'attuale ultimo*/
    Ultimo = minHeap->array[minHeap->NumElementi-1];
    minHeap->array[0] = Ultimo;

    /*aggiorna le posizioni del nuovo primo elemento e dell'elemento estratto nel vettore delle posizioni*/
    minHeap->pos[Primo->Vertice] = minHeap->NumElementi-1;
    minHeap->pos[Ultimo->Vertice] = 0;

    /*riduce la dimensione dello heap "attivo" e aggiorna lo heap (fa scorrere l'elemento aggiunto fino alla sua legittima posizione)*/
    (minHeap->NumElementi)--;
    AggMinHeap(minHeap, 0);

    return Primo;
}

/*Simile alla precedente ma tiene conto del secondo criterio di ordinamento sugli indici degli elementi dello heap*/
ElementoHeap* extractminAuto(Heap* minHeap){
  ElementoHeap* Primo;
  ElementoHeap* Ultimo;

    if (isEmpty(minHeap))
        return NULL;

    /*salva il primo elemento*/
    Primo = minHeap->array[0];

    /*Lo sostituisce con l'attuale ultimo*/
    Ultimo = minHeap->array[minHeap->NumElementi-1];
    minHeap->array[0] = Ultimo;

    /*aggiorna le posizioni del nuovo primo elemento e dell'elemento estratto nel vettore delle posizioni*/
    minHeap->pos[Primo->Vertice] = minHeap->NumElementi-1;
    minHeap->pos[Ultimo->Vertice] = 0;

    /*riduce la dimensione dello heap "attivo" e aggiorna lo heap (fa scorrere l'elemento aggiunto fino alla sua legittima posizione)*/
    (minHeap->NumElementi)--;
    AggminHeapAuto(minHeap, 0);

    return Primo;
}

/*estrae il Massimo dallo heap, restituisce un puntatore ad esso e lo rimuove concettualmente dallo heap.
 Non fisicamente, in quanto quella regione di memoria è ancora puntata*/
ElementoHeap* extractMax(Heap* maxHeap){
  ElementoHeap* Primo;
  ElementoHeap* Ultimo;

    if (isEmpty(maxHeap))
        return NULL;

    /*salva il primo elemento*/
    Primo = maxHeap->array[0];

    /*Lo sostituisce con l'attuale ultimo*/
    Ultimo = maxHeap->array[maxHeap->NumElementi-1];
    maxHeap->array[0] = Ultimo;

    /*aggiorna le posizioni del nuovo primo elemento e dell'elemento estratto nel vettore delle posizioni*/
    maxHeap->pos[Primo->Vertice] = maxHeap->NumElementi-1;
    maxHeap->pos[Ultimo->Vertice] = 0;

    /*riduce la dimensione dello heap "attivo" e aggiorna lo heap (fa scorrere l'elemento aggiunto fino alla sua legittima posizione)*/
    (maxHeap->NumElementi)--;
    AggMaxHeap(maxHeap, 0);

    return Primo;
}

/*Simile alla precedente ma tiene conto del secondo criterio di ordinamento sui viaggi*/
ElementoHeap* extractMaxViaggi(Heap* maxHeap,ptcall* Chiamate){
  ElementoHeap* Primo;
  ElementoHeap* Ultimo;

    if (isEmpty(maxHeap))
        return NULL;

    /*salva il primo elemento*/
    Primo = maxHeap->array[0];

    /*Lo sostituisce con l'attuale ultimo*/
    Ultimo = maxHeap->array[maxHeap->NumElementi-1];
    maxHeap->array[0] = Ultimo;

    /*aggiorna le posizioni del nuovo primo elemento e dell'elemento estratto nel vettore delle posizioni*/
    maxHeap->pos[Primo->Vertice] = maxHeap->NumElementi-1;
    maxHeap->pos[Ultimo->Vertice] = 0;

    /*riduce la dimensione dello heap "attivo" e aggiorna lo heap (fa scorrere l'elemento aggiunto fino alla sua legittima posizione)*/
    (maxHeap->NumElementi)--;
    AggMaxHeapViaggi(maxHeap, 0,Chiamate);

    return Primo;
}

/*Simile alla precedente ma tiene conto del secondo criterio di ordinamento sul posizionamento delle auto*/
ElementoHeap* extractMaxAuto(Heap* maxHeap){
  ElementoHeap* Primo;
  ElementoHeap* Ultimo;

    if (isEmpty(maxHeap))
        return NULL;

    /*salva il primo elemento*/
    Primo = maxHeap->array[0];

    /*Lo sostituisce con l'attuale ultimo*/
    Ultimo = maxHeap->array[maxHeap->NumElementi-1];
    maxHeap->array[0] = Ultimo;

    /*aggiorna le posizioni del nuovo primo elemento e dell'elemento estratto nel vettore delle posizioni*/
    maxHeap->pos[Primo->Vertice] = maxHeap->NumElementi-1;
    maxHeap->pos[Ultimo->Vertice] = 0;

    /*riduce la dimensione dello heap "attivo" e aggiorna lo heap (fa scorrere l'elemento aggiunto fino alla sua legittima posizione)*/
    (maxHeap->NumElementi)--;
    AggMaxHeapAuto(maxHeap, 0);

    return Primo;
}

/*Modifica la distanza (si intende che viene ridotta) di un vertice di indice v nello heap,
 e poi lo fa salire di tante posizioni quante sono necessarie a mantenere la struttura di heap.
 Usa pos[] per trovare l'indice attuale di v nello heap, così non serve scorrere per contare la posizione, ma lo fa in tempo costante*/
void AggiornaDistanza(Heap* minHeap, int v, int dist){
  int i;

  /*Acqusisce la posizione dell'elemento v-esimo (conta da 0) nello heap*/
  i = minHeap->pos[v];

  /*Aggiorna il suo peso, ovvero la distanza dall'origine nelle chiamate di Dijkstra*/
  minHeap->array[i]->Peso = dist;

  /*fa salire l'elemento nello heap fintanto che non trova un punto dove la condizione di heap è verificata. se i è già 0 sono in cima e quindi non ha senso salire:
  i è messo per primo perchè con la prima condizione di un && falsa la seconda non viene valutata da C. Il ciclo gira in O(logn)*/
  while (i && (minHeap->array[i]->Peso < minHeap->array[(i-1)/2]->Peso))
  {
      /*scambio prima le posizioni e poi gli elementi veri e propri*/
      minHeap->pos[minHeap->array[i]->Vertice] = (i-1)/2;
      minHeap->pos[minHeap->array[(i-1)/2]->Vertice] = i;
      ScambiaElemHeap(&minHeap->array[i], &minHeap->array[(i-1)/2]);

      /*il nuovo indice è quello del padre*/
      i=(i-1)/2;
  }
}

/*Una routine di controllo per verificare se un elemento è o meno in minHeap. Avere il vettore delle posizioni permette questa operazione in O(1)*/
boolean isInmHeap(Heap *minHeap, int v){
   if (minHeap->pos[v] < minHeap->NumElementi){
      return TRUE;
    }
    else{
       return FALSE;
     }
}

/*Funzione che dealloca uno heap, suppoendo che gli elementi "non attivi" siano stati deallocati dopo l'uso quando sono staati estratti*/
void freeHeap(Heap *vHeap){
  int v;
  /*per deallocare gli elementi dello heap non ancora deallocati senza rischiare di liberare la stessa cella due volte chiamo free solo sulle celle "ancora attive"*/
  for(v=0;v<vHeap->NumElementi;v++){/*Nello heap ho ancora NumElementi elementi allocati*/
    free(vHeap->array[v]);
  }
  free(vHeap->array);
  free(vHeap->pos);
  free(vHeap);
}

/*Implementazione dell'algoritmo di Dijkstra per la risoluzione del problema del cammino minimo tra una coppia di nodi dati. Questa Implementazione ha complessità
 O(m logn) dove m è il numero di archi e n il numero di vertici del grafo, grazie all'impiego degli heap per estrarre il minimo valore.
 Osserviamo che m<n^2 ma possiamo supporlo più vicino all'ordine di grandezza di n, data la sensata presunzione che una rete stradale sia rappresentabile da un grafo
 sparso.*/
/*Gli input devono avere come src e dst il NOME del nodo (quindi l'indice che occupa in memoria +1)*/
int dijkstra(grafo* Rete, int src, int dst){
  Heap *minHeap;
  int V, *dist;
  int v,u;
  ElementoHeap* MinimumElement;
  nodo pCrawl;

  src--;/*questi comandi permettono l'uso del nome intuitivo dei nodi senza la traslazione data dal conto che parte da 0 ...*/
  dst--;/*... tipico delle strutture di memoria in cui sono salvati i dati*/

  V = Rete->NumeroNodi;        /*Legge il numero di vertici del grafo*/
  dist=malloc(V*sizeof(int));   /*Vettore per tener conto delle distanze aggiornate dei nodi dall'origine*/

  /*minHeap dovrà rappresentare gli archi del grafo*/
  minHeap = NuovoHeap(V);

  /*inizializzazione dello heap e del vettore delle distanze*/
  for (v = 0; v < V; v++){
      dist[v] = INF;
      minHeap->array[v] = newElemHeap(v, dist[v]);
      minHeap->pos[v] = v;
  }

  /*Aggiorna la distanza di src da se stesso a 0: sarà il primo nodo ad essere estratto dallo while*/
  dist[src] = 0;
  AggiornaDistanza(minHeap, src, dist[src]);
/*OSS: questo approccio all'inizializzazione è più conveniente rispetto a quello in cui si scrive direttamente in posizione src una distanza uguale a 0
in quanto richiede una sola operazione invece di O(n) controlli*/

  /*Inizializzo il numero di elementi "attivi" dello heap*/
  minHeap->NumElementi = V;

  /*il loop esegue fintanto che non ho estratto dallo heap la destinazione, e
  quindi trovato la sua distanza minima da src*/
  while (isInmHeap(minHeap,dst)){

    /*estraggo il vertice più vicino all'origine tra quelli raggiungibili in un passo da vertici per cui ho già stabilito la distanza minima da src*/
    MinimumElement = extractMin(minHeap);
    u = MinimumElement->Vertice; /*salvo l'indice del vertice appena estratto per non dover guardare ogni volta in MinimumElement->Vertice*/

    /*Aggiorno la distanza di tutti i nodi adiacenti al nodo di indice u scegliendo la minima tra la precedente e quella possibile passando da u*/
    pCrawl = Rete->ListaNodi[u]; /*salvo la testa della lista di adiacenza di u per poi scorrerla*/
    while (pCrawl != NULL){
        v = pCrawl->indice - 1; /*tolgo uno perchè in indice ho il nome intuitivo del vertice (contato da 1), e devo farlo corrispondere agli indici delle strutture
                                  in cui sono salvati i suoi dati (contate da 0)*/

        /*Se non ho ancora stabilito la distanza minima definitiva && u fa parte della stessa componente connessa del grafo && la distanza di v è maggiore
          di quella che avrebbe se raggiunto passando da u, aggiorno la distanza di v nel vettore ausiliario e nello heap*/
        if (isInmHeap(minHeap, v) && dist[u] != INF && pCrawl->peso + dist[u] < dist[v]){

            dist[v] = dist[u] + pCrawl->peso;
            AggiornaDistanza(minHeap, v, dist[v]);
        }
        pCrawl = pCrawl->fratello; /*scorro avanti nella lista di adiacenza*/
    }
    free(MinimumElement); /*Dato che lo Heap è interno alla sottoprocedura le parti ormai rimosse e utilizzate possono essere deallocate*/
  }

  /*Devo deallocare la roba rimasta in minHeap e tutto dist, ma prima devo salvarmi il valore da dare come output.
  quando esco dallo while la distanza di dst da src è nella relativa cella del vettore ausiliario delle distanze:
  La metto in u, il cui precedente valore non serve più*/
  u=dist[dst];
  free(dist);

  /*libero il resto dello heap*/
  freeHeap(minHeap);

  return u;
}

/*Funzione alternativa e complementare a Dijkstra che restituisce un vettore di interi in cui il primo elemento (indice 0) è la minima distanza tra src e dst
e gli altri valori sono gli indici reali (nomi) dei nodi attraversati per realizzare tale cammino minimo. Il carattere -1 termina l'array.
è implementata allo stesso modo ma traccia i predecessori, per poi produrre la lista dei nodi attraversati*/
/*questo algoritmo è come il precedente ma richiede fino a 2n di memoria in più e un ciclo O(n), ma mediamente molto più breve*/
int *DijkTragitto(grafo *Rete, int src,int dst){
  Heap *minHeap;
  int V, *dist, *succ, *Tragitto;
  int v,u;
  ElementoHeap* MinimumElement;
  nodo pCrawl;

  V=src-1;   /*Scambio destinazione e origine, cosicchè la lista dei predecessori diventi una lista di successori. Posso perchè il grafo non è orientato. Uso V ...*/
  src=dst-1; /*...come variabile intermedia alla copia perchè verrà inizializzata a breve. diminuisco di 1 come sopra.*/
  dst=V;

  V = Rete->NumeroNodi;        /*Legge il numero di vertici del grafo*/
  dist=malloc(V*sizeof(int));   /*Vettore per tener conto delle distanze aggiornate dei nodi dall'origine*/
  succ=malloc(V*sizeof(int));   /*Vettore per tener traccia dei predecessori nel cammino inverso*/
  Tragitto=malloc((V+2)*sizeof(int)); /*il vettore in cui costruire il tragitto. Inizialmente si era pensato di inizializzarlo a 0 poichè nessun nodo ha nome "0"
                                        ma tale operazione sarebbe stata inutile poichè ogni campo non sovrascritto viene deallocato. Inizializzare avrebbe
                                        richiesto inutilmente un tempo lineare*/
                                    /*OSS: è lungo due in più di V perchè nel caso peggiore contiene tutti i nodi, ma anche la lunghezza del tragitto ed il carattere terminatore*/

  /*minHeap dovrà rappresentare gli archi del grafo*/
  minHeap = NuovoHeap(V);

  /*inizializzazione dello heap e del vettore delle distanze*/
  for (v = 0; v < V; v++){
      dist[v] = INF;
      minHeap->array[v] = newElemHeap(v, dist[v]);
      minHeap->pos[v] = v;
  }

  /*Aggiorna la distanza di src da se stesso a 0: sarà il primo nodo ad essere estratto dallo while*/
  dist[src] = 0;
  succ[src] = src+1; /*la destinazione è succeduta solo da se stessa*/
  AggiornaDistanza(minHeap, src, dist[src]);

  /*Inizializzo il numero di elementi "attivi" dello heap*/
  minHeap->NumElementi = V;

  /*il loop esegue fintanto che non ho estratto dallo heap la destinazione, e quindi trovato la sua distanza minima da src*/
  while (isInmHeap(minHeap,dst)){
    /*estraggo il vertice più vicino all'origine tra quelli raggiungibili in un passo da vertici per cui ho già stabilito la distanza minima da src*/
    MinimumElement = extractMin(minHeap);
    u = MinimumElement->Vertice; /*salvo l'indice del vertice appena estratto per non dover guardare ogni volta in MinimumElement->Vertice*/

    /*Aggiorno la distanza di tutti i nodi adiacenti al nodo di indice u scegliendo la minima tra la precedente e quella possibile passando da u*/
    pCrawl = Rete->ListaNodi[u]; /*salvo la testa della lista di adiacenza di u per poi scorrerla*/
    while (pCrawl != NULL){
        v = pCrawl->indice - 1; /*tolgo uno perchè in indice ho il nome intuitivo del vertice (contato da 1), e devo farlo corrispondere agli indici delle strutture
                                  in cui sono salvati i suoi dati (contate da 0)*/

        /*Se non ho ancora stabilito la distanza minima definitiva && u fa parte della stessa componente connessa del grafo && la distanza di v è maggiore
          di quella che avrebbe se raggiunto passando da u, aggiorno la distanza di v nel vettore ausiliario e nello heap*/
        if (isInmHeap(minHeap, v) && dist[u] != INF && pCrawl->peso + dist[u] < dist[v]){
            succ[v]=u+1;  /*modifico il predecessore di v+1 nel cammino inverso*/
            dist[v] = dist[u] + pCrawl->peso;
            AggiornaDistanza(minHeap, v, dist[v]);
        }
        pCrawl = pCrawl->fratello; /*scorro avanti nella lista di adiacenza*/
    }
  /*Dato che lo Heap è interno alla sottoprocedura le parti ormai rimosse e utilizzate possono essere deallocate*/
  free(MinimumElement);
  }

  /*Dealloco lo Heap che non serve più*/
  freeHeap(minHeap);

  /*uso v per altro: la sua precedente funzione è esaurita*/
  Tragitto[0]=dist[dst]; /*nella prima casella metto la distanza*/
  free(dist);/*Ora il vettore delle distanze non mi serve più, lo dealloco*/

  Tragitto[1]=dst+1; /*nella seconda metto il nodo di partenza del cammino "dritto"*/
  v=2;                   /*ho due elementi in Tragitto*/
  while(succ[dst]!=dst+1){  /*fintanto che il precedente di dst+1 nel cammino inverso è un nodo diverso da sè*/
    Tragitto[v]=succ[dst];  /*metto tale nodo in coda al tragitto "dritto"*/
    dst=Tragitto[v++]-1;      /*Modifico dst nel nodo appena aggiunto, poi incremento v (perchè ora Tragitto ha un elemento in più)*/
  }
  /*A questo punto ho v elementi diversi da 0 in Tragitto. posso riallocarlo in modo da togliere le caselle inutili. L'ultima casella che abbiamo modificato
  contiene il nome del punto di partenza, che non ci serve. la poniamo a 0, che farà da identificatore del temine del vettore, poichè la sua lunghezza non è
  passata a priori*/

  free(succ); /*Ho terminato l'utilizzo di succ, lo dealloco*/

  Tragitto[v]=-1; /*la v+1-esima casella ha indice v e viene posta a -1: Sto aggiungendo un terminatore dopo l'ultimo nodo, quello di partenza. Poi rialloco togliendo le caselle in fondo*/
  Tragitto=realloc(Tragitto,(v+1)*sizeof(int));

  return Tragitto;

}

/*Stampa le chiamate in ordine di durata DECRESCENTE del viaggio associato*/
void PrintChiamViaggio (ptcall *Chiamate, grafo *Rete, int NumChiamate){
  Heap *CHeap;
  ElementoHeap *MaximumElement;
  int i,j;

  CHeap=NuovoHeap(NumChiamate);
  CHeap->NumElementi=NumChiamate; /*Inizialmente lo heap è considerato pieno*/

  /*Inizializzo lo Heap dando ad ogni elemento un indice corrispondente alla chiamata puntata dalla cella di indice medesimo di Chiamate
    ed un peso pari alla lunghezza del cammino minimo tra i nodi della richiesta associata alla chiamata. Aggiorno il tragitto nel viaggio associato*/
  for (i = 0; i < NumChiamate; i++){
      Chiamate[i]->Richiesta->ElencoNodi=DijkTragitto(Rete,Chiamate[i]->Richiesta->Partenza,Chiamate[i]->Richiesta->Arrivo);
      CHeap->array[i] = newElemHeap(i,Chiamate[i]->Richiesta->ElencoNodi[0]);
      CHeap->pos[i] = i;
  }


  /*Creo uno heap sulle distanze dei cammini, tenedo conto del criterio di priorità dell'ordine di arrivo delle chiamate in caso di parità*/
  BuildMaxHeapViaggi(CHeap,Chiamate);

  /*Stampo le chiamate, come richieste, rimuovendo di volta in volta l'elemento corrispondente dello heap e aggiornandolo*/
  while(!isEmpty(CHeap)){ /*Finchè ho chiamate nello Heap*/
    MaximumElement=extractMaxViaggi(CHeap,Chiamate);/*Estraggo l'elemento corrispondente al cammino più lungo*/

    i=MaximumElement->Vertice;/*salvo il suo vertice*/
    printf("%d %s",Chiamate[i]->Ora,Chiamate[i]->Nome); /*stampo ora e nome della chiamata corrispondente*/

    for(j=0;Chiamate[i]->Richiesta->ElencoNodi[j]>0;j++){ /*finchè non ho trovato il carattere di terminazione di ElencoNodi stampo il contenuto*/
      printf(" %d",Chiamate[i]->Richiesta->ElencoNodi[j]);/*ogni elemento è un intero. il primo la distanza, il resto i nodi del tragitto, estremi compresi*/
    }
    printf("\n");
    free(MaximumElement);
  }

  /*Gli elementi puntati dagli elementi di array sono già stati estratti e deallocati uno per uno dallo while ma non è un problema per freeHeap*/
  freeHeap(CHeap);

}

/*Funzione che crea il Parco Auto specificato dal file passato in argomento e restituisce un puntatore ad esso*/
ParcoAuto *CreaAutomobili (FILE *fp){
  int Numero, TMax, Autonomia, TRicarica;
  int i;
  ParcoAuto *PAuto;

  /*Leggo e parso il file*/
  fscanf(fp,"%d %d %d %d ",&Numero,&TMax,&Autonomia,&TRicarica);

  /*Alloco memoria per il parco auto*/
  PAuto=malloc(sizeof(ParcoAuto));
  if(PAuto == NULL){
    printf("Errore allocazione Parco Auto: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  /*Alloco memoria per l'array delle singole auto*/
  PAuto->Taxi=malloc(sizeof(car*)*Numero);
  if(PAuto->Taxi == NULL){
    printf("Errore allocazione Puntatori ai singoli Taxi: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  /*Alloco memoria per le singole auto e compilo i loro campi. Questa operazione non ha una sottofunzione dedicata data la similitudine dell'inizializzazione delle auto*/
  for(i=0;i<Numero;i++){
    PAuto->Taxi[i]=malloc(sizeof(car));
    if(PAuto->Taxi[i] == NULL){
      printf("Errore allocazione Auto %d-esima: %s\n",i+1, strerror(errno));
      exit(EXIT_FAILURE);
    }
    PAuto->Taxi[i]->Numero=i+1;
    PAuto->Taxi[i]->CAutonomia=0;
    PAuto->Taxi[i]->TToTMovimento=0;
    PAuto->Taxi[i]->posizione=1; /*Verrà aggiornata successivamente, ma quella della prima auto è già fissata*/
    PAuto->Taxi[i]->Libera=TRUE;
    PAuto->Taxi[i]->FineRicarica=0;
  }

  /*Compilo i restanti campi di PAuto, dato che sarebbe stato inutile farlo prima in caso di errori di allocazione successivi*/
  PAuto->NumeroAuto=Numero;
  PAuto->AttivitaMax=TMax;
  PAuto->AutonomiaMax=Autonomia;
  PAuto->TempoRicarica=TRicarica;

}

/*Funzione ecologica che dealloca lo spazio occupato dalle auto*/
void Rottama(ParcoAuto *PAuto){
int i;

  for(i=0;i<PAuto->NumeroAuto;i++){
    free(PAuto->Taxi[i]);
  }
  free(PAuto->Taxi);
  free(PAuto);

}

/*Funzione che riceve in argomento il parco auto e la rete stradale e distribuisce i Taxi secondo le specifiche*/
void PlaceCar (grafo *Rete, ParcoAuto *PAuto){
int i,j,n,peep;
Heap *maxHeap;
ElementoHeap *NodoScelto;
  /*Ricordiamo che per assegnamento il taxi in cella 0, di Numero 1, è già assegnato alla sede (nodo 1). annche gli altri, ma non per molto*/

  /*Creo uno Heap Al Massimo delle distanze di ogni Nodo dalla sede*/
  n=Rete->NumeroNodi;
  maxHeap=NuovoHeap(n);
  maxHeap->NumElementi=n;
  for(i=0;i<n;i++){
    maxHeap->array[i] = newElemHeap(n-i-1,dijkstra(Rete,1,n-i)); /*i pesi iniziali sono le distanze dei nodi dall'origine. vertice j corrisponde a distanza 1,j+1*/
    maxHeap->pos[i] = n-i-1;
  }
  /*Ho Riempito lo Heap al contrario, mantenendo la consueta corrispondenza tra pos[] e ->Vertice. In questo modo ho l'elemento dello heap che rappresenta la distanza di 1 da se in fondo*/
  maxHeap->NumElementi--; /*tolgo l'ultimo nodo da quelli attivi*/

  /*Posso ora creare lo heap al massimo della distanza dei nodi dalla sede*/
  BuildMaxHeapAuto(maxHeap);

  for(i=1;i<PAuto->NumeroAuto;i++){ /*Parto da i=1 così da non toccare la prima auto, già associata alla sede*/
    NodoScelto=extractMaxAuto(maxHeap);
    n=NodoScelto->Vertice; /*a Vertice v corrisponde la distanza dal nodo di nome v+1*/
                              /*n è il nome del nodo scelto a questo giro*/
    PAuto->Taxi[i]->posizione=n+1; /*l'auto i-esima ha posizione nel nodo n*/
    for(j=0;j<maxHeap->NumElementi;j++){
      peep=dijkstra(Rete,n+1,(maxHeap->array[j]->Vertice)+1);
      maxHeap->array[j]->Peso=maxHeap->array[j]->Peso+peep; /*Incremento il peso di ogni nodo non scelto della loro distanza dal nodo scelto*/
    }
    free(NodoScelto); /*Dealloco il nodo estratto, non più utile*/
    BuildMaxHeapAuto(maxHeap); /*ricompongo lo heap: dato che i pesi sono cambiati radicalmente devo ricomporlo completamente*/
  }

  /*Qui ogni auto ha una sua posizione, dealloco lo Heap*/
  freeHeap(maxHeap);

}

/*utility per stampare la posizione corrente di ogni auto*/
void StampaPosAuto(ParcoAuto *PAuto){
  int i;

  for(i=0; i<PAuto->NumeroAuto; i++){
    printf("%d ",PAuto->Taxi[i]->posizione);
  }
  printf("\n");
}

/*utility per selezionare l'auto più adatta a servire il viaggio indicato. Ne restituisce un puntatore così che sia agevole modificare gli attributi della vettura. Suppone che le auto libere siano prime in elenco*/
car *ScegliAuto(ParcoAuto *Pauto, grafo *Rete, evento *corsa, boolean *premio){
  Heap *idleHeapPos, idleHeapNeg;
  ElementoHeap *AutoScelta;
  int i,j,Temp;

  if(Pauto->AutoLibere == 0){ /*se non ho auto libere nessuna auto è adatta al servizio*/
    return NULL;
    }

  ArrivoADestinazioneTemp = corsa->quest->OraPartenza + corsa->quest->ElencoNodi[0];/*tempo minimo teorico di soluzione del servizio*/
  if(ArrivoADestinazioneTemp>corsa->quest->OraArrivo){/*se il servizio non è completabile neanche in tempo ottimale nessuna auto è adatta al servizio*/
    return NULL;
  }

  /*indici=malloc(Pauto->AutoLibere*sizeof(int));
  if(indici == NULL){
    printf("Errore allocazione vettore in ScegliAuto: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }*/
  AutoScelta=NULL;

  /*Voglio fare ricerche a volte di massimo e a volte di minimo in uno heap, con comportamenti diversi sopra e sotto lo zero. quindi ne faccio due*/
  idleHeapPos=NuovoHeap(Pauto->AutoLibere);
  idleHeapNeg=NuovoHeap(Pauto->AutoLibere);

  /*inizializzo idleHeapPos e idleHeapNeg*/
  p=0;
  n=0;
  for(i=0;i<Pauto->NumeroAuto;i++){ /*valuto ogni auto libera e stabilisco quanto attenderebbe ferma in caso serva la corsa*/
    if(Pauto->Taxi[i]->Libera==TRUE){
      Temp = corsa->quest->OraPartenza - (dijkstra(Rete,Pauto->Taxi[i]->posizione,corsa->quest->Partenza)+corsa->Ora);
      if(Temp < 0){ /*tempo di idle negativo*/
        idleHeapNeg->array[n] = newElemHeap(i,Temp);
        idleHeapNeg->pos[n] = n;
        n++;
      }
      else{   /*tempo di idle positivo o nullo*/
        idleHeapPos->array[p] = newElemHeap(i,Temp);
        idleHeapPos->pos[p] = p;
        p++;
      }
    }
  }
  idleHeapNeg->NumElementi=n+1;
  idleHeapPos->NumElementi=p+1;

  /*Avere tempo di idle positivo o nullo significa poter servire la chiamata perfettamente. il massimo negativo è altrimenti il miglio candidato per la prima condizione di scelta*/
  /*Il primo criterio è equivalente su tutte le auto di idleHeapPos, quindi si ricorre subito al secondo e terzo. se al termine dell'analisi non si trova una soluzione si lavorerà sul secondo heap*/

  /*qui di seguito si usano gli heap "Auto" in quanto automaticamente tengono conto del terzo criterio.*/
  BuildminHeapAuto(idleHeapPos); /*la scelta migliore, se ammissibile, è quella col tempo di idle non negativo minore*/
  /*le auto con idle positivo arrivano a destinazione esattamente nel minimo tempo possibile, già calcolato*/
  while(idleHeapPos->NumElementi > 0){
    AutoScelta=extractminAuto(idleHeapPos);
    /*se l'elemento è adatto ho finito.  per essere adatto deve poter tornare in sede dopo il servizio*/
    if(dijkstra(Pauto->Taxi[AutoScelta->Vertice]->posizione,corsa->quest->Partenza) + corsa->quest->ElencoNodi[0] + dijkstra(corsa->quest->Arrivo,1) + Pauto->Taxi[AutoScelta->Vertice]->CAutonomia <= Pauto->AutonomiaMax) {
      freeHeap(idleHeapNeg);
      freeHeap(idleHeapPos);
      *premio=TRUE; /*tutte e sole le auto che possono ottenere il premio sono in idleHeapPos*/
      Temp=AutoScelta->Vertice;
      free(AutoScelta);
      return Pauto->Taxi[Temp];    /*se arrivo a questa istruzione AutoScelta è libera, adatta a compiere il servizio ed è la migliore secondo tutti i criteri*/
    }
    else{ /*se l'elemento non è adatto lo butto*/
      free(AutoScelta);
    }
  }
  /*Se esco da questo while ma non dalla funzione è perchè nessun elemento ad idle positivo è valido. cerchiamone uno ad idle negativo*/
  freeHeap(idleHeapPos);
  *premio=FALSE; /*tutte e sole le auto che possono ottenere il premio sono in idleHeapPos*/
  AutoScelta=NULL;/*La memoria collegata è già deallocata. lo reinizializzo in modo da non corrompere la memoria dopo*/

  /*sul secondo heap valutiamo il primo criterio, osservando che una parità nel primo criterio è equivalente alla parità nel secondo poichè il cammino minimo è unico. Teniamo quindi in considerazione solo il primo ed il terzo criterio*/

  /*qui di seguito si usano gli heap "Auto" in quanto automaticamente tengono conto del terzo criterio.*/
  if(idleHeapNeg->NumElementi > 0) do {
    free(AutoScelta);
    AutoScelta=extractMaxAuto(idleHeapNeg);
    }
  /*scarto e ripeto fintanto che ho elementi e quello scelto NON va bene: non ha autonomia*/
  }while((idleHeapNeg->NumElementi > 0)&&(dijkstra(Pauto->Taxi[AutoScelta->Vertice]->posizione,corsa->quest->Partenza) + corsa->quest->ElencoNodi[0] + dijkstra(corsa->quest->Arrivo,1) + Pauto->Taxi[AutoScelta->Vertice]->CAutonomia <= Pauto->AutonomiaMax))
  freeHeap(idleHeapNeg);

  /*BUT WAIT! il veicolo scelto potrebbe anche non andare bene perchè ci mette troppo. Ma se è così ogni veicolo successivo nello heap arriverebbe a destinazione ancora dopo, quindi controllo solo una volta e in caso butto tutto*/
  if(corsa->quest->OraArrivo < (corsa->quest->OraPartenza - AutoScelta->Peso + corsa->quest->ElencoNodi[0]) ){
    free(AutoScelta);
    return NULL;
  }

  Temp=AutoScelta->Vertice;
  free(AutoScelta);
  return Pauto->Taxi[Temp];

}

/*gli unici eventi che possono avere lo stesso tipo senza numero auto corrispondente sono eventi chiamata, ma non ha senso usare questa funzione su due chiamate, quindi si ritiene sufficente limitarsi ai criteri di ora e tipo*/
/*restituisce -1 se il primo precede il secondo, 1 altrimenti*/
int ConfrontoEventi(ptevent evA, ptevent evB){
  if (evA->Ora < evB->Ora){
    return -1;
  }
  else if(evA->Ora > evB->Ora){
    return 1;
  } else if(strcmp(evA->Tipo,evB->Tipo)){
    if((evA->Auto - evB->Auto) < 0){
      return -1;
    }
    else return 1;
  }
  else if(strcmp(evA->Tipo,"FINE_RICARICA")){ /*so già che i tipi sono diversi a questo punto, quindi se A è un evento di fine ricarica, B ha per forza priorità minore*/
    return -1
  }
  else if(strcmp(evA->Tipo,"CHIAMATA")){
    return 1;
  }
  else if(strcmp(evA->Tipo,"RIENTRO_SEDE")){
    if(strcmp(evB->Tipo,"FINE_RICARICA")){
      return 1;
    }
    else return -1;
  }
  else if(strcmp(evA->Tipo,"FINE_SERVIZIO")){
    if(strcmp(evB->Tipo,"CHIAMATA")){
      return -1;
    }
    else return 1;
  }
}

/*Inserisce l'evento al posto giusto scorrendo la lista*/
void InserisciEvento (ptevent TestaLista, ptevent nuovo){
  ptevent *spooler;

  spooler=&TestaLista
  while(ConfrontoEventi(spooler->next,nuovo)<0){
    spooler=&((*spooler)->next);
  }
  *spooler=AddEvent(nuovo,*spooler);

}

/*Crea un evento FINE_RICARICA*/
void FineRicarica(ptevent TestaLista, ParcoAuto *PAuto,car *Auto){
  ptevent fric;

  fric=NewEvent();
  fric->Ora=Auto->FineRicarica;
  strcpy(fric->Tipo,"FINE_RICARICA");
  fric->Auto=Auto->Numero;
  strcpy(fric->Nome,"\0");
  fric->quest=NULL;
  fric->next=NULL;

  InserisciEvento(TestaLista,fric);

}

/*restituisce 0 in caso di successo, 1 altrimenti, ad esempio in caso si rifiuti una chiamata*/
int HandleEvent(ParcoAuto *Pauto, grafo *Rete, evento *event){
car *Auto;
boolean Premio;


  /*Il tipo di evento è assegnato automaticamente e corrisponde a una delle seguenti stringhe nella seguente relazione d'ordine: CHIAMATA < FINE_RICARICA < FINE_SERVIZIO < RIENTRO_SEDE*/
    if(strcmp(event->Tipo,"CHIAMATA")){
      Auto=ScegliAuto(Pauto,Rete,event,&Premio);

    }
    else if(strcmp(event->Tipo,"FINE_RICARICA")){

    }
    else if(strcmp(event->Tipo,"FINE_SERVIZIO")){

    }
    else if(strcmp(event->Tipo,"RIENTRO_SEDE")){

    }
    else printf("errore nella nomenclatura automatica degli eventi: tempo %d\ttipo %s\n",event->Ora,event->Tipo);
}
