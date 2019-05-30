#ifndef __advio_h           /* Questo comando previene il caricamento della libreria più di una volta se chiamato più volte nel codice,
                               nella fase di precompiling, si comporta da "if", con condizione "not (yet) defined" */

#define __advio_h           /* Qui la definisco per poter triggerare il comando sopra al secondo passaggio */

#define SPAZIO    ' '
#define A_CAPO    '\n'

/* Stampa a video la stringa di caratteri "s" */
void StampaStringa (char *s);

/* Legge da tastiera la stringa di caratteri "s" */
void LeggeRiga (char *s);

/* Legge la prima parola inserita da tastiera e la scrive nella stringa "s" */
void LeggeParola (char *s);

/* Stampa a video il carattere "c" ripetuto "num" volte */
void StampaCarattere (char c, int num);

/* Legge il primo carattere inserito da tastiera */
char LeggeCarattere ();

/* Va a capo */
void ACapo ();

/* Stampa il numero intero "n" */
void StampaIntero (int n);

/* Legge da tastiera un numero intero */
int LeggeIntero ();

/* Stampa il numero naturale "n" */
void StampaNaturale (unsigned int n);

/* Legge da tastiera un numero naturale */
unsigned int LeggeNaturale ();

/* Stampa il numero long "n" */
void StampaLong (long n);

/* Legge da tastiera un numero intero di tipo long */
long LeggeLong ();

/* Stampa il numero "f", reale di tipo float */
void StampaFloat (float f);

/* Legge da tastiera un numero reale di tipo float */
float LeggeFloat ();

/* Stampa il numero "d", reale di tipo double */
void StampaDouble (double d);

/* Legge da tastiera un numero reale di tipo double */
double LeggeDouble ();

/* Stampa l'indirizzo "i" */
void StampaIndirizzo (void *i);

/* Stampa la cifra in denaro rappresentata dal numero reale "f" */
void StampaCifraDenaro (float f);

/* Legge da tastiera una cifra in denaro e la converte in un numero reale di tipo float */
float LeggeCifraDenaro ();

/* Converte una stringa numerica in un numero intero (in caso di errore, termina il programma) */
int ConverteStringaInIntero (char *s);

/* Determina il numero di caratteri di una stringa data */
int LunghezzaStringa (char *s);

#endif
