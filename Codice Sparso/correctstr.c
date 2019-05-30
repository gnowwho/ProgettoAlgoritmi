#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ROW_LENGHT 256

int correctstr(char *in,char *articolo,float *prezzo,int *giorno,int *m,int *anno);

int main( int argc, int *argv[]){
  char s[ROW_LENGHT];

printf("scrivi\n");
scanf("%s",s);
printf("%d\n", notstop(s));

return 0;
}

int correctstr(char *in,char *articolo,float *prezzo,int *giorno,int *m,int *anno){
//dichiarazione
int n;
char mese[ROW_LENGHT];
//esecuzione
  n=sscanf(Buff,"%s %f %2d %s %4d",articolo,prezzo,giorno,mese,anno);
    if (n!=5){
        return 0;
    }

  *prezzo=roundf(*prezzo*100)/100;  //il prezzo è automaticamente arrotondato a due decimali

  n=checkmonth(mese);
    if (n==0){
      return 0;
    }
  *m=n;

  if (n==2){
    if (*anno%4==0)
  }

}
