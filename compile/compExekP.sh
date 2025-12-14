#!/bin/bash

echo "Compilatu"
gcc -O2 -fopenmp -o ../multzohitz_p ../src/paralelo/multzohitz_p.c ../src/paralelo/funth_p.c -lm #Compilatu

echo "Exekutatu"
../multzohitz_p ../../ARK/3-proiektua/hitzbektoreak.dat ../../ARK/3-proiektua/ztalor.dat

echo "Konparatu"
diff ../emaitzak/emaitzak_p.out ../../ARK/3-proiektua/emaitzak.out
