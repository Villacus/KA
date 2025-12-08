#!/bin/bash

echo "Compilatu"
gcc -O2 -fopenmp -o multzohitz_p multzohitz_p.c funth_p.c -lm #Compilatu

echo "Exekutatu"
./multzohitz_p ../ARK/3-proiektua/hitzbektoreak.dat ../ARK/3-proiektua/ztalor.dat 1000 #soilik 1000 hitzekin

echo "Konparatu"
diff emaitzak_s.out ../ARK/3-proiektua/emaitzak1000.out
