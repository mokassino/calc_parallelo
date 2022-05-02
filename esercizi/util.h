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
		res = 1;
		return res;
	}else{
		res = num;
	}

	for (int i=1; i<exp; i++){
		res *= res;
	}

	return res;
}
