/*
 * controle.h
 *
 *  Created on: 26/06/2013
 *      Author: Guilherme Sampaio
 */

#ifndef CONTROLE_H_
#define CONTROLE_H_

#include "modelo.h"

#define ESCALA_PADRAO_DIST_MAX  25000000.00
#define ESCALA_PADRAO_DIST_MIN  500000.0

#define ESCALA_PADRAO_RAIO_MAX  5000000.0
#define ESCALA_PADRAO_RAIO_MIN  20000.0

#define ESCALA_PADRAO_DIST      10000000.0
#define ESCALA_PADRAO_RAIO      25000.0

#define DIST_CAMERA 200.00

void inicia_controle();
void posicao_inicial();

void posicao_camera(GLdouble pos[]);
bool desenha_orbita();
bool desenha_textura();
double angulo_translacao(CorpoCeleste *a);
double angulo_rotacao (CorpoCeleste *a);
double angulo_translacao(CorpoCeleste *a, double dia);
double angulo_rotacao (CorpoCeleste *a, double dia);
void orbita_asteroide(double Q[3], double R[3]);
double get_dia();
double precisao_orbita();
CorpoCeleste *get_em_foco();

#endif /* CONTROLE_H_ */
