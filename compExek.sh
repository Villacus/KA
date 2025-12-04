#!/bin/bash

echo "Compilatu"
gcc -O2 -o multzohitz_s multzohitz_s.c funth_s.c -lm #Compilatu

echo "Exekutatu"
./multzohitz_s ../ARK/3-proiektua/hitzbektoreak.dat ../ARK/3-proiektua/ztalor.dat 1000 #soilik 1000 hitzekin

echo "Konparatu"
diff emaitzak_s.out emaitzak1000.out
