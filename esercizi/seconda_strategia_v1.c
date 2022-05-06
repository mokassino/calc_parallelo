#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include "util.h"

int main(int argc, char *argv[]) {

        int menum; //id del processore
        int nproc; //numero di processori
        int n=0  ; //dimensione del problema della somma
        int tag; //tag usato dalla libreria MPI per le spedizioni

        int imov;  //spiazzamento dell'indice per accedere agli elementi dell'array
        int roffs; //rest offset, spiazzamento dell'indice in caso n non sia divisibile per nproc

        int sumparz = 0, sumtot=0; // variabili dove conservare le somme parziai
        int logproc; //variabile usata per le spedizioni per la seconda strategia della somma
        int recv_parz = 0; //variabile temporanea dove tenere le somme parziali

        int *a, *arecv;  //array 

        int r=0; //resto


        //inizializzazione MPI

        MPI_Status info;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &menum); //Mette in menum l'id del processore

        //Mette in nproc il numero di unità computazionali
        //con cui viene eseguito il codice
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);

        logproc = log2n(nproc);


        //Popolamento array e spedizione della complessità computazionale
        if ( menum == 0){

                //Lettura da file di size ed elementi dell'array
                if ( read_config(&a, &n) == 1){ //vedi util.h per read_config
                        n=1;
                        nproc=0;
                }

                imov = n/nproc; //calcolo elementi che ogni processore deve sommare
                r = n%nproc; //calcolo del resto
                
                // P0 spedisce a tutti gli altri in sequenziale il numero di elementi totale da sommare
                for(int i=1; i<nproc; i++){
                        tag=10+i;
                        MPI_Send(&n, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
                }

        }else{

                tag=10+menum;
                //Gli altri processori ricevono il valore di N
                MPI_Recv(&n,1,MPI_INT,0,tag,MPI_COMM_WORLD,&info);

                imov = n/nproc;
                r = n%nproc;

                if ( r != 0 && menum < r){
                        arecv = malloc( (imov + r) * sizeof(int) );
                }else{
                        arecv = malloc( (imov) * sizeof(int) );
                }

        }
        
        if ( n == 1){ //Non si è riusciti a leggere il file
                exit(1);
        }

        //Invio degli elementi da sommare agli altri processori 
        if (menum == 0) { //processore 0 spedisce

                for (int i=1; i<nproc; i++){
                        tag = 10+i; //10 + id processore  che riceve
                              
                        if ( i < r ){
                                MPI_Send(&a[i*imov],imov+r,MPI_INT,i,tag,MPI_COMM_WORLD);
                        }else{
                                MPI_Send(&a[i*imov],imov,MPI_INT,i,tag,MPI_COMM_WORLD);
                        }

                }

        }else { //gli altri ricevono
                tag = 10+menum; //10 + id del processore che riceve
           
                if ( menum < r ){
                        MPI_Recv(arecv,imov+r,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
                }else{
                        MPI_Recv(arecv,imov,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
                }
        }


        if (menum < r) imov++; //Deve sommare un elemento in più

        for (int k=0; k<imov; k++){
                if (menum == 0) //Il processore 0 non utilizza arecv ma l'array che ha letto da file
                   sumparz += a[k];
                else
                   sumparz += arecv[k];
                   
        }
        
        //Invio, seconda strategia: ad ogni passo i processori inviano le somme parziali 
        //a quelli adiacenti che contengono le somme parziali, fino ad arrivare a P0

        for(int i=0; i<logproc; i++){

                int p1 = myPow(2,i);
                if ( (menum%p1) ==0 ) {
                
                        int p = myPow(2,i+1);
                        
                        if ( (menum%p) == 0 ){ //riceve
                                tag = p; //il tag funziona perché i processori a cui si invia sono diversi
                                MPI_Recv(&recv_parz, 1, MPI_INT, menum+p1, tag, MPI_COMM_WORLD, &info);
                                sumparz += recv_parz;
                        }else{ //spedisce
                                tag = p;
                                MPI_Send(&sumparz, 1, MPI_INT, menum-p1, tag, MPI_COMM_WORLD);

                        }
                }
        }


        if (menum == 0){ //Il processore 0 stampa la somma totale
                sumtot = sumparz;
                printf("\nSomma totale: %d\n", sumtot);
        }


        //se P0, libera la memoria di a, altrimenti libera la memoria di arecv
        (menum == 0) ? free(a) : free (arecv);

        MPI_Finalize();
        return 0;
}

