/*
 * modelo.cpp
 *
 *  Created on: 16/06/2013
 *      Author: Guilherme Sampaio
 *
 *  Texturas e dados:
 *  http://planetpixelemporium.com/planets.html
 *  http://www.planetaryvisions.com/
 *  http://www.solarviews.com/portug/toc.htm
 *
 */
#include <GL/glut.h>
#include <string.h>

#include "visao.h"
#include "modelo.h"

static double
    c_vermelha[3] = {1.0, 0.0, 0.0},
    c_verde[3] = {0.0, 1.0, 0.0},
    c_azul[3] = {0.0, 0.0, 1.0},
    //c_branca[3] = {1.0, 1.0, 1.0},
    //c_amarelo[3] = {1.0, 1.0, 0.0},
    //c_laranja[3] = {1.0,165.0/255.0,0},
    c_sol[3] = {1.0, 1.0, 0.0},
    c_mercurio[3] = {140.0/255.0, 140.0/255.0, 140.0/255.0},
    c_venus[3] = {1.0, 165.0/255.0, 79.0/255.0},
    c_terra[3] = {0.0, 0.0, 1.0},
        c_lua[3] = {1.0, 1.0, 1.0},
    c_marte[3] = {1.0, 0.0, 0.0},
    c_jupiter[3] = {89.0/255.0, 89.0/255.0, 171.0/255.0},
    c_saturno[3] = {1.0, 215.0/255.0, 0.0},
    c_urano[3] = {173.0/255.0, 234.0/255.0, 234.0/255.0},
    c_netuno[3] = {67 / 255, 110/255, 238.0/255.0},
    c_plutao[3] = {1.0, 239.0/255.0, 213.0/255.0};

// Referência de tamanho usada para definir a intensidade (carga) da explosão
// Poderia ser usado a massa ou qualquer outra informação
// no caso, a explosão será proposcional ao tamanho (raio) do corpo celeste
#define RAIO_JUPITER  (142984.0 / 2.0)

#define MAX_ENEGIA    150.0
#define ENERGIA(a)    ((a->raio_rotacao / RAIO_JUPITER) * MAX_ENEGIA)

// configura e aloca a memória usada nas explosões
// faz de forma adiantada para evitar problemas de controle
// de contexto entre as "threads" do glut
void inicia_explosoes(CorpoCeleste *astro)
{
    astro->explosao = newExplosion(ENERGIA(astro));
    for (int i = 0; i < astro->satelites; i++)
        inicia_explosoes(&(astro->satelite[i]));
}

