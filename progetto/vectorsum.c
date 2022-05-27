#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "util.h"


double vectorsum_seq(float **c, float**a, float**b, int alpha, long int n){
	double ec=0.0, sc=0.0;

	sc = omp_get_wtime();
	for (int i=0; i<n; i++){
		(*c)[i] = (*a)[i] * alpha + (*b)[i];
	}
	ec = omp_get_wtime();

	return ec-sc;
}


double vectorsum(float **c, float**a, float**b, int alpha, long int n , int nproc, int r){
	omp_set_num_threads(nproc); //setta il numero di processori al programma
	int i=0;
	int nloc=0;
	int step=0;
	int tnum=0;
	int imov=0;
	double ec = 0.0,sc = 0.0;

	sc = omp_get_wtime();

	#pragma omp parallel private(tnum,step,i, imov, nloc)
	{
		nloc=n/nproc;
		tnum = omp_get_thread_num();
		if ( tnum < r){
			nloc++;
			step=0;
		}else{
			step=r;
		}
		imov = tnum*nloc + step;
		for (i=imov; i<imov+nloc; i++){ 
			(*c)[i] =  (*a)[i] * alpha + (*b)[i] ;
		}
	}
	ec = omp_get_wtime();

	return ec-sc;
}

int usage(){
	printf("Uso: inserisci due interi: prima il numero di processori e poi alpha\n");
	return 1;
}

//In input il main prende il numero di processori e alpha
int main(int argc, char* argv[]){
	float *a, *b, *c;
	long int n=0;
	int alpha=0;
	int nproc=0,r=0;
	double time=0.0;


	//Controlla l'input
	if ( argc < 3){
		return usage();
	}

	//strtol converte le stringhe prese in input in interi 
	nproc = (int) strtol(argv[1], NULL, 10);
	alpha = (int) strtol(argv[2], NULL, 10);

	if ( nproc == 0 || alpha == 0){
		fprintf(stderr,"Errore nell'inserimento in input\n");
		return usage();
	}

	read_file(&a, &b, &n ); //vedi util.h

	c = (float *) malloc(n*sizeof(float)); //alloco il vettore C

	r = n%nproc; //calcolo

	if (nproc == 1){
		time = vectorsum_seq(&c,&a,&b,alpha,n);
	}else{
		time = vectorsum(&c,&a,&b,alpha,n,nproc,r);
	}


	//printf("Finito in: %f secondi\n",time);

	if ( write_timings(time) == 1 ){
		exit(1);
	}

	//for(int i=0;i<n; i++) printf("c[%d]: %f\n",i,c[i]);


	exit(0);

}
