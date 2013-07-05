/*
 * visao.h
 *
 *  Created on: 26/06/2013
 *      Author: Guilherme Sampaio
 */

#ifndef VISAO_H_
#define VISAO_H_

#include "modelo.h"

#define C_R 0
#define C_G 1
#define C_B 2

#define SET_COR(a,b) {(a)[C_R]=(b)[C_R]; (a)[C_G]=(b)[C_G]; (a)[C_B]=(b)[C_B];}


//----------------------------------------------------------------------------

void configura_visao();
void pos_camera(double longitude, double latitude);
void alterna_luz();
void configura_saturno(CorpoCeleste *sol);
void showAxis();
void desenhar_sol(CorpoCeleste *);
void desenhar_asteroide(CorpoCeleste *, CorpoCeleste *, double);
void renderizar(bool m);
bool get_renderizar();

void iniciar_colapso_anel_saturno();
void terminar_colapso_anel_saturno();
bool em_colapso_anel_saturno();
bool anel_saturno_ativo();
void desenhar_mouse_cursor(int x, int y, int l, int a, double dimensao);

#define FATOR_ANEL_SATURNO    2.05

#endif /* VISAO_H_ */
