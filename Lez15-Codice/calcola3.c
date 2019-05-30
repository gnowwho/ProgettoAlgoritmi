/* calcola.c */

/* Direttive */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "albero.h"

#define ROW_LENGTH 256


/* Prototipi delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_espressioni);

/* Elabora le espressioni contenute nel file file_espressioni */
void ElaboraEspressioni (char *file_espressioni);

/* Carica l'espressione corrente dalla stringa nell'albero */
albero ConverteEspressione (char *Espressione);

/* Scompone Espressione nelle sue componenti s, s_op1 e s_op2 */
void ScomponeEspressione (char *Espressione, char *s, char *s_op1, char *s_op2);

int TrovaCarattereNonAnnidato (char ch, char *Espressione);

/* Identifica l'operatore rappresentato dalla stringa s */
Operatore IdentificaOperatore (char *s);

/* Calcola il valore dell'espressione rappresentata dall'albero */
double CalcolaValore (albero T);

/* Stampa in notazione infissa l'espressione rappresentata dall'albero */
void StampaEspressione (albero T);

/* Calcola la profondita' dell'albero */
int Profondita (albero T);


/* Programma principale */
int main (int argc, char *argv[])
{
  /* Parte dichiarativa */
  char file_espressioni[ROW_LENGTH];

  /* Parte esecutiva */
  InterpretaLineaComando(argc,argv,file_espressioni);

  /* Elabora le espressioni nel file */
  ElaboraEspressioni(file_espressioni);

  return EXIT_SUCCESS;
}


/* Definizione delle procedure secondarie */

void InterpretaLineaComando (int argc, char *argv[], char *file_espressioni)
{
  if (argc != 2)
  {
    fprintf(stderr,"Errore nella linea di comando!\n");
    fprintf(stderr,"%s [file_espressioni]\n",argv[0]);
    exit(EXIT_FAILURE);
  }

  strcpy(file_espressioni,argv[1]);
}


void ElaboraEspressioni (char *file_espressioni)
{
  FILE *fp;
  char Espressione[ROW_LENGTH];
  albero T;
  double v;
  int p;


  fp = fopen(file_espressioni,"r");
  if (fp == NULL)
  {
    fprintf(stderr,"Errore nell'apertura del file %s!\n",file_espressioni);
    exit(EXIT_FAILURE);
  }

  /* Legge il file una riga alla volta */
  while (fscanf(fp,"%[^\n]\n",Espressione) == 1)
  {
    /* Carica l'espressione corrente dalla stringa nell'albero */
    T = ConverteEspressione(Espressione);

    /* Calcola il valore dell'espressione rappresentata dall'albero */
    v = CalcolaValore(T);
    printf("valore     = %f\n",v);

    /* Stampa in notazione infissa l'espressione rappresentata dall'albero */
    StampaEspressione(T);
    printf("\n");

    /* Calcola la profondita' dell'albero */
    p = Profondita(T);
    printf("profondita = %d\n",p);

    /* Distrugge l'albero */
    distruggealbero(&T);
    printf("\n");
  }

  fclose(fp);
}


/* Carica l'espressione corrente dalla stringa nell'albero */
albero ConverteEspressione (char *Espressione)
{
  char s[ROW_LENGTH];
  char s_op1[ROW_LENGTH];
  char s_op2[ROW_LENGTH];
  Operando val;
  Operatore op;
  albero T, Ts, Td;


  /* Se l'espressione consiste in un numero reale, e' semplice */
  if (sscanf(Espressione,"%lf",&val) == 1)
    T = costruiscealbero(NO_OP,val,NO_TREE,NO_TREE);
  else
  /* Se invece comincia con il nome di un operatore, seguito da parentesi, e' composta */
  {
    /* Identifica le tre parti dell'espressione: operatore, operando1 e operando2 */
    ScomponeEspressione(Espressione,s,s_op1,s_op2);

    op = IdentificaOperatore(s);
    Ts = ConverteEspressione(s_op1);
    Td = ConverteEspressione(s_op2);
    T = costruiscealbero(op,NO_VAL,Ts,Td);
  }

  return T;
}


/* Scompone Espressione nelle sue componenti s, s_op1 e s_op2 */
void ScomponeEspressione (char *Espressione, char *s, char *s_op1, char *s_op2)
{
  int i;


  i = TrovaCarattereNonAnnidato('(',Espressione);
  strncpy(s,Espressione,i);
  s[i] = '\0';

  Espressione = &Espressione[i+1];
  i = TrovaCarattereNonAnnidato(',',Espressione);
  strncpy(s_op1,Espressione,i);
  s_op1[i] = '\0';

  Espressione = &Espressione[i+1];
  i = TrovaCarattereNonAnnidato(')',Espressione);
  strncpy(s_op2,Espressione,i);
  s_op2[i] = '\0';
}


int TrovaCarattereNonAnnidato (char ch, char *Espressione)
{
  int i, cont;

  cont = 0;
  for (i = 0; Espressione[i] != '\0'; i++)
  {
    if ( (Espressione[i] == ch) && (cont == 0) ) return i;
    if (Espressione[i] == '(') cont++;
    if (Espressione[i] == ')') cont--;
  }

  return i;
}


/* Identifica l'operatore rappresentato dalla stringa s */
Operatore IdentificaOperatore (char *s)
{
  if (strcmp(s,"somma") == 0)
    return '+';
  else if (strcmp(s,"diff") == 0)
    return '-';
  else if (strcmp(s,"prod") == 0)
    return '*';
  else if (strcmp(s,"div") == 0)
    return '/';
  else
  {
    fprintf(stderr,"Errore nell'identificazione dell'operatore %s!\n",s);
    exit(EXIT_FAILURE);
  }
}


/* Calcola il valore dell'espressione rappresentata dall'albero */
double CalcolaValore (albero T)
{
  return NO_VAL;
}


/* Stampa in notazione infissa l'espressione rappresentata dall'albero */
void StampaEspressione (albero T)
{
}


/* Calcola la profondita' dell'albero */
int Profondita (albero T)
{
  return 0;
}