// principal configuração do sistema solar
// dados obtidos em vários sites da WEB
void inicia_sistema_solar(CorpoCeleste *sol)
{
    strncpy(sol->nome, "SOL", TAM_NOME);
    SET_COR(sol->cor, c_sol);
    sol->dist = 0;
    sol->fr = 0.1;
    sol->ft = 0;
    sol->raio_translacao = 0;
    sol->raio_rotacao = 1391000.00 / 2.0;
    sol->customizar = NULL;
    sol->especial = false;
    sol->ativo = true;
    strncpy(sol->arquivo, "texturas/sol.bmp", TAM_ARQ);
    sol->satelites = NUM_CORPOS;
    sol->satelite = (CorpoCeleste *)
            calloc(sol->satelites, sizeof(CorpoCeleste));

    SET_COR(sol->satelite[MERCURIO].cor, c_mercurio);
    strncpy(sol->satelite[MERCURIO].nome, "Mercurio", TAM_NOME);
    sol->satelite[MERCURIO].fr = 1.0 / 58.65;
    sol->satelite[MERCURIO].ft = 1.0 / 87.95;
    sol->satelite[MERCURIO].raio_translacao = 57910000.00;
    sol->satelite[MERCURIO].raio_rotacao = 4879.40 / 2.0;
    sol->satelite[MERCURIO].satelites = 0;
    sol->satelite[MERCURIO].customizar = NULL;
    sol->satelite[MERCURIO].especial = false;
    sol->satelite[MERCURIO].ativo = true;
    strncpy(sol->satelite[MERCURIO].arquivo, "texturas/mercurio.bmp", TAM_ARQ);

    SET_COR(sol->satelite[VENUS].cor, c_venus);
    strncpy(sol->satelite[VENUS].nome, "Venus", TAM_NOME);
    sol->satelite[VENUS].fr = 1.0 / 243.02;
    sol->satelite[VENUS].ft = 1.0 / 226.0;
    sol->satelite[VENUS].raio_translacao = 108200000.00;
    sol->satelite[VENUS].raio_rotacao = 12103.60 / 2.0;
    sol->satelite[VENUS].satelites = 0;
    sol->satelite[VENUS].customizar = NULL;
    sol->satelite[VENUS].especial = false;
    sol->satelite[VENUS].ativo = true;
    strncpy(sol->satelite[VENUS].arquivo, "texturas/venus.bmp", TAM_ARQ);

    SET_COR(sol->satelite[TERRA].cor, c_terra);
    strncpy(sol->satelite[TERRA].nome, "Terra", TAM_NOME);
    sol->satelite[TERRA].fr = 1.0;
    sol->satelite[TERRA].ft = 1.0 / 365.25;
    sol->satelite[TERRA].raio_translacao = 149600000.0;
    sol->satelite[TERRA].raio_rotacao = 12756.20 / 2.0;
    sol->satelite[TERRA].customizar = NULL;
    sol->satelite[TERRA].especial = false;
    sol->satelite[TERRA].ativo = true;
    strncpy(sol->satelite[TERRA].arquivo, "texturas/terra.bmp", TAM_ARQ);
    sol->satelite[TERRA].satelites = 1;
    sol->satelite[TERRA].satelite = (CorpoCeleste *)
            calloc(sol->satelite[TERRA].satelites, sizeof(CorpoCeleste));
        SET_COR(sol->satelite[TERRA].satelite[LUA].cor, c_lua);
        strncpy(sol->satelite[TERRA].satelite[LUA].nome, "Lua", TAM_NOME);
        sol->satelite[TERRA].satelite[LUA].fr = 1.0 / 27.32;
        sol->satelite[TERRA].satelite[LUA].ft = 1.0 / 27.32;
        sol->satelite[TERRA].satelite[LUA].raio_translacao = 384400.00;
        sol->satelite[TERRA].satelite[LUA].raio_rotacao = 3474.80 / 2.0;
        sol->satelite[TERRA].satelite[LUA].satelites = 0;
        sol->satelite[TERRA].satelite[LUA].customizar = NULL;
        sol->satelite[TERRA].satelite[LUA].especial = false;
        sol->satelite[TERRA].satelite[LUA].ativo = true;
        strncpy(sol->satelite[TERRA].satelite[LUA].arquivo, "texturas/lua.bmp", TAM_ARQ);

    SET_COR(sol->satelite[MARTE].cor, c_marte);
    strncpy(sol->satelite[MARTE].nome, "Marte", TAM_NOME);
    sol->satelite[MARTE].fr = 1.025;
    sol->satelite[MARTE].ft = 1.0 / 686.97;
    sol->satelite[MARTE].raio_translacao = 227940000.00;
    sol->satelite[MARTE].raio_rotacao = 6794.40 / 2.0;
    sol->satelite[MARTE].satelites = 0;
    sol->satelite[MARTE].customizar = NULL;
    sol->satelite[MARTE].especial = false;
    sol->satelite[MARTE].ativo = true;
    strncpy(sol->satelite[MARTE].arquivo, "texturas/marte.bmp", TAM_ARQ);

    SET_COR(sol->satelite[JUPITER].cor, c_jupiter);
    strncpy(sol->satelite[JUPITER].nome, "Jupiter", TAM_NOME);
    sol->satelite[JUPITER].fr = 9.9 / 24;
    sol->satelite[JUPITER].ft = 1.0 / (4331572.0);
    sol->satelite[JUPITER].raio_translacao = 778547200.0;
    sol->satelite[JUPITER].raio_rotacao = RAIO_JUPITER;
    sol->satelite[JUPITER].satelites = 0;
    sol->satelite[JUPITER].customizar = NULL;
    sol->satelite[JUPITER].especial = false;
    sol->satelite[JUPITER].ativo = true;
    strncpy(sol->satelite[JUPITER].arquivo, "texturas/jupter.bmp", TAM_ARQ);

    SET_COR(sol->satelite[SATURNO].cor, c_saturno);
    strncpy(sol->satelite[SATURNO].nome, "Saturno", TAM_NOME);
    sol->satelite[SATURNO].fr = 10.7 / 24;
    sol->satelite[SATURNO].ft = 1.0 / 167.0;
    sol->satelite[SATURNO].raio_translacao = 1433000000.00;
    sol->satelite[SATURNO].raio_rotacao = 120536.00 / 2;
    sol->satelite[SATURNO].customizar = NULL;
    sol->satelite[SATURNO].especial = false;
    sol->satelite[SATURNO].ativo = true;
    strncpy(sol->satelite[SATURNO].arquivo, "texturas/saturno.bmp", TAM_ARQ);
    sol->satelite[SATURNO].satelites = 3;
    sol->satelite[SATURNO].satelite = (CorpoCeleste *)
            calloc(sol->satelite[SATURNO].satelites, sizeof(CorpoCeleste));
        SET_COR(sol->satelite[SATURNO].satelite[TITAN].cor, c_azul);
        strncpy(sol->satelite[SATURNO].satelite[TITAN].nome, "Titan", TAM_NOME);
        sol->satelite[SATURNO].satelite[TITAN].fr = 1.0 / 15.94;
        sol->satelite[SATURNO].satelite[TITAN].ft = 1.0 / 15.94;
        sol->satelite[SATURNO].satelite[TITAN].raio_translacao = (1221850.00*5);
        sol->satelite[SATURNO].satelite[TITAN].raio_rotacao = 5100.00;
        sol->satelite[SATURNO].satelite[TITAN].satelites = 0;
        sol->satelite[SATURNO].satelite[TITAN].customizar = NULL;
        sol->satelite[SATURNO].satelite[TITAN].especial = false;
        sol->satelite[SATURNO].satelite[TITAN].ativo = true;
        strncpy(sol->satelite[SATURNO].satelite[TITAN].arquivo, "texturas/titan.bmp", TAM_ARQ);

        // os dados foram ajustados para melhor adaptar ao TP
        SET_COR(sol->satelite[SATURNO].satelite[FEBE].cor, c_vermelha);
        strncpy(sol->satelite[SATURNO].satelite[FEBE].nome, "Febe", TAM_NOME);
        sol->satelite[SATURNO].satelite[FEBE].fr = 0.4;
        sol->satelite[SATURNO].satelite[FEBE].ft = 1.0 / 550.48;
        sol->satelite[SATURNO].satelite[FEBE].raio_translacao = 12952000.00 * 10;
        sol->satelite[SATURNO].satelite[FEBE].raio_rotacao = 220.00 * 5;
        sol->satelite[SATURNO].satelite[FEBE].satelites = 0;
        sol->satelite[SATURNO].satelite[FEBE].customizar = NULL;
        sol->satelite[SATURNO].satelite[FEBE].especial = true;
        sol->satelite[SATURNO].satelite[FEBE].ativo = true;
        strncpy(sol->satelite[SATURNO].satelite[FEBE].arquivo, "texturas/febe.bmp", TAM_ARQ);

        // os dados foram ajustados para melhor adaptar ao TP
        SET_COR(sol->satelite[SATURNO].satelite[HIPERION].cor, c_verde);
        strncpy(sol->satelite[SATURNO].satelite[HIPERION].nome, "Hiperion", TAM_NOME);
        sol->satelite[SATURNO].satelite[HIPERION].fr = 2.0 / 24.0;
        sol->satelite[SATURNO].satelite[HIPERION].ft = 1.0 / (21.2 * 100.0);
        sol->satelite[SATURNO].satelite[HIPERION].raio_translacao = 1481100.00;
        sol->satelite[SATURNO].satelite[HIPERION].raio_rotacao = 270.00 * 10;
        sol->satelite[SATURNO].satelite[HIPERION].satelites = 0;
        sol->satelite[SATURNO].satelite[HIPERION].customizar = NULL;
        sol->satelite[SATURNO].satelite[HIPERION].especial = true;
        sol->satelite[SATURNO].satelite[HIPERION].ativo = true;
        strncpy(sol->satelite[SATURNO].satelite[HIPERION].arquivo, "texturas/hiperion.bmp", TAM_ARQ);

/*
        // os dados foram ajustados para melhor adaptar ao TP
        SET_COR(sol->satelite[SATURNO].satelite[ANEIS].cor, c_branca);
        strncpy(sol->satelite[SATURNO].satelite[ANEIS].nome, "Aneis de saturno", TAM_NOME);
        sol->satelite[SATURNO].satelite[ANEIS].fr = 0.0;
        sol->satelite[SATURNO].satelite[ANEIS].ft = 0.0;
        sol->satelite[SATURNO].satelite[ANEIS].raio_translacao = 213000.0;
        sol->satelite[SATURNO].satelite[ANEIS].raio_rotacao = 69000.0;
        sol->satelite[SATURNO].satelite[ANEIS].satelites = 0;
        sol->satelite[SATURNO].satelite[ANEIS].customizar = NULL;
        sol->satelite[SATURNO].satelite[ANEIS].especial = true;
        sol->satelite[SATURNO].satelite[ANEIS].ativo = true;
        strncpy(sol->satelite[SATURNO].satelite[ANEIS].arquivo, "", TAM_ARQ);
*/

    SET_COR(sol->satelite[URANO].cor, c_urano);
    strncpy(sol->satelite[URANO].nome, "Urano", TAM_NOME);
    sol->satelite[URANO].fr = 17.2 / 24;
    sol->satelite[URANO].ft = 1.0 / (84.01 * 365.25);
    sol->satelite[URANO].raio_translacao = 2870990000.0;
    sol->satelite[URANO].raio_rotacao = 51118.0 / 2.0;
    sol->satelite[URANO].satelites = 0;
    sol->satelite[URANO].customizar = NULL;
    sol->satelite[URANO].especial = false;
    sol->satelite[URANO].ativo = true;
    strncpy(sol->satelite[URANO].arquivo, "texturas/urano.bmp", TAM_ARQ);

    SET_COR(sol->satelite[NETUNO].cor, c_netuno);
    strncpy(sol->satelite[NETUNO].nome, "Netuno", TAM_NOME);
    sol->satelite[NETUNO].fr = 16.10 / 24;
    sol->satelite[NETUNO].ft = 1.0 / (164.79 * 365.25);
    sol->satelite[NETUNO].raio_translacao = 4504300000.0;
    sol->satelite[NETUNO].raio_rotacao = 49538.0 / 2.0;
    sol->satelite[NETUNO].satelites = 0;
    sol->satelite[NETUNO].customizar = NULL;
    sol->satelite[NETUNO].especial = false;
    sol->satelite[NETUNO].ativo = true;
    strncpy(sol->satelite[NETUNO].arquivo, "texturas/netuno.bmp", TAM_ARQ);

    // os dados foram ajustados para melhor adaptar ao TP
    SET_COR(sol->satelite[PLUTAO].cor, c_plutao);
    strncpy(sol->satelite[PLUTAO].nome, "Plutao", TAM_NOME);
    sol->satelite[PLUTAO].fr = 6.39;
    sol->satelite[PLUTAO].ft = 1.0 / (247.7 * 365.25);
    sol->satelite[PLUTAO].raio_translacao = 5922000000.0;
    sol->satelite[PLUTAO].raio_rotacao = (23200.0 / 2);
    sol->satelite[PLUTAO].satelites = 0;
    sol->satelite[PLUTAO].customizar = NULL;
    sol->satelite[PLUTAO].especial = false;
    sol->satelite[PLUTAO].ativo = true;
    strncpy(sol->satelite[PLUTAO].arquivo, "texturas/plutao.bmp", TAM_ARQ);

    // requisito do TP
    strncpy(sol->satelite[ASTEROIDE].nome, "Asteroide", TAM_NOME);
    SET_COR(sol->satelite[ASTEROIDE].cor, c_lua);
    sol->satelite[ASTEROIDE].fr = 0.1;
    sol->satelite[ASTEROIDE].ft = 0;
    sol->satelite[ASTEROIDE].raio_translacao = 0;
    sol->satelite[ASTEROIDE].raio_rotacao = 50000.00;
    sol->satelite[ASTEROIDE].customizar = NULL;
    sol->satelite[ASTEROIDE].especial = true;
    sol->satelite[ASTEROIDE].satelites = 0;
    sol->satelite[ASTEROIDE].desenhar = desenhar_asteroide;
    sol->satelite[ASTEROIDE].ativo = false;
    strncpy(sol->satelite[ASTEROIDE].arquivo, "texturas/asteroide1.bmp", TAM_ARQ);
    sol->satelite[ASTEROIDE].textura = -1;
    sol->satelite[ASTEROIDE].inexistente = true;

    inicia_explosoes(sol);
}

// libera memórias alocadas
void libera_astro(CorpoCeleste *astro)
{
    if (astro->satelite != NULL)
        free(astro->satelite);
    freeExplosion(&(astro->explosao));
    for (int i = 0; i < astro->satelites; i++)
        libera_astro(&(astro->satelite[i]));
}

// ativa (reativa) astros, que foram destruídos (e a explosão já terminou)
// os satélites não são reativados para astros "explodindo"
void ativa(CorpoCeleste *astro)
{
    if (astro->inexistente)
        return;
    if ((astro->ativo = (!inExplosion(astro->explosao))))
        for (int i = 0; i < astro->satelites; i++)
            ativa(&(astro->satelite[i]));
}

// aplica a escala e define as distâncias e raios usados na renderização
void configura_escalas(CorpoCeleste *astro,
        double escalaRaio, double escalaDist)
{
    astro->dist = astro->raio_translacao / escalaDist;
    astro->raio = astro->raio_rotacao / escalaRaio;
    for (int i = 0; i < astro->satelites; i++)
        configura_escalas(&(astro->satelite[i]), escalaRaio, escalaDist);
}
