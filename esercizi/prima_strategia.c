#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

//Invio e ricezione di uno scalare tra due processori
int main(int argc, char *argv[]) {

	int numero_processori_massimo = 4;
	int menum, nproc;
	int n = 8;
	int *a; 
	int tag, num;

	int imov = n/numero_processori_massimo;
	int sumparz = 0, sumtot=0;
	
	// popolare l'array
	a = malloc( n * sizeof(int)); //array of two elemnts
	for (int i=0; i<n; i++){
		a[i] = i+1;
	}

	//inizializzazione

	MPI_Status info;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &menum);

	//inviare gli elementi da sommare agli altri processori che fanno la somma

	if (menum == 0) {

		for (int i=1; i<numero_processori_massimo; i++){
			tag = 10+i;
			MPI_Send(&a[i*imov],imov,MPI_INT,i,tag,MPI_COMM_WORLD);
		}
		printf("Sono %d e ho finito di inviare\n",menum);
		sumparz = a[menum*2] + a[menum*2 + 1];
		printf("Sono %d e ho ricevuto l'array, sommo: %d\n", menum, sumparz); 
	}else {
		tag = 10+menum;
		MPI_Recv(&a[menum*imov],imov,MPI_INT,0,tag,MPI_COMM_WORLD,&info);
		sumparz = a[menum*imov] + a[menum*imov+1]; 
		printf("Sono %d e ho ricevuto l'array, sommo: %d\n", menum, sumparz); 
	}

	int *aparz =  malloc( n/numero_processori_massimo * sizeof(int));
	

	// Spedisco gli elementi al processore 1
	if (menum != 0){
		tag = 20+menum;
		MPI_Send(&sumparz, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
	}else { //proc 0
		aparz[0] = sumparz;
		for (int i=1; i<numero_processori_massimo; i++){
			tag = 20+i;
			MPI_Recv(&aparz[i], i, MPI_INT, i, tag, MPI_COMM_WORLD, &info);
		}

		//effettuo la somma totale
		for (int i=0; i<numero_processori_massimo; i++){
			sumtot += aparz[i];
		}

		sleep(1);
		printf("\nSomma totale: %d\n", sumtot);
	}


	MPI_Get_count(&info, MPI_INT, &num);
	MPI_Finalize();
	return 0;
}
