#include "albero.h"

albero creaalbero ()
{
  return NO_TREE;
}


/* Visita l'albero in postordine deallocandone i nodi a partire dalle foglie */
void distruggealbero (albero *pT)
{
}


boolean alberovuoto (albero T)
{
  return TRUE;
}


void leggenodo (albero T, posizione r, Operatore *pop, Operando *pval)
{
}


void scrivenodo (albero T, posizione r, Operatore op, Operando val)
{
}


posizione radice (albero T)
{
  return NO_NODE;
}


posizione figliosinistro (albero T, posizione p)
{
  return NO_NODE;
}


posizione figliodestro (albero T, posizione p)
{
  return NO_NODE;
}


posizione padre (albero T, posizione p)
{
  return NO_NODE;
}


albero costruiscealbero (Operatore op, Operando val, albero Ts, albero Td)
{
  return NO_TREE;
}


albero cancsottoalbero (albero T, posizione *pp)
{
  return NO_TREE;
}
