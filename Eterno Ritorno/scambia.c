#include<stdio.h>
#include<stdlib.h>

//scambia due interi passandone i puntatori

void scambia(int *pa,int *pb){
  int i;
  i=*pa;
  *pa=*pb;
  *pb=i;
}

//main
/*int main( int argc, int *argv[]){
  int a,b;

a=1;
b=2;

printf("%d\t%d\n",a,b);
scambia(&a,&b);
printf("%d\t%d\n",a,b);

return 0;
}*/
