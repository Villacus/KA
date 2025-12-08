#!/bin/bash

git pull

echo "Compilatu"
gcc -O2 -o ../multzohitz_s ../src/serie/multzohitz_s.c ../src/serie/funth_s.c -lm #Compilatu

echo "Exekutatu"
../multzohitz_s ../../ARK/3-proiektua/hitzbektoreak.dat ../../ARK/3-proiektua/ztalor.dat 1000 #soilik 1000 hitzekin

echo "Konparatu"
diff ../emaitzak/emaitzak_s.out ../../ARK/3-proiektua/emaitzak1000.out

echo "Git eguneraketa"
cd ..
git add .
git commit -m "Archivos despues de compilar"
git push