#ifndef __lista_h
#define __lista_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_LENGTH 256

#define boolean int
#define TRUE  1
#define FALSE 0


typedef char libro[ROW_LENGTH];

#define NO_ELEMENT NULL
#define NO_LIST    NULL

typedef struct _elemento elemento;
typedef elemento *listalibri;
typedef elemento *posizione;

struct _elemento {
  libro AutoreTitolo;
  posizione succ;
  posizione pred;
};

listalibri crealistalibri ();

void distruggelista (listalibri *L);

void leggelista (listalibri L, posizione p, libro TitoloAutore);

listalibri scrivelista (listalibri L, posizione p, libro TitoloAutore);

boolean listavuota (listalibri L);

posizione primolista (listalibri L);

posizione ultimolista (listalibri L);

posizione succlista (listalibri L, posizione p);

posizione predlista (listalibri L, posizione p);

boolean finelista (listalibri L, posizione p);

listalibri inslista (listalibri L, posizione p, libro TitoloAutore);

listalibri canclista (listalibri L, posizione *pp);

#endif
