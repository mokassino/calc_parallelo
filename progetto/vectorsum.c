#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "util.h"

int usage(){
    printf("Inserisci un intero");
    return 1;
}

//In input il main prende, alpha
int main(int argc, char*argv){
    int *a, int*b;
    int n=0,alpha=0;


    //Controlla l'input
    if ( argc < 2){
        return usage();
    }


    read_file(&a, &b, n );

}
