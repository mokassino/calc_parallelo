#! /usr/bin/python3

from random import randrange
from sys import exit
import subprocess

# Questo script serve a testare in automatico il funzionamento del progetto

REP=10 # Numero di ripetizioni del programma
ttot=0 # somma dei tempi ricavati
N=10000000 # Dimensione del problema N cioè la dimensione degli array a,b,c

subprocess.run(["python3","file_populator.py",str(N)]) # Esegui lo script per popolare il file

out = subprocess.run(["gcc", "-o", "file_populator", "file_populator.c"]);

if ( out.returncode == 1):
    printf("File 'file_populator.c' non trovato\n");
    exit(1);

subprocess.run(["./file_populator",str(N)])

print("File vector_a e vector_b creati con successo")

alpha=randrange(2,10); # Un arbitrario valore alpha con cui eseguire il file vectorsum

out = subprocess.run(["gcc", "-o", "vectorsum", "vectorsum.c", "-fopenmp"]) # compilazione

if ( out.returncode == 1): # Se non sei riuscito a compilare il file, esci
    print("File 'vectorsum.c' non trovato")
    exit(1)

subprocess.run(["rm","timings","--force"]) # ignora se il file non esiste

for i in range(REP): #Esegui vectorsum per #REP volte
    subprocess.run(["./vectorsum", str(alpha)])


# Leggi tutti i tempi scritti su file da vectorsum
with open("timings","r") as f:
    for i in range(REP):
        try:
            time = float(f.readline()) #time = tempo dal file
            ttot = ttot + time
        except: pass

print("Ripetizioni del programma vectorsum effettuate: " + str(REP)+  "\nSomma dei tempi di esecuzione delle operazioni: " + str(ttot) )
print("Media dei tempi di esecuzione: " + str(float(ttot/REP)) + "\n")