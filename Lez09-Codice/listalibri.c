#include "listalibri.h"

listalibri crealistalibri ()
{
  listalibri L;

  L = (listalibri) calloc(1,sizeof(elemento));
  if (L == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione di una lista!\n");
    exit(EXIT_FAILURE);
  }
  L->pred = L;
  L->succ = L;

  return L;
}

void distruggelista (listalibri *pL)
{
  posizione p;

  while (!listavuota(*pL))
  {
    p = primolista(*pL);
    canclista(*pL,&p);
  }

  free(*pL);
  *pL = NULL;
}

void leggelista (listalibri L, posizione p, libro AutoreTitolo)
{
  strcpy(AutoreTitolo,p->AutoreTitolo);
}

listalibri scrivelista (listalibri L, posizione p, libro AutoreTitolo)
{
  strcpy(p->AutoreTitolo,AutoreTitolo);
  return L;
}

boolean listavuota (listalibri L)
{
  return (L->succ == L);
}

posizione primolista (listalibri L)
{
  return L->succ;
}

posizione ultimolista (listalibri L)
{
  return L->pred;
}

posizione succlista (listalibri L, posizione p)
{
  return p->succ;
}

posizione predlista (listalibri L, posizione p)
{
  return p->pred;
}

boolean finelista (listalibri L, posizione p)
{
  return (p == L);
}

listalibri inslista (listalibri L, posizione p, libro AutoreTitolo)
{
  posizione q;

  q = (posizione) calloc(1,sizeof(elemento));
  if (q == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione dell'elemento %s!\n",AutoreTitolo);
    exit(EXIT_FAILURE);
  }
  strcpy(q->AutoreTitolo,AutoreTitolo);
  q->pred = p->pred;
  q->succ = p;

  p->pred->succ = q;  /* NB: non scambiarla con la seguente! */
  p->pred = q;

  return L;
}

listalibri canclista (listalibri L, posizione *pp)
{
  posizione p = *pp;

  *pp = p->succ;
  p->pred->succ = p->succ;
  p->succ->pred = p->pred;
  free(p);

  return L;
}
