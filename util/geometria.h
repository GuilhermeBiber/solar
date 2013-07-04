/*
 * PlanetasMath.h
 */

#ifndef PLANETASMATH_H_
#define PLANETASMATH_H_

// copia um vetor em outro
void cpVet(double l[], double r[]);
// Soma escalar
void somaEsc(double l[], double r[]);
// Produto e divisão vetoral
void prodVet(double p[], double v[], double w[]);
void divPEsc(double v[], double f);
// Multiplicação por escalar (direto no vetor)
void multPEsc(double v[], double f);
// Cria vetor v baseado nos pontoa A e B
void vetorAB(double v[], double A[], double B[]);
// Módulo ou norma vetorial
double normaVet(double v[]);
// distância entre o ponto P e a reta formada pelos pontos Q e R
double distPontoP_RetaQR(double P[], double Q[], double R[]);
// produto interno de vetores P e Q
double prodIntPQ(double P[],double Q[]);
// distância entre dois pontos
double distPontoPQ(double P[],double Q[]);
// distância entre um ponto e o segmento de reta
double distPontoP_SegmentoQR(double P[], double Q[], double R[]);
// verifica se um ponto está em um segimento de reta
double noSegmentoQR(double P[], double Q[], double R[]);
// retorna o ângulo da reta em função do eixo Z
double anguloZRetaQR(double Q[], double R[]);
// ponto Q mais próxmio da reta AB e ponto Q (t - deslocamento Q->A)
void pontoMaisProximo(double Q[], double A[], double B[], double P[], double *t);


// multiplicação de matrizes 4x4 por vetor 4
void MultiplyMatrixByVector4by4(double *resultvector, const double *matrix, const double *pvector);

void MultiplyMatrices4by4(double *result, double *matrix1, double *matrix2);
int InvertMatrixf2(double *out, double *m);

#endif /* PLANETASMATH_H_ */
