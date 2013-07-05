/*
 * geometria.cpp
 *
 *  Created on: 08/05/2012
 *      Author: Frederico Sampaio
 *      Ref: Real-Time Collision Detection, ISBN: 1-55860-732-3
 *      Adaptadop por Gulherme Sampaio (inclusão de novas funções)
 *      O programa é baseado nos fontes disponível em http://www.opengl.org
 */

#include <math.h>

#include "geometria.h"

// Copia vetor l = r
void cpVet(double l[], double r[]) {
    l[0] = r[0];
    l[1] = r[1];
    l[2] = r[2];
}

// Soma escalar l = l + r
void somaEsc(double l[], double r[]) {
    l[0] += r[0];
    l[1] += r[1];
    l[2] += r[2];
}

// Soma escalar l = l + r
void subtEsc(double l[], double r[]) {
    l[0] -= r[0];
    l[1] -= r[1];
    l[2] -= r[2];
}

// Produto vetoral (Cross)
// p = v x w
void prodVet(double p[], double v[], double w[]) {
	p[0] = (v[1]*w[2]) - (v[2] * w[1]);
	p[1] = (v[0]*w[2]) - (v[2] * w[0]);
	p[2] = (v[0]*w[1]) - (v[1] * w[0]);
}

// Cria vetor v baseado nos pontoa A e B
// v = AB
void vetor(double v[], double A[], double B[]) {
    v[0] = (B[0] - A[0]);
    v[1] = (B[1] - A[1]);
    v[2] = (B[2] - A[2]);
}

// Multiplicação por escalar
// v = v x FATOR
void multPEsc(double v[], double f) {
    v[0] *= f;
    v[1] *= f;
    v[2] *= f;
}

// Divisão por escalar
// v = v x FATOR
void divPEsc(double v[], double f) {
    if (f) {
        v[0] /= f;
        v[1] /= f;
        v[2] /= f;
    }
}

// Soma vetorial em v
// v = v + w
void somaVet(double v[], double w[]) {
    v[0] += w[0];
    v[1] += w[1];
    v[2] += w[2];
}

