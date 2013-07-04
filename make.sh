#!/bin/bash

rm -rf bin/*
cd bin
mkdir util
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o modelo.o ../modelo.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/Text.o ../util/Text.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o solar.o ../solar.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o visao.o ../visao.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/explosion.o ../util/explosion.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/Color.o ../util/Color.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/geometria.o ../util/geometria.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o controle.o ../controle.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/PickRay.o ../util/PickRay.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/Texture.o ../util/Texture.cpp
g++ -DFREEGLUT_STATIC -I../util -O3 -Wall -c -fmessage-length=0 -o util/Position.o ../util/Position.cpp
g++ -o tpcg-gui visao.o util/geometria.o util/explosion.o util/Texture.o util/Text.o util/Position.o util/PickRay.o util/Color.o solar.o modelo.o controle.o -lm -lGL -lGLU -lglut
