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
  }
 return vettcall;

}

/*Stampa un elenco di k chiamate nell'ordine che i loro puntatori occupano in memoria. per iniziare da una posizione diversa dalla prima basta passare il puntatore
spostato di alcune posizioni: per aritmetica dei puntatori basta sommarvi un intero*/
void printcalls (ptcall *vettcall, int k){
  int i;

  for(i=0;i<k;i++){
    printf("%d %s %d %d %d %d %d\n", vettcall[i]->Ora,
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

  for(i=0; i<k; i++){
    free(vettcall[i]);
  }

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

/*Crea un arco verso ... e restituisce un puntatore ad esso*/
nodo NuovoArcoSlegato (int destinazione, int peso){
  nodo Nuovo;

  Nuovo=malloc(sizeof(arco));
  if(Nuovo == NULL){
    printf("Errore allocazione Nuovo Arco: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
  }
  Nuovo->indice=destinazione;
  Nuovo->peso=peso;
  Nuovo->fratello=NULL;

  return Nuovo;
}

/*Aggiunge un arco non orientato da part a fine al grafo Rete passato per indirizzo*/
void NuovoArco (int part, int fine, int peso, grafo *Rete){
  nodo new;

  new=NuovoArcoSlegato (fine, peso);
  new->fratello=Rete->ListaNodi[part-1];
  Rete->ListaNodi[part-1]=new;

  new=NuovoArcoSlegato (part, peso);
  new->fratello=Rete->ListaNodi[fine-1];
  Rete->ListaNodi[fine-1]=new;
}

/*Funzione di acquisizione dei grafi da file*/
grafo *getgraph (FILE *fp){
  grafo *Rete;
  int i,hmnodes,hmedges,A,B,P;

  fscanf(fp, "%d %d", &hmnodes, &hmedges);
  Rete=NewGraph(hmnodes);

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

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/* Funzione di creazione di singoli elementi per lo Heap*/
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

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for AggiornaDistanza()

void AggiornamHeap(mHeap* minHeap, int idx){
  int smallest, left, right;
  ElementoHeap *smallestNode;
  ElementoHeap *idxNode;


    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->NumElementi &&
        minHeap->array[left]->Peso < minHeap->array[smallest]->Peso )
      smallest = left;

    if (right < minHeap->NumElementi &&
        minHeap->array[right]->Peso < minHeap->array[smallest]->Peso )
      smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        smallestNode = minHeap->array[smallest];
        idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->Vertice] = idx;
        minHeap->pos[idxNode->Vertice] = smallest;

        // Swap nodes
        ScambiaElemHeap(&minHeap->array[smallest], &minHeap->array[idx]);
        AggiornamHeap(minHeap, smallest);
    }
}

// A utility function to check if the given minHeap is ampty or not
int isEmpty(mHeap* minHeap){
    return minHeap->NumElementi == 0;
}

// Standard function to extract minimum node from heap
ElementoHeap* extractMin(mHeap* minHeap){
  ElementoHeap* root;
  ElementoHeap* lastNode;

    if (isEmpty(minHeap))
        return NULL;

    /*salva il primo nodo*/
    root = minHeap->array[0];

    /*Lo sostituisce con l'attuale ultimo*/
    lastNode = minHeap->array[minHeap->NumElementi-1];
    minHeap->array[0] = lastNode;

    /*aggiorna le posizioni del nuovo primo elemento e dell'elemento estratto nel vettore delle posizioni*/
    minHeap->pos[root->Vertice] = minHeap->NumElementi-1;
    minHeap->pos[lastNode->Vertice] = 0;

    // Reduce heap size and heapify root
    --minHeap->NumElementi;
    AggiornamHeap(minHeap, 0);

    return root;
}

/* Modifica la distanza (si intende che viene ridotta) di un vertice di indice v
nello heap, e poi lo fa salire di tante posizioni quante sono necessarie a mantenere la struttura di heap.
 Usa pos[] per trovare l'indice attuale di v nello heap, così non serve scorrere per contare la posizione*/
void AggiornaDistanza(mHeap* minHeap, int v, int dist){
    // Get the index of v in  heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->Peso = dist;

    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->Peso < minHeap->array[(i - 1) / 2]->Peso)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->Vertice] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->Vertice] = i;
        ScambiaElemHeap(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
boolean isInmHeap(mHeap *minHeap, int v){
   if (minHeap->pos[v] < minHeap->NumElementi){
      return TRUE;
    }
    else{
       return FALSE;
     }
}

// The main function that calulates distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
int dijkstra(grafo* graph, int src, int dst){
  mHeap *minHeap;
  int V, *dist;
  int v,u;
  ElementoHeap* MinimumElement;
  nodo pCrawl;


    V = graph->NumeroNodi; /*Legge il numero di vertici del grafo*/
    dist=malloc(V*sizeof(int)); /*Vettore per tener conto delle distanze aggiornate dei nodi dall'origine*/

    /*minHeap dovrà rappresentare gli archi del grafo*/
    minHeap = NuovomHeap(V);

    /*inizializzazione dello heap e del vettore delle distanze*/
    for (v = 0; v < V; v++){
        dist[v] = INF;
        minHeap->array[v] = newElemHeap(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    dist[src] = 0;
    AggiornaDistanza(minHeap, src, dist[src]);
    /*dist[src] = 0;
    minHeap->array[src] = newElemHeap(src, dist[src]);
    minHeap->pos[src]   = src;*/


    // Initially NumElementi of min heap is equal to V
    minHeap->NumElementi = V;

    /*il loop esegue fintanto che non ho estratto dallo heap la destinazione, e
    quindi trovato la sua distanza minima da src*/
    while (isInmHeap(minHeap,dst))
    {
        // Extract the vertex with minimum distance value
        MinimumElement = extractMin(minHeap);
        u = MinimumElement->Vertice; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        pCrawl = graph->ListaNodi[u];
        while (pCrawl != NULL){
            v = pCrawl->indice - 1;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInmHeap(minHeap, v) && dist[u] != INF &&
                                          pCrawl->peso + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->peso;

                // update distance value in min heap also
                AggiornaDistanza(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->fratello;
        }
    }

    // print the calculated shortest distances
    return dist[dst];
}
