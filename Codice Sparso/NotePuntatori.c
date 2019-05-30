#include<stdlib.h>

int main(int argc, char *argc[]){
int V[10];
int *p;

p=V;    //ho conversione implicita da "array di interi" a puntatore alla prima cella dell'array (int*)
//V=p;    questa cosa non è lecita: V NON è un puntatore a intero


//voglio lavorare solo sugli ultimi elementi del vettore:
p=&V[7];
//tratto p come un vettore di tre interi, indiciati 0,1 e 2 invece che 7, 8 e 9
//allo stesso modo potrei usare qualsiasi altro indirizzo di memoria per avere qualsiasi altro indice: posso usare anche gl indici negativi
//con l'assegnazione qui sopra p[-1] ha senso ed è V[6]

p=(int*) malloc(10*sizeof(int));
//questo vettore è allocato dinamicamente, e può variare la propria dimensione liberamente
V[2]=1; //questa istruzione viene eseguita su un vettore allocato nello STACK, e salva il valore 1 nella seconda casella dell'array V
p[2]=1; //questa istruzione viene eseguita su un vettore allocanto nell'HEAP, punta alla cella salvata in p, più due volte sizeof(int),
        //e salva 1 in quella casella. OSSERVA che p sta nello STACK.

//La memoria allocata dinamicamente deve essere DEALLOCATA in modo esplicito.
free(p);    //dealloca tutte le celle di memoria allocate con malloc, puntate da p: è importante non perderle per poterle deallocare.


return 0;
}
