#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include "util.h"

//Invio e ricezione di uno scalare tra due processori
int main(int argc, char *argv[]) {

        int menum; //id del processore
        int nproc; //numero di processori
        int n = 32; //dimensione del problema della somma
        int tag; //tag usato dalla libreria MPI per le spedizioni

        int imov;  //spiazzamento dell'indice per accedere agli elementi dell'array

        int sumparz = 0, sumtot=0; // variabili dove conservare le somme parziai
        int logproc; //variabile usata per le spedizioni per la seconda strategia della somma
        int recv_parz = 0; //variabile temporanea dove tenere le somme parziali

        int *a, *arecv;  //array 

        //inizializzazione MPI

        MPI_Status info;


        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &menum); //Mette in menum l'id del processore

        //Mette in nproc il numero di unità computazionali
        //con cui viene eseguito il codice
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);

        imov = n/nproc;
        logproc = log2n(nproc);


        //Popolamento array
               if ( menum == 0){

                a = malloc( n * sizeof(int)); // allocazione dinamica della memoria
                for (int i=0; i<n; i++){
                        a[i] = 1;
                }
        }else{

                arecv = malloc( (imov) * sizeof(int) );
        }


        //Invio degli elementi da sommare agli altri processori 

        if (menum == 0) { //processore 0 spedisce

                for (int i=1; i<nproc; i++){
                        tag = 10+i; //10 + id processore  che riceve
                        MPI_Send(&a[i*imov],imov,MPI_INT,i,tag,MPI_COMM_WORLD);
                }

        }else { //gli altri ricevono
                tag = 10+menum; //10 + id del processore che riceve
                //MPI_Recv(&a[menum*imov],imov,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
                MPI_Recv(arecv,imov,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
        }


        for (int k=0; k<imov; k++){
                sumparz += (menum == 0) ? a[k] : arecv[k];

                /*
                if (menum == 0)
                        sumparz += a[k];
                else
                        sumparz += arecv[k];
                */
        }
        
        //Invio, seconda strategia

        for(int i=0; i<logproc; i++){

                int p1 = myPow(2,i);
                if ( (menum%p1) ==0 ) {
                        int p = myPow(2,i+1);
                        if ( (menum%p) == 0 ){ //riceve
                                tag = p;
                                MPI_Recv(&recv_parz, 1, MPI_INT, menum+p1, tag, MPI_COMM_WORLD, &info);
                                sumparz += recv_parz;
                        }else{ //spedisce
                                tag = p;
                                MPI_Send(&sumparz, 1, MPI_INT, menum-p1, tag, MPI_COMM_WORLD);

                        }
                }
        }


        if (menum == 0){
                sumtot = sumparz;
                printf("\nSomma totale: %d\n", sumtot);
        }


        //se P0, libera la memoria di a, altrimenti libera la memoria di arecv
        (menum == 0) ? free(a) : free (arecv);

        MPI_Finalize();
        return 0;
}




