#!/bin/bash

rm -rf bin
mkdir bin
cd bin
mkdir util
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o modelo.o ../modelo.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/Texto.o ../util/Texto.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o solar.o ../solar.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o visao.o ../visao.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/explosion.o ../util/explosion.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/geometria.o ../util/geometria.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o controle.o ../controle.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/PickRay.o ../util/PickRay.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/Textura.o ../util/Textura.cpp
g++ -o solar visao.o util/geometria.o util/explosion.o util/Textura.o util/Texto.o util/PickRay.o solar.o modelo.o controle.o -lm -lGL -lGLU -lglut
