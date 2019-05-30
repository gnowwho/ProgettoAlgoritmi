#include<stdlib.h>
#include<stdio.h>

int main(){
    char T;

    while('T!=1'&&'T!=2'){
        printf("Scegli modalità:\nscrivi 1 per calcolare l'area noti i lati\nscrivi 2 per calcolare l'area nota la base e altezza\n");
        scanf("%c",T);
        
        switch (T)
        {
            case 1:
                /* code */
                break;
            case 2:

                break;
        
            default:
                printf("carattere non valido\n");
                break;
        }        
    }

    
    
        

}