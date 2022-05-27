#include <stdio.h>
#include <stdlib.h>

int usage(){
    printf("Inserisci un numero intero che rappresenta dimensione del problema N in input\nEsempio: ./file_populator.py 1000");
    return 1;
}

int main(int argc, char *argv[]){

    int size=0;
    if (argc < 2){
        return usage();
    }

    size = atoi(argv[1]);

    FILE* fa = fopen("vector_a","w");
    FILE* fb = fopen("vector_b","w");

    fprintf(fa,"n=%d\n",size);

    for(int i=0; i<size; i++){
        fprintf(fa,"%f\n",1.00);
        fprintf(fb,"%f\n",1.00);
    }

    fclose(fa);
    fclose(fb);

    exit(0);

}