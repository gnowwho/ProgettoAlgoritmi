#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int checkmonth(char *s);
//mainfarlocco
/*int main(int argc, char *argv[]){
char memme[256];
int a;

printf("inserisci mese\n");
scanf("%s",memme);
a=checkmonth(memme);
printf("\n%d\n",a);
return 0;
}*/


//funzione
int checkmonth(char *s){
int m;

if (strcmp(s,"gennaio")==0)
  m=1;
else if (strcmp(s,"febbraio")==0)
  m=2;
else if (strcmp(s,"marzo")==0)
  m=3;
else if (strcmp(s,"aprile")==0)
  m=4;
else if (strcmp(s,"maggio")==0)
  m=5;
else if (strcmp(s,"giugno")==0)
  m=6;
else if (strcmp(s,"luglio")==0)
  m=7;
else if (strcmp(s,"agosto")==0)
  m=8;
else if (strcmp(s,"settembre")==0)
  m=9;
else if (strcmp(s,"ottobre")==0)
  m=10;
else if (strcmp(s,"novembre")==0)
  m=11;
else if (strcmp(s,"dicembre")==0)
  m=12;
else m=0;

return m;
}
