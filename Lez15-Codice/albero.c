#include "albero.h"
//ogni cosa è a tempo costante tranne creazione e distruzione che sono lineari sul numero di nodi
albero creaalbero ()
{
  return NO_TREE;
}


/* Visita l'albero in postordine deallocandone i nodi a partire dalle foglie */
void distruggealbero (albero *pT)
{
  cancsottoalbero(*pT,pT);
}


boolean alberovuoto (albero T)
{
  return (T == NO_TREE);
}


void leggenodo (albero T, posizione p, Operatore *pop, Operando *pval)
{
  *pop = p->op;
  *pval = p->val;
}


void scrivenodo (albero T, posizione p, Operatore op, Operando val)
{
  p->op = op;
  p->val = val;
}


posizione radice (albero T)
{
  return T;
}


posizione figliosinistro (albero T, posizione p)
{
  return p->Ts;
}


posizione figliodestro (albero T, posizione p)
{
  return p->Td;
}


posizione padre (albero T, posizione p)
{
  return p->padre;
}


albero costruiscealbero (Operatore op, Operando val, albero Ts, albero Td) /*uno di questi alberi ha come elemento un valore o
                                                          un operatore, che sono oggetti di tipo diverso e dal senso diverso*/
{
  nodo *r;

  /* Alloca l'elemento radice */
  r = (nodo *) malloc(sizeof(nodo));
  if (r == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione di un albero!\n");
    exit(EXIT_FAILURE);
  }

  r->op = op;
  r->val = val;
  r->padre = NO_NODE;//<--------------
  r->Ts = Ts;//                       \
  if (Ts != NULL) Ts->padre = r;//<----------questa roba va gestita così SE decido di implementare il campo "Padre"
  r->Td = Td;//                       |
  if (Td != NULL) Td->padre = r;//<---

  return r;
}


albero cancsottoalbero (albero T, posizione *pp) /*per cancellare un sottoalbero devo prima cancellare i suoi sottoalberi e deallocare tutto con ordine*/
{
  if (*pp != NO_NODE)
  {
    cancsottoalbero(T,&(*pp)->Ts);
    cancsottoalbero(T,&(*pp)->Td);
    free(*pp);
    *pp = NO_NODE;
  }

  return T;
}
