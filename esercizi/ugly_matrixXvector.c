#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void matXvet_local(MPI_Comm *grigliar, int menum, int nproc, int righe_loc, int colonne, int A_loc[][colonne], int *b){
        int *y = (int *)malloc(sizeof(int)*righe_loc/colonne);

        for(int i=0;i<righe_loc;i++){
                y[i]=0;
                for(int j=0; j<colonne; j++){
                        y[i] = y[i] + A_loc[i][j]*b[j];
                }
        }

        for(int i=0;i<righe_loc;i++){
                printf("Sono %d, ho prodotto y[%d]: %d\n", menum, i, y[i]);
        }
}

void crea_griglia(MPI_Comm *grigliar, int menum, int nproc, int righe, int colonne){
    int dim, *ndim, reorder, *period, vc[2];
    dim = 2;
    ndim = (int*) calloc (dim, sizeof(int));
    ndim[0] = 2; // o un qualundue numero > 1
    ndim[1] = 1;
    period = (int*) calloc (dim, sizeof(int));
    period[0] = period [1] = 0;
    // la periodicità non è necessaria
    reorder = 0;

    // creazione griglia
     MPI_Cart_create(MPI_COMM_WORLD,dim,ndim,period,reorder,grigliar);

     return;

}


int main(int argc, char*argv[]){

    int menum, nproc;
    int righe_loc=0;
    int colonne=0, righe=0;
    int coord[2];
    int matrix[2][2];

    int b[2] = {2,2};

    MPI_Status status;
    MPI_Comm grigliar;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &menum);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    if (menum == 0){
            colonne=2;
            righe=2;
            printf("\n");
            matrix[0][0] = 1;
            matrix[0][1] = 1;
            matrix[1][1] = 1;
            matrix[1][0] = 1;
    }


    /*CREAZIONE DELLA GRIGLIA DI PROCESSORI*/
    crea_griglia(&grigliar,menum,nproc,righe,colonne);

    /*Distribuzione Dati*/
    //invio dimensioni matrice con Bcast su grigliar
    MPI_Bcast(&colonne, 1, MPI_INT, 0, grigliar);
    MPI_Bcast(&righe, 1, MPI_INT, 0, grigliar);


    printf("Sono %d e ho colonne: %d e righe: %d", menum, colonne, righe);
    //calcolo dimensioni locali
    // invio blocchi matrice

    righe_loc = righe/nproc;
    if ((righe%nproc) != 0){
            MPI_Finalize();
            return 1;
    }

    int m[righe_loc][colonne];

    // invio blocchi matrice

    for(int p=1;p<nproc;p++){
            int tag = p+10;
            if ( menum == 0){
                    MPI_Send(&(matrix[p][0]), colonne, MPI_INT, p, tag, grigliar  );
            }else if (menum == p){
                    MPI_Recv(&(m[0][0]), colonne, MPI_INT, 0, tag, grigliar, &status);
            }
    }

    if (menum == 0){
            for(int i=0;i<colonne;i++)
                    m[0][i] = matrix[0][i];
    }

    //Calcolo locale
    matXvet_local(&grigliar, menum, nproc, righe_loc, colonne, m, &b);


    MPI_Finalize();
    return 0;


}
