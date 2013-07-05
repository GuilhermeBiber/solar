/*
 * PlanetasMath.h
 */

#ifndef GEOMETRIA_H_
#define GEOMETRIA_H_

typedef double Vetor[3];
typedef double Ponto[3];

struct Plano {
        Vetor n; // Vetro normal do plano. Qualquer ponto do plano Dot(n,x) = d
        double d; // d = dot(n,p) de um ponto qualquer do plano
};

// funções implícitas
#define normalizar(v)    divPEsc(v, normaVet(v))

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
void vetor(double v[], double A[], double B[]);
// Módulo ou norma vetorial
double normaVet(double v[]);
// distância entre o ponto P e a reta formada pelos pontos Q e R
double distPontoReta(double P[], double Q[], double R[]);
// produto interno de vetores P e Q
double dotVet(double P[], double Q[]);
// distância entre dois pontos
double distPonto(double P[], double Q[]);
// distância entre um ponto e o segmento de reta
double distPontoSegmento(double P[], double Q[], double R[]);
// verifica se um ponto está em um segimento de reta
double noSegmento(double P[], double Q[], double R[]);
// retorna o ângulo da reta em função do eixo Z
double anguloZReta(double Q[], double R[]);
// ponto Q mais próxmio da reta AB e ponto Q (t - deslocamento Q->A)
void pontoMaisProximo(double Q[], double A[], double B[], double P[], double *t);
// verifica se um plano p é interceptado por um segmento de reta AB
int interceptaSegmentoPlano(Ponto a, Ponto b, Plano p, double &t, Ponto &q);

// multiplicação de matrizes 4x4 por vetor 4
void MultiplyMatrixByVector4by4(double *resultvector, const double *matrix, const double *pvector);

void MultiplyMatrices4by4(double *result, double *matrix1, double *matrix2);
int InvertMatrixf2(double *out, double *m);

#endif /* GEOMETRIA_H_ */
