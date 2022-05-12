#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "util.h"

int usage(){
    printf("Inserisci un intero\n");
    return 1;
}

//In input il main prende un intero
int main(int argc, char* argv[]){
    int *a, *b, *c;
    int n=0,alpha=0;
    int i=0;


    //Controlla l'input
    if ( argc < 2){
        return usage();
    }

    //atoi converte la stringa presa in input in intero e lo assegna ad alpha
    alpha = atoi(argv[1]); 


    read_file(&a, &b, &n ); //vedi util.h

    c = (int *) malloc(n*sizeof(int));

    

    #pragma omp parallel for private(i) schedule(dynamic) num_threads(8)
    for (i=0; i<n; i++){
        c[i] = (a[i]*alpha)+b[i];
        printf("%d + %d = %d\n", a[i]*alpha, b[i], c[i]);

    }

    printf("\n");

}
