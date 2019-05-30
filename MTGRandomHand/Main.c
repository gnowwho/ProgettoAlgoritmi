#include<stdio.h>
#include<stdlib.h>

/*dichiarazione procedure{*/
void CheckCLInput (int argc, char *argv[]);
/*}*/


int main (int argc, char *argv[])
{


  return EXIT_SUCCESS;
}


void CheckCLInput (int argc, char *argv[])
{
  if (argc != 2){
     if (argc==1){
        fprintf(stderr, "File elenco carte mancante\n");
     }
     else{
        fprintf(stderr,"Errore nella linea di comando!\n");
        exit(EXIT_FAILURE);
  }  
}

  strcpy(filemosse,argv[1]);
}
