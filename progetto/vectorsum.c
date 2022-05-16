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
    long int n=0;
    int i=0, alpha=0;
    int nproc=0,k=0,r=0,step=0;

    double sc, ec, time; //start clock , end clock sono usate per prendere i tempi. 


    //Controlla l'input
    if ( argc < 2){
        return usage();
    }

    //atoi converte la stringa presa in input in intero e lo assegna ad alpha
    alpha = atoi(argv[1]); 


    read_file(&a, &b, &n ); //vedi util.h

    c = (int *) malloc(n*sizeof(int));

    nproc = 8;
    k = n/nproc;
    r = n%nproc;

    step = r != 0 ? 0 : r;

    
    sc = omp_get_wtime();
    #pragma omp parallel for private(i) schedule(static) num_threads(nproc)
    for (i=0; i<n; i++){
        c[i] = (a[i]*alpha)+b[i];

    }
    ec = omp_get_wtime();

    time = ec - sc;
    //printf("Finito in: %f secondi\n",time);
    if ( write_timings(time) == 1 ){
        exit(1);
    }

    exit(0);

}
