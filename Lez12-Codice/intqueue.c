#include "intqueue.h"


void CreateQueue (intqueue *pQ, int size)
{
  pQ->V = (int *) calloc(size,sizeof(int));
  if (pQ->V == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione della coda!\n");
    exit(EXIT_FAILURE);
  }

  pQ->size = size;      //quanta memoria allocata ho? size (ricorda che in aritmetica dei puntatori punt+1=punt+sizeof(tipo puntato))
  pQ->tail = 0;
  pQ->head = 0;
}


void DestroyQueue (intqueue *pQ)
{
  free(pQ->V);  //da dare prima del comando successivo, necessariamente, o perdo memoria in giro
  pQ->V = NULL;
  pQ->size = 0;
  pQ->tail = 0;
  pQ->head = 0;
}


boolean IsEmptyQueue (intqueue *pQ)
{
  return (pQ->head == pQ->tail);
}


int Front (intqueue *pQ)
{
  return pQ->V[pQ->head];
}


void Enqueue (int i, intqueue *pQ)
{
  pQ->V[pQ->tail] = i;
  pQ->tail++;
  /* In caso di aritmetica modulare------> in questo caso risolve i problemi di
                                           algoritmi in cui rischio di sfasare
                                           inizio e fine della coda con l'inizio
                                           e la fine della memoria in cui la
                                           salvo*/
     if (pQ->tail >= pQ->size) pQ->tail -= pQ->size;
}


void Dequeue (intqueue *pQ)
{
  pQ->head++;
  /* In caso di aritmetica modulare   sia qui che sopra, in questo problema è superflua*/
     if (pQ->head >= pQ->size) pQ->head -= pQ->size;
}
