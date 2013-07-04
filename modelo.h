/*
 * modelo.h
 *
 *  Created on: 26/06/2013
 *      Author: Guilherme Sampaio
 */

#include "explosion.h"

#ifndef MODELO_H_
#define MODELO_H_

#define NUM_CORPOS  10
#define SOL       0

#define MERCURIO  0
#define VENUS     1

#define TERRA     2
#define LUA       0

#define MARTE     3
#define JUPITER   4

#define SATURNO   5
#define TITAN     0
#define FEBE      1
#define HIPERION  2

#define URANO     6
#define NETUNO    7
#define PLUTAO    8
#define ASTEROIDE 9

#define TAM_NOME 20
#define TAM_ARQ  80

typedef struct CorpoCeleste {
    // (Colisão) Nome usado para escrever o nome do planeta apontado pelo mouse
    char nome[TAM_NOME];
    // (OpenGL) distância entre o astro e seu foco gravitacional
    // ou seja, dist. entre planeta e sol, lua e planeta, etc.
    double dist;
    // (OpenGL) raio do astro, usado para  renderização
    double raio;
    // (Física) Fator de rotação (no próprio eixo) por dia
    double fr;
    // (Física) Fator de translação por dia
    double ft;
    // (Física) Distância entre o astro e seu foco gravitacional
    // (Escala) Define 'dist' de acordo com a escala espacial
    double raio_translacao;
    // (Física) Raio da circunferência do astro
    // (Escala) Define 'raio' de acordo com a escala planetária
    double raio_rotacao;
    // (OpenGL) Cor no desenho sem textura
    double cor[3];
    // (Design) Número de satélites
    int satelites;
    // (Design) Satélites de forma estruturada
    struct CorpoCeleste *satelite;
    // (Design) Ponteiro da funçção de customização (ex: anéis de saturno)
    double (*customizar)(CorpoCeleste*);
    // (Design) Ponteiro da função de desenho (ex: luas FEBE e HIPERION)
    void (*desenhar)(CorpoCeleste*, CorpoCeleste*, double);
    // (Design) Indicador de órbita especial (não faz o desenho circular padrão)
    bool especial;
    // (Colisão) Posição projetada do astro no espaço OpenGL
    GLdouble pos[3];
    // (Explosão) Flag de controle de visualização de um astro (afeta satélites)
    bool ativo;
    // (Explosão) Flag de controle de existência do astro (evita auto ativação)
    bool inexistente;
    // (OpenGL) nome do arquivo de textura do astro
    char arquivo[TAM_ARQ];
    // (OpenGL) Id da textura no servidor (engine remota ou local) OpenGL
    int textura;
    // (Explosão) Controle da explosão (partículas, detritos, energia, etc)
    Explosion explosao;
} CorpoCeleste;

typedef CorpoCeleste SistemaSolar;

//---------------------------------------------------------------------------

void inicia_sistema_solar(CorpoCeleste *sol);
void libera_astro(CorpoCeleste *astro);
void configura_escalas(CorpoCeleste *sol,
        double escalaRaio, double escalaDist);
void configura_asteroide(CorpoCeleste *asteroide,
        double escalaRaio, double escalaDist);
void ativa(CorpoCeleste *astro);

#endif /* MODELO_H_ */
