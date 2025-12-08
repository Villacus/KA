#!/bin/bash

git pull

echo "Compilatu"
gcc -O2 -fopenmp -o ../multzohitz_p ../src/paralelo/multzohitz_p.c ../src/paralelo/funth_p.c -lm #Compilatu

echo "Exekutatu"
../multzohitz_p ../../ARK/3-proiektua/hitzbektoreak.dat ../../ARK/3-proiektua/ztalor.dat 1000 #soilik 1000 hitzekin

echo "Konparatu"
diff ../emaitzak/emaitzak_p.out ../../ARK/3-proiektua/emaitzak1000.out

echo "Git eguneraketa"
cd ..
git add .
git commit -m "Archivos despues de compilar"
git push