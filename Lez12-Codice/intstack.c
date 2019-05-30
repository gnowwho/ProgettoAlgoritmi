#include "intstack.h"

void CreateStack (intstack *pS, int size)
{
  pS->V = (int *) calloc(size,sizeof(int));
  if (pS->V == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione della pila!\n");
    exit(EXIT_FAILURE);
  }

  pS->size = size;
  pS->top = -1;
}


void DestroyStack (intstack *pS)
{
  free(pS->V);
  pS->V = NULL;
  pS->size = 0;
  pS->top = -1;
}


boolean IsEmptyStack (intstack *pS)
{
  return (pS->top == -1);
}


int Top (intstack *pS)
{
  return pS->V[pS->top];
}


void Push (int i, intstack *pS)
{
  pS->top++;
  pS->V[pS->top] = i;
}


void Pop (intstack *pS)
{
  pS->top--;
}
