#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include "util.h"
#include <time.h>

#define ISZE sizeof(int)

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

        int r=0; //rest

        clock_t tutto_il_programma;
        clock_t seconda_strategia;
        clock_t seconda_strategia_reduce;

        tutto_il_programma = clock(); //Inzio tempo tutto il programma

        //inizializzazione MPI

        MPI_Status info;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &menum); //Mette in menum l'id del processore

        //Mette in nproc il numero di unità computazionali
        //con cui viene eseguito il codice
        MPI_Comm_size(MPI_COMM_WORLD, &nproc);

        logproc = log2n(nproc);
  
        //Popolamento array
  
        if ( menum == 0){

                //Lettura da file
                if ( read_config(&a, &n) == 1){ //vedi util.h
                        n=1;
                        nproc=0;
                }

                imov = n/nproc; //calcolo elementi che ogni processore deve sommare
                r = n%nproc; //calcolo del resto

                //Spedizione in sequenziale
                for(int i=1; i<nproc; i++){
                        tag=10+i;
                        MPI_Send(&n, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
                }

        }else{

                tag=10+menum;
                MPI_Recv(&n,1,MPI_INT,0,tag,MPI_COMM_WORLD,&info);

                if ( n == 1){
                        exit(1);
                }

                imov = n/nproc;
                r = n%nproc;

                //se n non è divisible per nproc, alloca più spazio
                size_t size = ( menum < r ) ? (imov+r)*ISZE : imov*ISZE ;

                arecv = (int *)malloc(size); //allocazione array elementi da sommare

        }
  
  
        //Invio degli elementi da sommare agli altri processori 

        if (menum == 0) { //processore 0 spedisce

                for (int i=1; i<nproc; i++){
                        tag = 10+i; //10 + id processore  che riceve
                        roffs = (i < r ) ? r : 0; //se i < r, roffs = r else roffs = 0

                        MPI_Send(&a[i*imov],imov+roffs,MPI_INT,i,tag,MPI_COMM_WORLD);

                }

        }else { //gli altri ricevono
                tag = 10+menum; //10 + id del processore che riceve
                MPI_Recv(arecv,imov+r,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
        }


        if (menum < r) imov++; //se menum < r allora ci sono più elementi nell'array da sommare

        for (int k=0; k<imov; k++){
                sumparz += (menum == 0) ? a[k] : arecv[k];
        }


        //Invio somme parziali, seconda strategia
        
        //Inizio tempo seconda strategia
        seconda_strategia = clock();

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
        
        //Fine tempo seconda strategia
        seconda_strategia = clock() - seconda_strategia;
  
        if (menum == 0){
                sumtot = sumparz;
                printf("\nSomma totale: %d\n", sumtot);
        }

        // Confronto con MPI_Reduce
        if (menum < r) imov++; //se menum < r allora ci sono più elementi nell'array da sommare

        for (int k=0; k<imov; k++){
                sumparz += (menum == 0) ? a[k] : arecv[k];
        }
        
        //Inizio tempo seconda strategia con reduce
        seconda_strategia_reduce = clock();

        MPI_Reduce(&sumtot,&sumparz,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD); //Seconda strategia con MPI_Reduce
        
        //Fine tempo seconda strategia con reduce
        seconda_strategia_reduce = clock() - seconda_strategia_reduce;


        if (menum == 0){
                printf("\nSomma totale con MPI_Reduce: %d\n", sumtot);
        }

        //se P0, libera la memoria di a, altrimenti libera la memoria di arecv
        (menum == 0) ? free(a) : free (arecv);

         //Fine tempo tutto il programma
        tutto_il_programma = clock() - tutto_il_programma;

        //Conversione dei clock in secondi
        double tempo1 = ((double)tutto_il_programma) / CLOCKS_PER_SEC;
        double tempo2 = ((double)seconda_strategia) / CLOCKS_PER_SEC;
        double tempo3 = ((double)seconda_strategia_reduce) / CLOCKS_PER_SEC;


        MPI_Finalize();

        if (menum == 0){
                printf("Tempo totale: %f \nTempo seconda strategia: %f\nTempo seconda strategia con reduce: %f\n\n", tempo1,tempo2, tempo3);
                write_timings(tempo1,tempo2,tempo3); //Scrivi i tempi su file
        }

        return 0;
}