// Módulo ou norma vetorial
// v = ||v||
double normaVet(double v[]) {
	return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

// Distancia entre dois pontos
double distPonto(double P[],double Q[]) {
    return sqrt(pow(P[0]-Q[0],2) + pow(P[1]-Q[1],2) + pow(P[2]-Q[2],2));
}

// produto interno (http://en.wikipedia.org/wiki/Dot_product)
double dotVet(double P[],double Q[]) {
    return (P[0] * Q[0] + P[1] * Q[1] + P[2] * Q[2]);
}

// distancia entre o ponto P e a reta formada pelos pontos Q e R
// d = ||QP x  QR|| / ||QR||
double distPontoReta(double P[], double Q[], double R[]) {
	// vetor v da reta R
	double v[3];
	vetor(v, Q, R);
	// vetor w de um ponto de r até P
	double w[3];
	vetor(w, P, Q);
	//  produto vetorial ||QP x QR||
	double prod[3];
	prodVet(prod, w, v);
	// retorna ||QP x  QR|| / ||QR||
	return normaVet(prod) / normaVet(v);
}

// angulo em Z da reta formada pelos pontos Q e R
double anguloZReta(double Q[], double R[]) {
    double x0 = Q[0];
    double y0 = Q[1];
    double x1 = R[0];
    double y1 = R[1];
    return atanf((y1 - y0) / (x1 - x0));
}

// distancia entre o ponto P e o segmento de reta formada pelos pontos Q e R
// Ref: http://realidadgh.wordpress.com/2013/01/19/
//      criando-um-sistem-detector-de-colisoes-parte-1-colisao-ponto-poligono/
double distPontoSegmento(double P[], double Q[], double R[])
{
    double x = P[0];
    double y = P[1];
    double x0 = Q[0];
    double y0 = Q[1];
    double x1 = R[0];
    double y1 = R[1];

    double theta = anguloZReta(Q, R);
    double co = cosf(-theta);
    double se = sinf(-theta);
    double z = x * co + y * se;
    double z0 = x0 * co + y0 * se;
    double z1 = x1 * co + y1 * se;

    if ((z0 < z && z < z1) || (z1 < z && z < z0)) {
        return distPontoReta(P, Q, R);
    } else {
        double d0 = distPonto(P, Q);
        double d1 = distPonto(P, R);
        if (d0 < d1)
            return d0;
        else
            return d1;
    }
}

// verifica se o ponto P está em direção da reta formada pelos pontos Q e R
double noSegmentoQR(double P[], double Q[], double R[])
{
    double x = P[0];
    double y = P[1];
    double x0 = Q[0];
    double y0 = Q[1];
    double x1 = R[0];
    double y1 = R[1];

    double theta = anguloZReta(Q, R);
    double co = cosf(-theta);
    double se = sinf(-theta);
    double z = x * co + y * se;
    double z0 = x0 * co + y0 * se;
    double z1 = x1 * co + y1 * se;

    return ((z0 < z && z < z1) || (z1 < z && z < z0));
}

/*
 * Real-Time Collision Detection
 * Copyright © 2005 Elsevier Inc. All rights reserved
 * Author(s): Christer Ericson
 * ISBN: 978-1-55860-732-3
 * Publisher's Note: Transferred to Taylor & Francis as of 2012
 *
 */
// Dados três pontos não colineares, calcula a equação do plano
// em função do vetor normal (ordem ccw) e o deslocamento
Plano plano(Ponto a, Ponto b, Ponto c)
{
    Plano p;
    Vetor ba, ca;
    prodVet(ba, b, a);
    prodVet(ca, c, a);
    prodVet(p.n, ba, ca);
    p.d = dotVet(p.n, a);
    return p;
}

// verifica se um segmento de reta intercepta um plano
// e, se existir, retorna o ponto de interceptação q
int interceptaSegmentoPlano(Ponto a, Ponto b, Plano p, double &t, Ponto &q)
{
    // Compute the t value for the directed line ab intersecting the plane
    Vetor ab; vetor(ab, a, b);
    t = (p.d - dotVet(p.n, a)) / dotVet(p.n, ab);
    // If t in [0..1] compute and return intersection point
    if (t >= 0.0 && t <= 1.0) {
        cpVet(q, ab);
        multPEsc(q, t);
        somaVet(q, a);
        return 1;
    }
    // Else no intersection
    return 0;
}

// pseudo code found at:
// http://www.gamedev.net/topic/221071-simple-raysphere-collision/
// Calcula Q, o ponto mais próximo da reta AB ao ponto P
// Retorna Q e t, que representa o deslocamento do ponto Q em relação à A
void pontoMaisProximo(double Q[], double A[], double B[], double P[], double *t)
{
    double AB[3]; // AB = B - A;
    cpVet(AB, B); subtEsc(AB, A);
    double ab_square = dotVet(AB, AB);
    double AP[3]; // AP = P - A;
    cpVet(AP, P); subtEsc(AP, A);
    double ap_dot_ab = dotVet(AP, AB);
    // t is a projection param when we project vector AP onto AB
    *t = ap_dot_ab / ab_square;
    // calculate the closest point
    //Q = A + AB * (*t);
    cpVet(Q, A); multPEsc(AB, *t); somaEsc(Q, AB);
}

// http://www.opengl.org/wiki/GluProject_and_gluUnProject_code
void MultiplyMatrixByVector4by4(double *resultvector, const double *matrix, const double *pvector)
{
  resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
  resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
  resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
  resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
}

void MultiplyMatrices4by4(double *result, double *matrix1, double *matrix2)
{
  result[0]=matrix1[0]*matrix2[0]+
    matrix1[4]*matrix2[1]+
    matrix1[8]*matrix2[2]+
    matrix1[12]*matrix2[3];
  result[4]=matrix1[0]*matrix2[4]+
    matrix1[4]*matrix2[5]+
    matrix1[8]*matrix2[6]+
    matrix1[12]*matrix2[7];
  result[8]=matrix1[0]*matrix2[8]+
    matrix1[4]*matrix2[9]+
    matrix1[8]*matrix2[10]+
    matrix1[12]*matrix2[11];
  result[12]=matrix1[0]*matrix2[12]+
    matrix1[4]*matrix2[13]+
    matrix1[8]*matrix2[14]+
    matrix1[12]*matrix2[15];
  result[1]=matrix1[1]*matrix2[0]+
    matrix1[5]*matrix2[1]+
    matrix1[9]*matrix2[2]+
    matrix1[13]*matrix2[3];
  result[5]=matrix1[1]*matrix2[4]+
    matrix1[5]*matrix2[5]+
    matrix1[9]*matrix2[6]+
    matrix1[13]*matrix2[7];
  result[9]=matrix1[1]*matrix2[8]+
    matrix1[5]*matrix2[9]+
    matrix1[9]*matrix2[10]+
    matrix1[13]*matrix2[11];
  result[13]=matrix1[1]*matrix2[12]+
    matrix1[5]*matrix2[13]+
    matrix1[9]*matrix2[14]+
    matrix1[13]*matrix2[15];
  result[2]=matrix1[2]*matrix2[0]+
    matrix1[6]*matrix2[1]+
    matrix1[10]*matrix2[2]+
    matrix1[14]*matrix2[3];
  result[6]=matrix1[2]*matrix2[4]+
    matrix1[6]*matrix2[5]+
    matrix1[10]*matrix2[6]+
    matrix1[14]*matrix2[7];
  result[10]=matrix1[2]*matrix2[8]+
    matrix1[6]*matrix2[9]+
    matrix1[10]*matrix2[10]+
    matrix1[14]*matrix2[11];
  result[14]=matrix1[2]*matrix2[12]+
    matrix1[6]*matrix2[13]+
    matrix1[10]*matrix2[14]+
    matrix1[14]*matrix2[15];
  result[3]=matrix1[3]*matrix2[0]+
    matrix1[7]*matrix2[1]+
    matrix1[11]*matrix2[2]+
    matrix1[15]*matrix2[3];
  result[7]=matrix1[3]*matrix2[4]+
    matrix1[7]*matrix2[5]+
    matrix1[11]*matrix2[6]+
    matrix1[15]*matrix2[7];
  result[11]=matrix1[3]*matrix2[8]+
    matrix1[7]*matrix2[9]+
    matrix1[11]*matrix2[10]+
    matrix1[15]*matrix2[11];
  result[15]=matrix1[3]*matrix2[12]+
    matrix1[7]*matrix2[13]+
    matrix1[11]*matrix2[14]+
    matrix1[15]*matrix2[15];
}

#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]

//This code comes directly from GLU except that it is for double
int InvertMatrixf2(double *out, double *m)
{
	double wtmp[4][8];
	double m0, m1, m2, m3, s;
	double *r0, *r1, *r2, *r3;
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
			r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
			r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
			r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
			r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
			r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
			r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
			r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
			r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
			r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
			r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
			r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
	/* choose pivot - or die */
	if (fabsf(r3[0]) > fabsf(r2[0]))
		SWAP_ROWS_DOUBLE(r3, r2);
	if (fabsf(r2[0]) > fabsf(r1[0]))
		SWAP_ROWS_DOUBLE(r2, r1);
	if (fabsf(r1[0]) > fabsf(r0[0]))
		SWAP_ROWS_DOUBLE(r1, r0);
	if (0.0 == r0[0])
		return 0;
	/* eliminate first variable     */
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[1]) > fabsf(r2[1]))
		SWAP_ROWS_DOUBLE(r3, r2);
	if (fabsf(r2[1]) > fabsf(r1[1]))
		SWAP_ROWS_DOUBLE(r2, r1);
	if (0.0 == r1[1])
		return 0;
	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[2]) > fabsf(r2[2]))
		SWAP_ROWS_DOUBLE(r3, r2);
	if (0.0 == r2[2])
		return 0;
	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
			r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
	/* last check */
	if (0.0 == r3[3])
		return 0;
	s = 1.0 / r3[3];		/* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	m2 = r2[3];			/* now back substitute row 2 */
	s = 1.0 / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
			r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
			r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
			r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	m1 = r1[2];			/* now back substitute row 1 */
	s = 1.0 / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
			r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
			r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	m0 = r0[1];			/* now back substitute row 0 */
	s = 1.0 / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
			r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];
	return 1;
}
