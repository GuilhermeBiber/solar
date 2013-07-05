/*
 * Text.cpp
 *
 *  Created on: 16/06/2012
 *      Author: Frederico Sampaio
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <GL/glut.h>
#include <string.h>

#include "Texto.h"

//  Pointer to a font style..
//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13,
//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10,
//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
//

//----------------------------------------------------------------------------
void Texto::setText(char *texto)
{
    strncpy(message, texto, 256);
}

void Texto::setText(const char* format, ...)
{
	va_list args;	//  Lista de argumentos da função
	char texto[256];	//	Text
	//  inicializa a lista de variáveis
	va_start(args, format);
	//  Escreve o texto na string
	vsprintf(texto, format, args);
	//  End using variable argument list
	va_end(args);
	// copia o resultado
	strncpy(message, texto, 256);
}

void Texto::setPosition(double _x, double _y, double _z)
{
    x = _x; y = _y, z = _z;
}


void Texto::setColor(int rgb[3])
{
    cor[0] = ((double)rgb[0])/255.0;
    cor[1] = ((double)rgb[1])/255.0;
    cor[2] = ((double)rgb[2])/255.0;
}

void Texto::draw()
{
	// Imprime o texto
	// cor do texto
	glColor3d(cor[0], cor[1], cor[2]);
	//  Specify the raster position for pixel operations.
	glRasterPos3d (x, y, z);
	//
	const char *text = message;
	//  Draw the characters one by one
	for (int i = 0; text[i] != '\0'; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	// Retorna o sistema de coordenadas do modelo
}

void Texto::draw2D()
{
	// Salva o sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		// Zera o sistema de coordenadas de projeção
		glLoadIdentity();
		// Salva sistema de coordenadas do modelo
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			// Zera o sistema de coordenadas do modelo
			glLoadIdentity();
			draw();
		glPopMatrix();
	// Retorna o sistema de coordenadas de projeção
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	// retonra para a matriz de modelo
	glMatrixMode(GL_MODELVIEW);
}
