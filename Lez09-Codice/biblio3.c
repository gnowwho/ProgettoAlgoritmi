/* biblio.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listalibri.h"


/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_libri, char *file_prestiti);

listalibri CaricaLibri (char *file_libri);

/* Esegue i movimenti riportati nel file dei prestiti e dei resi */
void EsegueMovimenti (char *file_prestiti, listalibri *Scaffale,
                      listalibri *Prestiti, listalibri *Resi);

/* Trova un libro in una lista */
posizione TrovaLibro (libro AutoreTitolo, listalibri L);

/* Stampa l'elenco dei libri disponibili */
void StampaLibri (listalibri Scaffale);

/* Stampa le istruzioni per riporre i libri resi */
void StampaIstruzioniRiordino (listalibri *Resi, listalibri *Scaffale);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char file_libri[ROW_LENGTH];
  char file_prestiti[ROW_LENGTH];
  listalibri Scaffale;
  listalibri Prestiti;
  listalibri Resi;

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_libri,file_prestiti);

  /* Carica l'elenco dei libri disponibili */
  Scaffale = CaricaLibri(file_libri);

  /* Crea le liste (vuote) dei libri prestati e resi */
  Prestiti = crealistalibri();
  Resi = crealistalibri();

  /* Esegue i movimenti riportati nel file dei prestiti e dei resi */
  EsegueMovimenti(file_prestiti,&Scaffale,&Prestiti,&Resi);

  /* Stampa l'elenco dei libri disponibili */
  StampaLibri(Scaffale);

  /* Stampa le istruzioni per riporre i libri resi */
  StampaIstruzioniRiordino(&Resi,&Scaffale);

  /* Stampa l'elenco dei libri disponibili */
  StampaLibri(Scaffale);

  /* Dealloca le strutture dinamiche */
  distruggelista(&Scaffale);
  distruggelista(&Prestiti);
  distruggelista(&Resi);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_libri, char *file_prestiti)
{
  if (argc != 3)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    exit(EXIT_FAILURE);
  }

  strcpy(file_libri,argv[1]);
  strcpy(file_prestiti,argv[2]);
}


  /* Carica l'elenco dei libri disponibili */
listalibri CaricaLibri (char *file_libri)
{
  FILE *fp;
  listalibri Scaffale;
  char Riga[ROW_LENGTH];
  char s[ROW_LENGTH];
  boolean fine;
  posizione p;


  fp = fopen(file_libri,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"Errore nell'apertura del file %s!\n",file_libri);
    exit(EXIT_FAILURE);
  }

  Scaffale = crealistalibri();

  do {
    /* Legge una riga dal file */
    fgets(Riga,ROW_LENGTH,fp);

    /* Legge la prima parola della riga */
    sscanf(Riga,"%s",s);

    /* Se s non e' il segnale di termine, Riga e' un nuovo libro */
    fine = (strcmp(s,"FINE") == 0);
    if (fine == FALSE)
    {
      /* Cancella l'a capo finale */
      Riga[strlen(Riga)-1] = '\0';

      /* Inserisce il libro in coda, dopo l'ultimo (cioe' prima del successore dell'ultimo) */
      p = succlista(Scaffale,ultimolista(Scaffale));
      inslista(Scaffale,p,Riga);
    }

  } while (fine == FALSE);

  fclose(fp);

  return Scaffale;
}

/* Esegue i movimenti riportati nel file dei prestiti e dei resi */
void EsegueMovimenti (char *file_prestiti, listalibri *pScaffale,
                      listalibri *pPrestiti, listalibri *pResi)
{
  FILE *fp;
  char Comando[ROW_LENGTH], Riga[ROW_LENGTH];
  posizione p;


  fp = fopen(file_prestiti,"r");
  if (fp == NULL)
  {
    printf("Errore nell'apertura del file %s!\n",file_prestiti);
    exit(EXIT_FAILURE);
  }

  /* Lo spazio dopo %s e' essenziale per cancellare gli spazi dopo il comando! */
  fscanf(fp,"%s ",Comando);
  while (strcmp(Comando,"FINE") != 0)
  {
    fgets(Riga,ROW_LENGTH,fp);

    /* Cancella l'a capo finale */
    Riga[strlen(Riga)-1] = '\0';

    if (strcmp(Comando,"PRESTITO") == 0)
    {
      /* Cerca il libro sullo scaffale e lo cancella */
      p = TrovaLibro(Riga,*pScaffale);
      if (p != NO_ELEMENT) *pScaffale = canclista(*pScaffale,&p);

      /* Inserisce il nuovo libro in coda alla lista dei prestiti */
      p = succlista(*pPrestiti,ultimolista(*pPrestiti));
      *pPrestiti = inslista(*pPrestiti,p,Riga);
    }
    else if (strcmp(Comando,"RESTITUZIONE") == 0)
    {
      /* Cerca il libro fra i prestiti e lo cancella */
      p = TrovaLibro(Riga,*pPrestiti);
      if (p != NO_ELEMENT) canclista(*pPrestiti,&p);

      /* Inserisce il nuovo libro in cima alla lista dei resi */
      p = primolista(*pResi);
      inslista(*pResi,p,Riga);
    }

    fscanf(fp,"%s ",Comando);
  }

  fclose(fp);
}

/* Trova un libro in una lista */
posizione TrovaLibro (libro AutoreTitolo, listalibri L)
{
  posizione p;
  libro s;

  for (p = primolista(L); !finelista(L,p); p = succlista(L,p))
  {
    leggelista(L,p,s);
    if (strcmp(s,AutoreTitolo) == 0) return p;
  }

  return NO_ELEMENT;
}


/* Stampa l'elenco dei libri disponibili */
void StampaLibri (listalibri L)
{
  posizione p;
  libro AutoreTitolo;

  for (p = primolista(L); !finelista(L,p); p = succlista(L,p) )
  {
    leggelista(L,p,AutoreTitolo);
    printf("%s\n",AutoreTitolo);
  }
  printf("FINE\n");
}


/* Stampa le istruzioni per riporre i libri resi */
void StampaIstruzioniRiordino (listalibri *pResi, listalibri *pScaffale)
{
}
