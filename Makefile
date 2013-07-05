all:
	rm -rf bin
	mkdir -p bin/util
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/modelo.o modelo.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/util/Texto.o util/Texto.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/solar.o solar.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/visao.o visao.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/util/explosion.o util/explosion.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/util/geometria.o util/geometria.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/controle.o controle.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/util/PickRay.o util/PickRay.cpp
	g++ -DFREEGLUT_STATIC -Iutil -O3 -Wall -c -fmessage-length=0 -o bin/util/Textura.o util/Textura.cpp
	g++ -o solar bin/visao.o bin/util/geometria.o bin/util/explosion.o bin/util/Textura.o bin/util/Texto.o bin/util/PickRay.o bin/solar.o bin/modelo.o bin/controle.o -lm -lGL -lGLU -lglut
	./solar
