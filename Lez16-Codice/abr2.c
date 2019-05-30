#include <limits.h>
#include "abr.h"

ABR creaABR ()
{
  return NO_TREE;
}


void distruggeABR (ABR *pT)
{
  if (*pT != NO_TREE)
  {
    distruggeABR(&(*pT)->Ts);
    distruggeABR(&(*pT)->Td);
    free(*pT);
    *pT = NO_TREE;
  }
}


boolean ABRvuoto (ABR T)
{
  return (T == NO_TREE);
}


boolean memberABR (int i, ABR T)
{
  if (T == NO_TREE)
    return FALSE;
  else if (T->val == i)
    return TRUE;
  else if (i < T->val)
    return memberABR(i,T->Ts);
  else /* if (i > T->val) */
    return memberABR(i,T->Td);
}


ABR costruisceABR (int i, ABR Ts, ABR Td)
{
  nodo *r;

  /* Alloca l'elemento radice */
  r = (nodo *) malloc(sizeof(nodo));
  if (r == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione di un albero!\n");
    exit(EXIT_FAILURE);
  }

  r->val = val;
  r->padre = NO_TREE;
  r->Ts = Ts;
  if (Ts != NULL) Ts->padre = r;
  r->Td = Td;
  if (Td != NULL) Td->padre = r;

  return r;
}


ABR insertABR (int i, ABR T)
{
  if (T == NO_TREE)
    T = costruisceABR(i,NO_TREE,NO_TREE);
  else if (i < T->val)
  {
    T->Ts = insertABR(i,T->Ts);
    T->Ts->padre = T;
  }
  else if (i > T->val)
  {
    T->Td = insertABR(i,T->Ts);
    T->Td->padre = T;
  }

  return T;
}


ABR deleteABR (int i, ABR T)
{
  return NO_TREE;
}


int minABR (ABR T)
{
  /* Implementazione ricorsiva */
  /*if (T == NO_TREE)
    return INT_MAX;
  else if (T->Ts == NO_TREE)
    return T->val;
  else
    return minABR(T->Ts);*/

  /* Implementazione iterativa */
  if (T == NO_TREE) return INT_MAX;
  while (T->Ts != NO_TREE)
    T = T->Ts;
  return T->val;
}


int maxABR (ABR T)
{
  /* Implementazione ricorsiva */
  /*if (T == NO_TREE)
    return INT_MIN;
  else if (T->Td == NO_TREE)
    return T->val;
  else
    return maxABR(T->Td);*/

  /* Implementazione iterativa */
  if (T == NO_TREE) return INT_MIN;
  while (T->Td != NO_TREE)
    T = T->Td;
  return T->val;
}


void stampaABR (ABR T)
{
  if (T != NO_TREE)
  {
    stampaABR(T->Ts);
    printf(" %d",T->val);
    stampaABR(T->Td);
  }
}
