#!/bin/bash

gcc -O2 -o multzohitz_s multzohitz_s.c funth_s.c -lm #Compilatu

./multzohitz_s ../ARK/3-proiektua/hitzbektoreak.dat ../ARK/3-proiektua/ztalor.dat 1000 #soilik 1000 hitzekin
