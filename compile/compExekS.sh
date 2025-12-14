#!/bin/bash

echo "Compilatu"
gcc -O2 -o ../multzohitz_s ../src/serie/multzohitz_s.c ../src/serie/funth_s.c -lm #Compilatu

echo "Exekutatu"
../multzohitz_s ../../ARK/3-proiektua/hitzbektoreak.dat ../../ARK/3-proiektua/ztalor.dat

echo "Konparatu"
diff ../emaitzak/emaitzak_s.out ../../ARK/3-proiektua/emaitzak.out