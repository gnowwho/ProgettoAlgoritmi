#include "grafo-fs.h"

/* Crea un grafo *pG di n nodi, senza archi */
void crea_grafo (int n, grafo *pG)
{
  int i;

  pG->n = n;
  pG->m = 0;

  pG->FS = (listaarchi *) calloc(pG->n+1,sizeof(listaarchi));
  if (pG->FS ==  NULL)
  {
    fprintf(stderr,"Errore nell'allocazione della forward-star!\n");
    exit(EXIT_FAILURE);
  }

  for (i = 1; i <= n; i++)
    pG->FS[i] = crealistaarchi();
}


/* Distrugge il grafo *pG */
void distrugge_grafo (grafo *pG)
{
  int i;

  for (i = 1; i <= pG->n; i++)
    distruggelistaarchi(&pG->FS[i]);
  free(pG->FS);
  pG->FS = NULL;

  pG->n = 0;
  pG->m = 0;
}


/* Aggiunge l'arco (orig,dest) al grafo *pG.
   Non verifica che l'arco non esista gia'! */
void ins_arco (nodo orig, nodo dest, grafo *pG)
{
  inslistaarchi(pG->FS[orig],succlistaarchi(pG->FS[orig],ultimolistaarchi(pG->FS[orig])),orig,dest);
  pG->m++;
}


/* Cancella l'arco (orig,dest) dal grafo *pG.
   Non verifica che l'arco esista! */
void canc_arco (nodo orig, nodo dest, grafo *pG)
{
  listaarchi L;
  posarco p;
  nodo n1, n2;
  boolean trovato;

  L = pG->FS[orig];
  p = primolistaarchi(L);
  trovato = FALSE;
  while (!trovato && !finelistaarchi(L,p))
  {
    leggearco(L,p,&n1,&n2);
    trovato = ( (n1 == orig) && (n2 == dest) );
    if (!trovato) p = succlistaarchi(L,p);
  }

  if (trovato)
  {
    L = canclistaarchi(L,&p);
    pG->m--;
  }
}


boolean esiste_arco (nodo orig, nodo dest, grafo *pG)
{
  posarco pa;
  nodo no, nd;

  for (pa = primolistaarchi(pG->FS[orig]); !finelistaarchi(pa,pG->FS[orig]); pa = succlistaarchi(pG->FS[orig],pa))
  {
    leggearco(pG->FS[orig],pa,&no,&nd);
    if ( (no == orig) && (nd == dest) ) return TRUE;
  }

  return FALSE;
}


/* ATTENZIONE: SOLUZIONE INCOMPLETA */
posarco primoarco (grafo *pG)
{
  return NO_ARC;
}


/* ATTENZIONE: SOLUZIONE INCOMPLETA */
boolean finearchi (grafo *pG, posarco pa)
{
  return TRUE;
}


/* ATTENZIONE: SOLUZIONE INCOMPLETA */
posarco succarco (grafo *pG, posarco pa)
{
  return NO_ARC;
}


posarco primoarcoFS (grafo *pG, nodo orig)
{
  return primolistaarchi(pG->FS[orig]);
}


boolean finearchiFS (grafo *pG, nodo orig, posarco pa)
{
  return finelistaarchi(pG->FS[orig],pa);
}


posarco succarcoFS (grafo *pG, nodo orig, posarco pa)
{
  return succlistaarchi(pG->FS[orig],pa);
}


nodo leggedest (grafo *pG, nodo orig, posarco pa)
{
  nodo i, j;

  leggearco(pG->FS[orig],pa,&i,&j);
  return j;
}
