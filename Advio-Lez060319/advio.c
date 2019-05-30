#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* Stampa a video la stringa di caratteri "s" */
void StampaStringa (char *s)
{
  printf("%s",s);
}


/* Legge da tastiera la stringa di caratteri "s" */
void LeggeRiga (char *s)
{
  gets(s);
}


/* Legge la prima parola inserita da tastiera e la scrive nella stringa "s" */
void LeggeParola (char *s)
{
  char riga[256];

  gets(s);
  sscanf(riga,"%s",s);
}


/* Stampa a video il carattere "c" ripetuto "num" volte */
void StampaCarattere (char c, int num)
{
  int i;

  for (i = 1; i <= num; i++)
    printf("%c",c);
}


/* Legge il primo carattere inserito da tastiera */
char LeggeCarattere ()
{
  char riga[256];

  gets(riga);
  return riga[0];
}


/* Va a capo */
void ACapo ()
{
  printf("\n");
}


/* Stampa il numero intero "n" */
void StampaIntero (int n)
{
  printf("%d",n);
}


/* Legge da tastiera un numero intero */
int LeggeIntero ()
{
  int res, i;

  res = scanf("%d",&i);
  if (res != 1)
  {
    fprintf(stderr,"Non si e' inserito un numero intero corretto!");
    exit(EXIT_FAILURE);
  }

  return i;
}


/* Stampa il numero naturale "n" */
void StampaNaturale (unsigned int n)
{
  printf("%u",n);
}


/* Legge da tastiera un numero naturale */
unsigned int LeggeNaturale ()
{
  int res;
  unsigned int i;

  res = scanf("%u",&i);
  if (res != 1)
  {
    fprintf(stderr,"Non si e' inserito un numero naturale corretto!");
    exit(EXIT_FAILURE);
  }

  return i;
}


/* Stampa il numero long "n" */
void StampaLong (long n)
{
  printf("%ld",n);
}


/* Legge da tastiera un numero intero di tipo long */
long LeggeLong ()
{
  int res;
  long i;

  res = scanf("%ld",&i);
  if (res != 1)
  {
    fprintf(stderr,"Non si e' inserito un numero intero long corretto!");
    exit(EXIT_FAILURE);
  }

  return i;
}


/* Stampa il numero "f", reale di tipo float */
void StampaFloat (float f)
{
  printf("%f",f);
}


/* Legge da tastiera un numero reale di tipo float */
float LeggeFloat ()
{
  int res;
  float f;

  res = scanf("%f",&f);
  if (res != 1)
  {
    fprintf(stderr,"Non si e' inserito un numero reale corretto!");
    exit(EXIT_FAILURE);
  }

  return f;
}


/* Stampa il numero "d", reale di tipo double */
void StampaDouble (double d)
{
  printf("%f",d);
}


/* Legge da tastiera un numero reale di tipo double */
double LeggeDouble ()
{
  int res;
  double d;

  res = scanf("%lf",&d);
  if (res != 1)
  {
    fprintf(stderr,"Non si e' inserito un numero reale double corretto!");
    exit(EXIT_FAILURE);
  }

  return d;
}


/* Stampa l'indirizzo "i" */
void StampaIndirizzo (void *i)
{
  printf("%p",i);
}


/* Stampa la cifra in denaro rappresentata dal numero reale "f" */
void StampaCifraDenaro (float f)
{
  printf("%.2f euro",f);
}


/* Legge da tastiera una cifra in denaro e la converte in un numero reale di tipo float */
float LeggeCifraDenaro ()
{
  float f;

  if (scanf("%f",&f) != 1)
  {
    fprintf(stderr,"Non si e' inserita una cifra in denaro corretta!");
    exit(EXIT_FAILURE);
  }

  return f;
}


/* Converte una stringa numerica in un numero intero (in caso di errore, termina il programma) */
int ConverteStringaInIntero (char *s)
{
  int n;

  if (sscanf(s,"%d",&n) != 1)
  {
    StampaStringa("Il parametro non e' un numero intero!\n");
    exit(EXIT_FAILURE);
  }

  return n;
}


/* Determina il numero di caratteri di una stringa data */
int LunghezzaStringa (char *s)
{
  return strlen(s);
}
