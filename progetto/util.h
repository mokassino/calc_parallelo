#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_INT (int)sizeof(int)

//Legge l'input da un file di configurazione
//La prima riga deve essere n=%d
//Le righe successive sono gli elementi nell'array
int read_file(float **a, float **b, long int *n){
	int k=0,j=0;
	char *buf;
	size_t len = 0;
	long int nwrap=0;

	//Lettura da file
	const char* filename_a = "vector_a"; //in a c'è anche n=SIZE_PROBLEMA
	const char* filename_b = "vector_b";

	FILE* vector_a = fopen(filename_a, "r");
	FILE* vector_b = fopen(filename_b, "r");

	if (!vector_a || !vector_b){
		return 1;
	}

	//Lettura del vettore A

	if ( getline(&buf, &len, vector_a) == 0){
		return 1;
    }

	sscanf(buf, "n=%ld", n); //Leggi il size N del file
	nwrap = *n;

	*a = (float *)malloc(nwrap * sizeof(float)); //allocazione dinamica della memoria
	*b = (float *)malloc(nwrap * sizeof(float)); //allocazione dinamica della memoria

	while ( getline(&buf, &len, vector_a) != -1 && k < nwrap){
		(*a)[k] = atoi(buf);
		//getline(&buf, &len, vector_b);
		(*b)[k++] = atoi(buf);
    }

	return 0;
}

int write_timings(double time){
	
	const char* filename = "timings"; //nome del file
	FILE* timings = fopen(filename,"a"); //apertura file in modalità "append"

	if (!timings) return 1; //non si è riusciti ad aprire il file, ritorna

	fprintf(timings, "%f\n", time);

	fclose(timings);

	return 0;
}
