#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_INT (int)sizeof(int)

//Legge l'input da un file di configurazione
//La prima riga deve essere n=%d
//Le righe successive sono gli elementi nell'array
int read_config(int **a, int *n){
                int k=0,j=0;

                //Lettura da file
                const char* filename = "config";
                FILE* config = fopen(filename, "r");

                if (!config){
                        return 1;
                }

                char *buf;
                size_t len = 0;
                if ( getline(&buf, &len, config) == 0){
                        return 1;
                }

                printf("%s",buf);
                sscanf(buf, "n=%d", n);
                printf("Numero: %d\n",*n);

                int wrap = *n;
                *a = (int *)malloc(wrap * sizeof(int)); //allocazione dinamica della memoria

                //Popolamento dell'array con gli elementi nel vettore
                while ( getline(&buf, &len, config) != -1 && k < *n){
                        (*a)[k++] = atoi(buf);
                }

                return 0;

}

//Scrive 
int write_timings(double tt, double ts, double tsr){
	//tt = tempo totale, ts = tempo seconda strategia, 
	//tsr= tempo seconda strategia reduce
	
	const char* filename = "timings"; //nome del file
	FILE* timings = fopen(filename,"a"); //apertura file in modalità "append"

	if ( ftell(timings) == 0){
		fprintf(timings, "Tot Time\tTime 2nd s\tTime w reduce\n");
	}

	if (!timings) return 1; //non si è riusciti ad aprire il file, ritorna

	fprintf(timings, "%f\t%f\t%f\n", tt, ts, tsr );

	//Esempio di output nel file: 
	//Tot Time	Time 2nd s	Time w reduce
	//0.20321	0.00029		0.00011
	

	fclose(timings);

	return 0;
}


//Prende degli elementi in input e li alloca ad un array
void get_input(int **array, int *size){
	char buffer[1024];

	printf("Quanto grande deve essere il tuo array?\n");
	fgets(buffer,1024,stdin);
	*size = atoi(buffer);
	printf("Size inserito: %d\n", *size);
	memset(&buffer[0], 0, 1024);

	int s = *size;
	size_t to_alloc = s*SIZE_OF_INT;
	*array = (int *)malloc(to_alloc);

	printf("Inserisci i numeri che vanno dentro l'array\n");
	for(int i=0; i<s; i++){
		fgets(buffer,1024,stdin);
		(*array)[i] = atoi(buffer); 
		memset(&buffer[0],0,1024);
		printf("Inserito! Prossimo \n");
	}

}

//Calcola la potenza di un numero
//Workaround per pow() che non sembra funzionare con mpi
int myPow(int num, int exp){
	int res=0;
	if (exp == 0){
		return 1;
	}else{
		res = num;
	}

	int i=1;
	while((i++)<exp){
		res <<= 1;
	}

	return res;
}

//Calcola un logaritmo in base 2
unsigned int log2n(unsigned int n){
	unsigned int res=0;
	while (n >>= 1){
		res++;
	}

	return res;
}

