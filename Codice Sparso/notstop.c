#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//#define ROW_LENGHT 256

int notstop(char *s){
  int i;
  i=strcmp("STOP",s);
  return i*i;
}

//main
/*int main( int argc, int *argv[]){
  char s[ROW_LENGHT];

printf("scrivi\n");
scanf("%s",s);
printf("%d\n", notstop(s));

return 0;
}*/
