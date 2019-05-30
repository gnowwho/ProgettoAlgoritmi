//direttive preprocessore
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW_LENGHT 256

//prototipi sottofunzioni
int notstop(char *s);       //funzione ausiliaria che controlla se la stringa è uno STOP
int correctstr(char *s);    //verifica che la stringa rispetti la struttura richiesta
int checkmonth(char *s);

//main
int main(int argc, char *argv[]){
  //dichiarazione
  char articolo[ROW_LENGHT];  //ricorda che il nome dell'array è già un puntatore al primo elemento
  float prezzo;
  int giorno;
  char mese[10];
  int anno;

  int n,m;
  char Buff[ROW_LENGHT];

  //esecuzione
Buff[0]=0;

    printf("Inserisci dati in forma \"articolo\" \"prezzo\" \"giorno\" \"mese\" \"anno\"\n");
    fgets(Buff,ROW_LENGHT,stdin);
    n=sscanf(Buff,"%s %f %2d %s %4d",articolo,&prezzo,&giorno,mese,&anno);
    m=checkmonth(mese);
    printf("ho letto %d dati che sono\nArticolo: %s\nPrezzo: %.2f\nData: %d/%02d/%4d",n,articolo,prezzo,giorno,m,anno);


return EXIT_SUCCESS;
}
