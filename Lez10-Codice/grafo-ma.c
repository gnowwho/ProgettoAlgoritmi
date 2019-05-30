#include "grafo-ma.h"


/* Crea un grafo *pG di n nodi senza archi */
void crea_grafo (int n, grafo *pG)
{
  int i;

  pG->n = n;
  pG->m = 0;

  pG->MatriceAdiacenza = (boolean **) calloc(n+1,sizeof(boolean *));
  if (pG->MatriceAdiacenza == NULL)
  {
    fprintf(stderr,"Errore nell'allocazione della matrice di adiacenza!\n");
    exit(EXIT_FAILURE);
  }

  for (i = 1; i <= n; i++)
  {
    pG->MatriceAdiacenza[i] = (boolean *) calloc(n+1,sizeof(boolean));
    if (pG->MatriceAdiacenza[i] == NULL)
    {
      fprintf(stderr,"Errore nell'allocazione della riga %d della matrice di adiacenza!\n",i);
      exit(EXIT_FAILURE);
    }
  }
}


/* Distrugge il grafo *pG */
void distrugge_grafo (grafo *pG)
{
  int i;

  for (i = 1; i <= pG->n; i++)
    free(pG->MatriceAdiacenza[i]);
  free(pG->MatriceAdiacenza);
  pG->MatriceAdiacenza = NULL;

  pG->n = 0;
  pG->m = 0;
}


/* Aggiunge l'arco (orig,dest) al grafo *pG.
   Non verifica che l'arco non esista gia'! */
void ins_arco (int orig, int dest, grafo *pG)
{
  pG->MatriceAdiacenza[orig][dest] = TRUE;
  pG->m++;
}


/* Cancella l'arco (orig,dest) dal grafo *pG.
   Non verifica che l'arco esista! */
void canc_arco (int orig, int dest, grafo *pG)
{
  pG->MatriceAdiacenza[orig][dest] = FALSE;
  pG->m--;
}


/* Determina se l'arco (orig,dest) appartiene al grafo *pG */
boolean esiste_arco (int orig, int dest, grafo *pG)
{
  return pG->MatriceAdiacenza[orig][dest];
}
