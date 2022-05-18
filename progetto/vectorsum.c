#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "util.h"

int usage(){
    printf("Uso: inserisci due interi: prima il numero di processori e poi alpha\n");
    return 1;
}

//In input il main prende il numero di processori e alpha
int main(int argc, char* argv[]){
    int *a, *b, *c;
    long int n=0;
    int i=0, alpha=0,id=0;
    int nproc=0,nloc=0,r=0,step=0;

    double sc, ec, time; //start clock , end clock sono usate per prendere i tempi. 


    //Controlla l'input
    if ( argc < 3){
        return usage();
    }

    //strtol converte le stringhe prese in input in interi 
    nproc = (int) strtol(argv[1], NULL, 10);//atoi(argv[1]);
    alpha = (int) strtol(argv[2], NULL, 10);//atoi(argv[2]); 

    if ( nproc == 0 || alpha == 0){
	    fprintf(stderr,"Errore nell'inserimento in input\n");
	    return usage();
    }

    read_file(&a, &b, &n ); //vedi util.h

    c = (int *) malloc(n*sizeof(int));

    omp_set_num_threads(nproc); //setta il numero di processori al programma


    r = n%nproc;
    sc = omp_get_wtime();
    #pragma omp parallel private(id,step,i,nloc) shared(r)
    {
        nloc = n/nproc;
        id = omp_get_thread_num();
        if ( id < r){
            nloc++;
            step=0;
        }else{
            step=r;
        }

        for (i=id*nloc+step; i<id*nloc+nloc+step; i++){ 
            c[i] = (a[i]*alpha)+b[i];
        }
    }
    ec = omp_get_wtime();

    time = ec - sc;
    //printf("Finito in: %f secondi\n",time);

    if ( write_timings(time) == 1 ){
        exit(1);
    }

    exit(0);

}
