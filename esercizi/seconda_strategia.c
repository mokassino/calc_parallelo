//E' un abbozzo!

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include "util.h"

//Invio e ricezione di uno scalare tra due processori
int main(int argc, char *argv[]) {

        int menum, nproc=4;
        int n = 8;
        int *a;
        int tag, num;

        int imov = n/nproc;
        int sumparz = 0, sumtot=0;

        // popolare l'array
        a = malloc( n * sizeof(int)); //array of two elemnts
        for (int i=0; i<n; i++){
                a[i] = 1;
        }

        //inizializzazione

        MPI_Status info;

        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &menum);

        //inviare gli elementi da sommare agli altri processori che fanno la somma

        if (menum == 0) {

                for (int i=1; i<nproc; i++){
                        tag = 10+i;
                        MPI_Send(&a[i*imov],imov,MPI_INT,i,tag,MPI_COMM_WORLD);
                }
                sumparz = a[menum*2] + a[menum*2 + 1];
        }else {
                tag = 10+menum;
                MPI_Recv(&a[menum*imov],imov,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
                sumparz = a[menum*imov] + a[menum*imov+1];
        }

        free(a);
         int *aparz =  malloc( n / (n/nproc) * sizeof(int)); //array di somme parziali
        for (int i=0; i<n / (n/nproc); i++){
                aparz[i]=2;
        }

        sleep(1);

        //Invio, seconda strategia
        int logproc = 2;
        int k=1;
        tag = 100;
        int ssparz = 0;
        int sped_count_for = 0;

        for(int i=0; i<logproc; i++){

                int p1 = myPow(2,i);
                if ( (menum%p1) ==0 ) {
                        int p = myPow(2,i+1);
                        if ( (menum%p) == 0 ){ //riceve
                                tag = 100 +k;
                                MPI_Recv(&ssparz, 1, MPI_INT, menum+p-k, tag, MPI_COMM_WORLD, &info);
                                aparz[menum] += ssparz;
                        }else{ //spedisce
                                sped_count_for++;
                                tag = 100 + k;
                                MPI_Send(&aparz[menum], 1, MPI_INT, menum-p+k, tag, MPI_COMM_WORLD);

                        }
                }
                k++;
        }

        if (menum == 0){
                sleep(1);
                printf("\n\nSomma totale: %d\n", aparz[0]);
        }


        MPI_Get_count(&info, MPI_INT, &num);
        MPI_Finalize();
        return 0;
}

