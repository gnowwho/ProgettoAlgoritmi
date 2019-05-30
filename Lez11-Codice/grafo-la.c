#include "grafo-la.h"

void crea_grafo (int n, grafo *pG)
{
  pG->n = n;
  pG->m = 0;
  pG->L = crealistaarchi();
}


void distrugge_grafo (grafo *pG)
{
  pG->n = 0;
  pG->m = 0;
  distruggelistaarchi(&pG->L);
}


void ins_arco (nodo orig, nodo dest, grafo *pG)
{
  inslistaarchi(pG->L,succlistaarchi(pG->L,ultimolistaarchi(pG->L)),orig,dest);
  pG->m++;
}


void canc_arco (nodo orig, nodo dest, grafo *pG)
{
  posarco p;
  nodo n1, n2;
  boolean trovato;

  p = primolistaarchi(pG->L);
  trovato = FALSE;
  while (!trovato && !finelistaarchi(pG->L,p))
  {
    leggearco(pG->L,p,&n1,&n2);
    trovato = ( (n1 == orig) && (n2 == dest) );
    if (!trovato) p = succlistaarchi(pG->L,p);
  }

  if (trovato)
  {
    canclistaarchi(pG->L,p);
    pG->m--;
  }
}


boolean esiste_arco (nodo orig, nodo dest, grafo *pG)
{
  posarco pa;
  nodo no, nd;

  for (pa = primolistaarchi(pG->L); !finelistaarchi(pG->L,pa); pa = succlistaarchi(pG->L,pa))
  {
    leggearco(pG->L,pa,&no,&nd);
    if ( (no == orig) && (nd == dest) ) return TRUE;
  }

  return FALSE;
}


posarco primoarco (grafo *pG)
{
  return primolistaarchi(pG->L);
}


boolean finearchi (posarco pa, grafo *pG)
{
  return finelistaarchi(pG->L,pa);
}


posarco succarco (posarco pa, grafo *pG)
{
  return succlistaarchi(pG->L,pa);
}


void leggeestremiarco (posarco pa, grafo *pG, nodo *porig, nodo *pdest)
{
  leggearco(pG->L,pa,porig,pdest);
}
