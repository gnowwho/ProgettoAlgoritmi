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
    fscanf(fp,"%d %s %d %d %d %d %d\n", &vettcall[i]->Ora,
                                        vettcall[i]->Nome,
                                        &vettcall[i]->Partenza,
                                        &vettcall[i]->Arrivo,
                                        &vettcall[i]->OraPartenza,
                                        &vettcall[i]->OraArrivo,
                                        &vettcall[i]->Premio);
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
                                      vettcall[i]->Partenza,
                                      vettcall[i]->Arrivo,
                                      vettcall[i]->OraPartenza,
                                      vettcall[i]->OraArrivo,
                                      vettcall[i]->Premio);
  }

}

void freecalls (ptcall *vettcall, int k){
  int i;

  /*prima dealloco la memoria occupata dalle chiamate*/
  for(i=0; i<k; i++){
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
  evchiamata->quest=malloc(sizeof(viaggio));
  evchiamata->Ora=tel->Ora;
  strcpy(evchiamata->Tipo,"CHIAMATA");
  evchiamata->Auto=0;
  strcpy(evchiamata->Nome,tel->Nome);
  evchiamata->quest->Partenza=tel->Partenza;
  evchiamata->quest->Arrivo=tel->Arrivo;
  evchiamata->quest->OraPartenza=tel->OraArrivo;
  evchiamata->quest->Premio=tel->Premio;
  evchiamata->next=NULL;

  return evchiamata;
}

/*Importa la lista delle chiamate, opportunamente in eventi, nell'ordine che i loro puntatori occupano in memoria*/
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
mHeap* NuovomHeap(int Num){
  mHeap* minHeap;

  minHeap=malloc(sizeof(mHeap));
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

/*Aggiorna il sottoheap dalla posizione idx, e tiene conto delle modifiche al posizionamento degli elementi, utile per AggiornaDistanza()*/
void AggiornamHeap(mHeap* minHeap, int idx){
  int minimo, sin, dex;
  ElementoHeap *ElemMin;
  ElementoHeap *Elemidx;


  minimo = idx;
  sin = 2 * idx + 1;
  dex = 2 * idx + 2;

  if ((sin < minHeap->NumElementi)&&(minHeap->array[sin]->Peso < minHeap->array[minimo]->Peso)){
    minimo = sin;
  }

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
      AggiornamHeap(minHeap, minimo);
    }
}

/*indovina*/
int isEmpty(mHeap* minHeap){
    return minHeap->NumElementi == 0;
}

/*estrae il minimo dallo heap, restituisce un puntatore ad esso e lo rimuove concettualmente dallo heap.
 Non fisicamente, in quanto quella regione di memoria è ancora puntata*/
ElementoHeap* extractMin(mHeap* minHeap){
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
    AggiornamHeap(minHeap, 0);

    return Primo;
}

/*Modifica la distanza (si intende che viene ridotta) di un vertice di indice v nello heap,
 e poi lo fa salire di tante posizioni quante sono necessarie a mantenere la struttura di heap.
 Usa pos[] per trovare l'indice attuale di v nello heap, così non serve scorrere per contare la posizione, ma lo fa in tempo costante*/
void AggiornaDistanza(mHeap* minHeap, int v, int dist){
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
boolean isInmHeap(mHeap *minHeap, int v){
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
  mHeap *minHeap;
  int V, *dist;
  int v,u;
  ElementoHeap* MinimumElement;
  nodo pCrawl;

  src--;/*questi comandi permettono l'uso del nome intuitivo dei nodi senza la traslazione data dal conto che parte da 0 ...*/
  dst--;/*... tipico delle strutture di memoria in cui sono salvati i dati*/

  V = Rete->NumeroNodi;        /*Legge il numero di vertici del grafo*/
  dist=malloc(V*sizeof(int));   /*Vettore per tener conto delle distanze aggiornate dei nodi dall'origine*/

  /*minHeap dovrà rappresentare gli archi del grafo*/
  minHeap = NuovomHeap(V);

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
  while (isInmHeap(minHeap,dst))
  {
      /*estraggo il vertice più vicino all'origine tra quelli raggiungibili in un passo da vertici per cui ho già stabilito la distanza minima da src*/
      MinimumElement = extractMin(minHeap);
      u = MinimumElement->Vertice; /*salvo l'indice del vertice appena estratto per non dover guardare ogni volta in MinimumElement->Vertice*/

      /*Aggiorno la distanza di tutti i nodi adiacenti al nodo di indice u scegliendo la minima tra la precedente e quella possibile passando da u*/
      pCrawl = Rete->ListaNodi[u]; /*salvo la testa della lista di adiacenza di u per poi scorrerla*/
      while (pCrawl != NULL){
          v = pCrawl->indice - 1; /*tolgo uno perchè in indice ho il nome intuitivo del vertice (contato da 1), e devo farlo corrispondere agli indici delle strutture
                                    in cui sono salvati i suoi dati (contate da 0)*/

          // If shortest distance to v is not finalized yet, and distance to v
          // through u is less than its previously calculated distance
          /*Se non ho ancora stabilito la distanza minima definitiva && u fa parte della stessa componente connessa del grafo && la distanza di v è maggiore
            di quella che avrebbe se raggiunto passando da u, aggiorno la distanza di v nel vettore ausiliario e nello heap*/
          if (isInmHeap(minHeap, v) && dist[u] != INF && pCrawl->peso + dist[u] < dist[v]){

              dist[v] = dist[u] + pCrawl->peso;
              AggiornaDistanza(minHeap, v, dist[v]);
          }
          pCrawl = pCrawl->fratello; /*scorro avanti nella lista di adiacenza*/
      }
  }

  /*quando esco dallo while la distanza di dst da src è nella relativa cella del vettore ausiliario delle distanze*/
  return dist[dst];
}

/*Funzione alternativa e complementare a Dijkstra che restituisce un vettore di interi in cui il primo elemento (indice 0) è la minima distanza tra src e dst
e gli altri valori sono gli indici reali (nomi) dei nodi attraversati per realizzare tale cammino minimo.
è implementata allo stesso modo ma traccia i predecessori, per poi produrre la lista dei nodi attraversati*/
/*questo algoritmo è come il precedente ma richiede fino a 2n di memoria in più e un ciclo O(n), ma mediamente molto più breve*/
int *DijkTragitto(grafo *Rete, int src,int dst,int ){
  mHeap *minHeap;
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
  Tragitto=malloc((V+1)*sizeof(int)); /*il vettore in cui costruire il tragitto. Inizialmente si era pensato di inizializzarlo a 0 poichè nessun nodo ha nome "0"
                                        ma tale operazione sarebbe stata inutile poichè ogni campo non sovrascritto viene deallocato. Inizializzare avrebbe
                                        richiesto inutilmente un tempo lineare*/
                                    /*OSS: è lungo uno in più perchè la prima casella contiene la distanza minima tra src e dst*/

  /*minHeap dovrà rappresentare gli archi del grafo*/
  minHeap = NuovomHeap(V);

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
  while (isInmHeap(minHeap,dst))
  {
      /*estraggo il vertice più vicino all'origine tra quelli raggiungibili in un passo da vertici per cui ho già stabilito la distanza minima da src*/
      MinimumElement = extractMin(minHeap);
      u = MinimumElement->Vertice; /*salvo l'indice del vertice appena estratto per non dover guardare ogni volta in MinimumElement->Vertice*/

      /*Aggiorno la distanza di tutti i nodi adiacenti al nodo di indice u scegliendo la minima tra la precedente e quella possibile passando da u*/
      pCrawl = Rete->ListaNodi[u]; /*salvo la testa della lista di adiacenza di u per poi scorrerla*/
      while (pCrawl != NULL){
          v = pCrawl->indice - 1; /*tolgo uno perchè in indice ho il nome intuitivo del vertice (contato da 1), e devo farlo corrispondere agli indici delle strutture
                                    in cui sono salvati i suoi dati (contate da 0)*/

          // If shortest distance to v is not finalized yet, and distance to v
          // through u is less than its previously calculated distance
          /*Se non ho ancora stabilito la distanza minima definitiva && u fa parte della stessa componente connessa del grafo && la distanza di v è maggiore
            di quella che avrebbe se raggiunto passando da u, aggiorno la distanza di v nel vettore ausiliario e nello heap*/
          if (isInmHeap(minHeap, v) && dist[u] != INF && pCrawl->peso + dist[u] < dist[v]){
              succ[v]=u+1;  /*modifico il predecessore di v+1 nel cammino inverso*/
              dist[v] = dist[u] + pCrawl->peso;
              AggiornaDistanza(minHeap, v, dist[v]);
          }
          pCrawl = pCrawl->fratello; /*scorro avanti nella lista di adiacenza*/
      }
  }

  /*uso v per altro: la sua precedente funzione è esaurita*/
  Tragitto[0]=dist[dst]; /*nella prima casella metto la distanza*/
  v=1;                   /*ho un elemento in Tragitto*/
  while(succ[dst]!=dst+1){  /*fintanto che il precedente di dst+1 nel cammino inverso è un nodo diverso da sè*/
    Tragitto[v]=succ[dst];  /*metto tale nodo in coda al tragitto "dritto"*/
    dst=Tragitto[v++]-1;      /*Modifico dst nel nodo appena aggiunto, poi incremento v (perchè ora Tragitto ha un elemento in più)*/
  }
  /*A questo punto ho v elementi diversi da 0 in Tragitto. posso riallocarlo in modo da togliere le caselle inutili. L'ultima casella che abbiamo modificato
  contiene il nome del punto di partenza, che non ci serve. la poniamo a 0, che farà da identificatore del temine del vettore, poichè la sua lunghezza non è
  passata a priori*/

  Tragitto[v]=-1; /*la v+1-esima casella ha indice v e viene azzerata. Poi rialloco togliendo le caselle in fondo*/
  Tragitto=realloc(Tragitto,(v+1)*sizeof(int));
  return Tragitto;

}
