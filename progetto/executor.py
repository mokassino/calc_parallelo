#! /usr/bin/python3

from random import randrange
from sys import exit
import sys
import time
import subprocess

# Questo script serve a testare in automatico il funzionamento del progetto

REP=100 # Numero di ripetizioni del programma
ttot=0 # somma dei tempi ricavati
N=100 # Dimensione del problema N cioè la dimensione degli array a,b,c
NPROC=2

if len(sys.argv) < 3:
    print("Inserisci nproc e N in input\n");
    exit(1)

NPROC = int(sys.argv[1])
N = int(sys.argv[2])


print("Compilazione ed esecuzione dello script popolamento...\\")

out = subprocess.run(["gcc", "-o", "file_populator", "file_populator.c"]);

if ( out.returncode == 1):
    printf("File 'file_populator.c' non trovato\n");
    exit(1)

out = subprocess.run(["./file_populator",str(N)])


if ( out.returncode == 0):
    print("File vector_a e vector_b creati con successo.../")

alpha=randrange(2,10); # Un arbitrario valore alpha con cui eseguire il file vectorsum

print("Generato alpha=" + str(alpha) + "...\\ \nCompilazione ed esecuzione di vectorsum per #"+str(REP)+" volte.../\n")

out = subprocess.run(["gcc", "-o", "vectorsum", "vectorsum.c", "-fopenmp"]) # compilazione

if ( out.returncode == 1): # Se non sei riuscito a compilare il file, esci
    print("Compilazione del file 'vectorsum.c' fallita")
    exit(1)

subprocess.run(["rm","timings","--force"]) # ignora se il file non esiste


for i in range(REP): #Esegui vectorsum per #REP volte
    subprocess.run(["./vectorsum", str(NPROC), str(alpha)])
    print("Progresso: {}/{}".format(i,REP),end='\r',file=sys.stdout,flush=True)


print("Esecuzioni effettuate, inizio lettura dei valori da file...\\")

# Leggi tutti i tempi scritti su file da vectorsum
with open("timings","r") as f:
    for i in range(REP):
        try:
            time = float(f.readline()) #time = tempo dal file
            ttot = ttot + time
        except: pass

print("\nRipetizioni del programma vectorsum effettuate: " + str(REP)+ " con NPROC = " + str(NPROC) + " e N = " + str(N) + "\nSomma dei tempi di esecuzione delle operazioni: " + str(ttot) )
print("Media dei tempi di esecuzione: " + str(float(ttot/REP)) + "\n")

with open("results","a") as f:
    if (f.tell() == 0):
        print("Rep\tN Proc\tSize\tResult",end='\n',file=f)
    else:
        print(str(REP)+"\t"+str(NPROC)+"\t"+str(N)+"\t"+str(ttot)+"\n",file=f)
