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


/* Cancella il nodo n dall'albero T, assumendo che abbia al massimo un figlio */
ABR toglienodoABR (nodo *n, ABR T)
{
  ABR Tf;

  /* Determina l'unico sottoalbero esistente (eventualmente NO_TREE) */
  Tf = n->Ts;
  if (Tf == NO_TREE) Tf = n->Td;

  /* Se c'e' un sottoalbero lo appende al padre (eventualmente NO_NODE) */
  if (Tf != NO_TREE) Tf->padre = n->padre;

  /* Se il nodo cancellato e' la radice, aggiorna l'albero */
  if (n->padre == NULL)
    T = Tf;
  else /* Se il nodo cancellato ha un padre, determina in quale sottoalbero del padre sta
          e sostituisce a se stesso il proprio sottoalbero */
  {
    if (n == n->padre->Ts)
      n->padre->Ts = Tf;
    else if (n == n->padre->Td)
      n->padre->Td = Tf;
  }

  /* Finalmente, dealloca il nodo */
  free(n);

  return T;
}


nodo *argmaxABR (ABR T)
{
  if (T == NO_TREE) return NO_TREE;
  while (T->Td != NO_TREE)
    T = T->Td;
  return T;
}


ABR deleteABR (int i, ABR T)
{
  nodo *n;

  /* Se l'albero e' vuoto, non fa nulla */
  if (T != NO_TREE)
  {
    if (i < T->val)
      T->Ts = deleteABR(i,T->Ts);
    else if (i > T->val)
      T->Td = deleteABR(i,T->Td);
    else if ( (T->Ts == NO_TREE) || (T->Td == NO_TREE) )
      /* Almeno un sottoalbero è vuoto: cancella il nodo e appene il sottoalbero al padre */
      T = toglienodoABR(T,T);
    else
    {
      /* Ci sono due sottoalberi: sovrascrive il nodo con l'elemento massimo del sottoalbero sinistro
         e cancella quest'ultimo */
      n = argmaxABR(T->Ts);
      T->val = n->val;
      T = toglienodoABR(n,T);
    }
  }

  return T;
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
