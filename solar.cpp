/*
 * visao.cpp
 *
 *  Created on: 16/06/2013
 *      Author: Guilherme Sampaio
 */


#include <GL/freeglut.h>

#include <stdio.h>
#include <time.h>

#include "controle.h"

int main(int argc, char** argv) {

	// forçar saída imediata no console do eclipse
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
    srand (time (NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow ("TP3 - Sistema Solar - Guilherme Sampaio");

    inicia_controle();

    glutMainLoop();
    return 0;
}

