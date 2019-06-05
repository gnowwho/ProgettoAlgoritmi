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

/*a partire da un puntatore a chiamata genera un evento corrispondente a quella chiamata, e restituisce un puntatore ad esso*/
ptevent CallToEvent (ptcall tel){
  ptevent evchiamata;

  evchiamata=malloc(sizeof(event));
  if(evchiamata == NULL){
    printf("Errore allocazione Lista eventi: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }
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

/*Non Usata*/
/*Rende un array salvato in un Heap uno Heap al Minimo. Usa come lunghezza quella "Attiva"*/
void BuildminHeap(Heap* minHeap){
  int i;

  /*Lo Heap è supposto di lunghezza positiva: la divisione intera quindi arrotonda per difetto*/
  for(i=minHeap->NumElementi/2;i>0;i--){
    AggMinHeap(minHeap,i);
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
  /*per deallocare gli elementi dello heap non ancora deallocati senza rischiare di liberare la stessa cella due volte chiamo free solo sulle celle "ancora attive"*/
  for(v=0;v<minHeap->NumElementi;v++){/*v non serve più. Nello heap ho ancora NumElementi elementi allocati*/
    free(minHeap->array[v]);
  }
  free(minHeap->array);
  free(minHeap);

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
  /*per deallocare gli elementi dello heap non ancora deallocati senza rischiare di liberare la stessa cella due volte chiamo free solo sulle celle "ancora attive"*/
  for(v=0;v<minHeap->NumElementi;v++){/*v non serve più. Nello heap ho ancora NumElementi elementi allocati*/
    free(minHeap->array[v]);
  }
  free(minHeap->array);
  free(minHeap);


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
    printf("%d\t %s\t\t",Chiamate[i]->Ora,Chiamate[i]->Nome); /*stampo ora e nome della chiamata corrispondente*/

    for(j=0;Chiamate[i]->Richiesta->ElencoNodi[j]>0;j++){ /*finchè non ho trovato il carattere di terminazione di ElencoNodi stampo il contenuto*/
      printf(" %d",Chiamate[i]->Richiesta->ElencoNodi[j]);/*ogni elemento è un intero. il primo la distanza, il resto i nodi del tragitto, estremi compresi*/
    }
    printf("\n");
    free(MaximumElement);
  }

  free(CHeap->pos);
  /*Gli elementi puntati dagli elementi di array sono già stati estratti e deallocati uno per uno dallo while*/
  free(CHeap->array);
  free(CHeap);

}
