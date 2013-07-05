/*
 * controle.cpp
 *
 *  Created on: 16/06/2013
 *      Author: Guilherme Sampaio
 */
#include <GL/freeglut.h>

#include <math.h>
#include <float.h>
#include <cmath>

#include "controle.h"
#include "modelo.h"
#include "visao.h"
#include "PickRay.h"
#include "geometria.h"
#include "Texto.h"
#include "Textura.h"
#include "explosion.h"

double dia = 1;
double tempo = 0.1;

//#define M_PI ((double)3.14159265358979323846)

double cameraLongitude = 0;
double cameraLatitude = 35;
double distCamera = DIST_CAMERA;
double direcaoCameraX = 0.0;
double direcaoCameraY = 0.0;
double direcaoCameraZ = 0.0;

int botaoMouse, estadoBotaoMouse;
int posXselecionada, posYselecionada;
int ultima_posMouseX, ultima_posMouseY;
int largura_tela, altura_tela;
int posXmouse, posYmouse;
int id_roda_mouse = 0;
int altura, largura;

bool parado = false;
bool desenhaOrbita = true;
bool desenhaTextura = true;
bool apagado = false;

double escalaRaio = ESCALA_PADRAO_RAIO;
double escalaDist = ESCALA_PADRAO_DIST;

double afastMinimo = 0.02;
double limiteMinimo = afastMinimo / 2.0;
double limiteVolume = 100000.0;
double rotacao, angulo = 10.0;
double cameraEmX = 1.0, cameraEmY = 1.0, cameraEmZ = 5.0;

// reta na forma r(x,y,z) formada por dois pontos
double reta_camera[2][3]; // pontos da reta de projeção da camera
double reta_asteroide[2][3];  // trajetória reta do asteriode

// controles do asteróide "assassino"
bool asteroideMovendo = false;
double asteroide_vetor[3];
double asteroide_pos[3];
double asteroide_vetUnitario[3];
double asteroide_desde;
double tempo_asteroide = limiteVolume / 50;
double trajeto_asteroide = limiteVolume / 2;

CorpoCeleste sol;
CorpoCeleste *asteroide, *em_foco;

int explosoes = 0;
int easter_egg = -1;

#define ASTEROID    0
#define SELECAO     1
#define SOBRE       2
#define TIMER_INTERVAL 10

#define TEMPO_MIN   0.001
#define TEMPO_MAX   1000.00

#define PRECISAO_MINIMA  2.0
#define PRECISAO_MAXIMA  0.1

//----------------------------------------------------------------------------
// Controles de tempo de relógio (FPS - Frames Por Segundo)
//----------------------------------------------------------------------------
int fps = 0;
int frameCount = 0;
int currentTime = 0;
int previousTime = 0;
int timeInterval = 0;

int calculaFPS()
{
    //  Increase frame count
    frameCount++;
    //  Get the number of milliseconds since glutInit called
    //  (or first call to glutGet(GLUT ELAPSED TIME)).
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    //  Calculate time passed
    timeInterval = currentTime - previousTime;
    if(timeInterval > 1000)
    {
        //  calculate the number of frames per second
        fps = (frameCount * 1000) / timeInterval;
        //  Set time
        previousTime = currentTime;
        //  Reset frame count
        frameCount = 0;
    }
    return fps;
}


//----------------------------------------------------------------------------
// Controles de texto
//----------------------------------------------------------------------------
///---------------------------------------------------------
    int cinza[3] = {136, 136, 136};
    int amarelo[34] = {217, 217, 0};
    int cinzaEscuro[3] = {119, 119, 119};
    int cinzaMaisEscuro[3] = {100, 100, 100};
    int vermelho[3] = {255, 0, 0};
    int verde[3] = {0, 255, 0};
    int laranja[3] = {255, 165, 0};
    int branco[3] = {255, 255, 255};
///----------------------------------------------------------
    Texto t_escalas = Texto();
    Texto t_asteroide = Texto();
    Texto t_tempo = Texto();
    Texto t_explosao = Texto();
    Texto t_msg = Texto();
    Texto t_fps = Texto();
    Texto t_foco = Texto();
    Texto t_selecionado = Texto();

void inicia_textos()
{
    t_msg.setColor(verde);
    t_foco.setColor(verde);
    t_explosao.setColor(vermelho);
    t_fps.setColor(amarelo);

    t_escalas.setColor(verde);
    t_asteroide.setColor(verde);
    t_tempo.setColor(verde);
    t_msg.setColor(verde);
    t_selecionado.setColor(verde);

    t_foco.setPosition(-0.98, 0.95, 0);
    t_selecionado.setPosition(-0.98, 0.80, 0);
    t_asteroide.setPosition(-0.98, 0.65, 0);
    t_msg.setPosition(-0.98, 0.50, 0);
    t_explosao.setPosition(0.0, 0.95, 0);
    t_tempo.setPosition(-0.999, -0.85, 0);
    t_escalas.setPosition(-0.999, -0.95, 0);
    t_fps.setPosition(0.80, 0.95, 0);
    t_fps.setText("FPS:...");
}


int ultima_msg = 0;

void mensagem(char *msg)
{
    ultima_msg = currentTime;
    t_msg.setText(msg);
}

void mostra_mensagem()
{
    int agora = currentTime;
    if ((agora - ultima_msg) < 3000)
        t_msg.draw2D();
}

int  ultimo_selec_em = 0;
bool fez_selecao = false;
char *ultimo_selec = NULL;

void informa_selecao(char *texto)
{
    ultimo_selec_em = currentTime;
    fez_selecao = true;
    ultimo_selec = texto;
}

void mostra_selecao()
{
    int agora = currentTime;
    if ((agora - ultimo_selec_em) < 3000) {
        if (fez_selecao) {
            t_selecionado.setColor(amarelo);
            t_selecionado.setText("Selecionado: %s", ultimo_selec);
            fez_selecao = false;
        } else {
            t_selecionado.setColor(verde);
            t_selecionado.setText("Foi selecionado: %s", ultimo_selec);
        }
        t_selecionado.draw2D();
    }
}

//----------------------------------------------------------------------------
// Controles de textura
//----------------------------------------------------------------------------
void carregar_textura(CorpoCeleste *astro)
{
    if (astro->arquivo != NULL)
        astro->textura = LoadBitmapTexture(astro->arquivo);
    int i;
    for (i = 0; i < astro->satelites; i++)
        carregar_textura(&(astro->satelite[i]));

}

void mudar_textura(CorpoCeleste *astro, char *arq)
{
    if (astro->textura >= 0)
        astro->textura = LoadBitmapTexture(arq, astro->textura);
    else
        astro->textura = LoadBitmapTexture(arq);
}

void restaurar_textura(CorpoCeleste *astro)
{
    if (astro->textura >= 0)
        astro->textura = LoadBitmapTexture(astro->arquivo, astro->textura);
    else
        astro->textura = LoadBitmapTexture(astro->arquivo);
}

//----------------------------------------------------------------------------
// controles de atualização e comandos
//----------------------------------------------------------------------------
void alterna_luz()
{
    apagado = !apagado;
    // Habilita/desabilita a luz de número 0
    if (apagado) {
        glDisable(GL_LIGHT0);
    } else {
        glEnable(GL_LIGHT0); // Usa iluminação na luz/lâmpada 0
    }
    glutPostRedisplay();
}

//---------------------------------------------------------------------------
// controles de movimentos relacionados com tempo
//---------------------------------------------------------------------------
double angulo_translacao(CorpoCeleste *a)
{
    return (fmod((double)(dia * a->ft * 360.0), 360));
}

double angulo_rotacao (CorpoCeleste *a)
{
    return (fmod((double)(dia * a->fr * 360.0), 360));
}

double angulo_translacao(CorpoCeleste *a, double dia)
{
    return (fmod((double)(dia * a->ft * 360.0), 360));
}

double angulo_rotacao (CorpoCeleste *a, double dia)
{
    return (fmod((double)(dia * a->fr * 360.0), 360));
}

// controle de mudança de tempo
void mult_tempo(double fator)
{
    static bool explica = false;
    double pre_tempo = tempo * fator;
    if (fator > 1.0 && pre_tempo > 1.0 && asteroide->ativo) {
        if(explica)
            mensagem((char *)"Espere o fim ou termine o asteroide...");
        else
            mensagem((char *)"Impossivel acelerar o tempo, asteroide em movimento...");
        explica = !explica;
        return;
    }
    tempo = pre_tempo;;
    if (tempo < TEMPO_MIN)
        tempo = TEMPO_MIN;
    // cuidado: tempo muito acelerado (como este definido aqui como máximo)
    // gera distorções em alguns controles como, por exemplo, colisão
    // sé é util para bricar e testar a translação secular de júpiter
    else if (tempo > TEMPO_MAX)
        tempo = TEMPO_MAX;
    glutPostRedisplay();
}

//----------------------------------------------------------------------------
// controles usados externamente (protegidos pelas funções)
//----------------------------------------------------------------------------
double get_dia() { return dia;}

bool desenha_textura() { return desenhaTextura;}

bool desenha_orbita() {return desenhaOrbita;}

CorpoCeleste *get_em_foco() { return em_foco; }

void posicao_camera(GLdouble pos[])
{
    pos[0] = cameraEmX;
    pos[1] = cameraEmY;
    pos[2] = cameraEmZ;
}

//----------------------------------------------------------------------------
// Controles da câmera
//----------------------------------------------------------------------------
void dec_distancia(double d)
{
    double dist = distCamera;
    distCamera -= d;
    // cuidado, a distância não pode ser negativa e nem deve ser zero!
    // menor e maior distância
    if (distCamera < (afastMinimo + (em_foco->raio * 1.1))) {
        distCamera = (afastMinimo + (em_foco->raio * 1.1));
    } else if (distCamera > trajeto_asteroide) {
        distCamera = trajeto_asteroide;
    }
    // se mudou, atualiza a camera
    if (dist != distCamera)
        glutPostRedisplay();
}

void mudar_foco_para(CorpoCeleste* astro)
{
    //if (em_foco != astro) {
        if (astro->ativo) {
            em_foco = astro;
            distCamera = em_foco->raio * 5;
            dec_distancia(0);
        } else if (!em_foco->ativo){
            em_foco = &sol;
            distCamera = em_foco->raio * 5;
            dec_distancia(0);
        }
    //}
}

void pos_camera(double longitude, double latitude)
{
    cameraLongitude = fmod(longitude, 360.0);
    cameraLatitude = fmod(latitude, 360);
    double raioLatitude = cos(cameraLatitude * (M_PI/180)) * distCamera;
    cameraEmX = sin(cameraLongitude*(M_PI/180))*raioLatitude + direcaoCameraX;
    cameraEmY = sin(cameraLatitude*(M_PI/180))*distCamera + direcaoCameraY;
    cameraEmZ = cos(cameraLongitude*(M_PI/180))*raioLatitude + direcaoCameraZ;
}

void atualiza_camera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    pos_camera(cameraLongitude, cameraLatitude);
    gluLookAt(cameraEmX, cameraEmY, cameraEmZ,
            direcaoCameraX, direcaoCameraY, direcaoCameraZ, 0.0, 1.0, 0.0);

    // limpa o atual buffer de renderização
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------------------------
// controle de escalas
//----------------------------------------------------------------------------
bool bloqueia_escala()
{
    static bool explica = false;
    if (explosoes || asteroide->ativo) {
        if(explica)
            mensagem((char *)"Impossivel redimensionar...");
        else {
            if (explosoes)
                mensagem((char *)"Espere a explosao ou acelere o tempo...");
            else
                mensagem((char *)"Espere ou termine o asteroide...");
        }
        explica = !explica;
        return true;
    }
    return false;
}

void mudar_escalas(double eD, double eR)
{
    if (bloqueia_escala()) return;
    escalaDist = eD;
    escalaRaio = eR;
    distCamera = DIST_CAMERA;
    posicao_inicial();
    dec_distancia(0);
    glutPostRedisplay();
}

void mult_escalaRaio(double fator)
{
    if(bloqueia_escala()) return;
    // não permite mudança de escala planetária
    // quando a escala espacial não for máxima
    if(escalaDist < ESCALA_PADRAO_DIST_MAX) {
        mensagem((char *)"Ajuste para a maxima escala espacial antes!");
        return;
    }
    // varia a escala dentro dos limites
    double novaEscala = escalaRaio * fator;
    if (novaEscala < ESCALA_PADRAO_RAIO_MIN)
        novaEscala = ESCALA_PADRAO_RAIO_MIN;
    else if (novaEscala > ESCALA_PADRAO_RAIO_MAX)
        novaEscala = ESCALA_PADRAO_RAIO_MAX;
    if (escalaRaio != novaEscala) {
        escalaRaio = novaEscala;
        configura_escalas(&sol, escalaRaio, escalaDist);
        dec_distancia(0);
        glutPostRedisplay();
    }
}

void mult_escalaDist(double fator)
{
    if (bloqueia_escala()) return;
    double novaEscala = escalaDist * fator;
    // ajsuta a escala planetária para o mínimo
    // isso tenta garantir precisão e coerência nas dimensões
    escalaRaio = ESCALA_PADRAO_RAIO_MIN;
    // varia a escala dentro dos limites
    if (novaEscala < ESCALA_PADRAO_DIST_MIN)
        novaEscala = ESCALA_PADRAO_DIST_MIN;
    else if (novaEscala > ESCALA_PADRAO_DIST_MAX)
        novaEscala = ESCALA_PADRAO_DIST_MAX;
    if (escalaDist != novaEscala) {
        escalaDist = novaEscala;
        configura_escalas(&sol, escalaRaio, escalaDist);
        dec_distancia(0);
        glutPostRedisplay();
    }

}

// configuração de controles como mudança de escala e outros
void posicao_inicial()
{
    mudar_foco_para(&sol);
    configura_escalas(&sol, escalaRaio, escalaDist);
    cameraEmX = sol.raio * 2;
    cameraEmY = 0;
    cameraEmZ = 0;
}

// aproximação ou afastamento da câmera
void zoom(int direcao)
{
    // varia a "velocidade" de zoom de acordo com a distância
    if (distCamera > 100)
        dec_distancia(direcao * 50);
    else
        if (distCamera > 10)
            dec_distancia(direcao);
        else
            if (distCamera > 1)
                dec_distancia(direcao * 0.1);
            else
                dec_distancia(direcao * 0.01);
}

// calcula a precisão do desenho das óbitas para acelerar a renderização:
// aumenta a prescisão nos casos de distância longa ou planeta muito pequeno
double precisao_orbita()
{
    double precisao;
    // se a escala do raio está fixada, então só é possível variar a distância:
    // a precisao aumenta com o aumento das distâncias
    if (escalaRaio == ESCALA_PADRAO_RAIO_MIN)
        precisao = (escalaDist / ESCALA_PADRAO_DIST_MIN) * PRECISAO_MAXIMA;
    else
        // senão, o fator importante é o raio:
        // a precisão aumenta com a redução dos planetas
        precisao = (ESCALA_PADRAO_RAIO_MAX / escalaRaio) * PRECISAO_MAXIMA;
    // verifica a precisão mínima
    if (precisao > PRECISAO_MINIMA) precisao = PRECISAO_MINIMA;
    // a precisão máxima é derivada dos cálculos
    return precisao;
}

//----------------------------------------------------------------------------
// controle do asteróide "assassino"
//----------------------------------------------------------------------------
void ativa_asteroide()
{
    if (tempo > 10) {
        mensagem ((char *)"Muito acelerado para movimentar asteroide...");
        return;
    }
    // calcula a trajetórioa linear do asteroide
    UnProject(reta_asteroide[0], reta_asteroide[1], posXmouse, posYmouse);
    // define o vetor de deslocamento da reta da trajetória do asteroide
    vetor(asteroide_vetor, reta_asteroide[0], reta_asteroide[1]);
    // dividide o vetor pelo tempo de vida do asteroide para que ele percorra
    // no máximo os limites da visão (zFar do gluPerspective)
    multPEsc(asteroide_vetor,  1.0/tempo_asteroide);
    // define a posição inicial do asteroide como a da câmera
    posicao_camera(asteroide_pos);
    // adianta um pouco o asteroide da câmera
    // extamente uma unidade de distancia por tempo
    //somaEsc(asteroide_pos, asteroide_vetor);
    // copia a posição inicial para o asteroide
    cpVet(asteroide->pos, asteroide_pos);
    // deixa o asteroide ativo mas espera o disparo
    asteroide->ativo = true;
    asteroideMovendo = false;
    asteroide->inexistente = false;
}

void desativa_asteroide(CorpoCeleste *foco)
{
    asteroideMovendo = false;
    asteroide->ativo = false;
    asteroide->inexistente = true;
    if (em_foco == asteroide)
        mudar_foco_para(foco);
}

void dispara_asteroide()
{
    // se o asteroide não estiver ativo, ativa no momento
    if (!asteroide->ativo)
        ativa_asteroide();
    // se o asteroide já estiver se movendo, termina a animação
    if (asteroideMovendo) {
        desativa_asteroide(&sol);
    } else {
        // define o momento desde que o asteroide se iniciou o movimento
        asteroide_desde = dia - tempo;
        asteroide->ativo = true;
        asteroideMovendo = true;
    }
}

void orbita_asteroide(double Q[3], double R[3])
{
    Q[0] = reta_asteroide[0][0];
    Q[1] = reta_asteroide[0][1];
    Q[2] = reta_asteroide[0][2];
    R[0] = reta_asteroide[1][0];
    R[1] = reta_asteroide[1][1];
    R[2] = reta_asteroide[1][2];
}

void movimenta_asteroide()
{
    // controla as falhas de movimento
    static int falhas = 0;
    // se o asteroide está movimentando, faz o cálculo do movimento...
    if (asteroideMovendo) {
        // faz o deslocamento proposcional à velocidade e tempo
        double deltaDia = dia - asteroide_desde;
        double pos[3];
        pos[0] = asteroide_pos[0] + asteroide_vetor[0] * deltaDia;
        pos[1] = asteroide_pos[1] + asteroide_vetor[1] * deltaDia;
        pos[2] = asteroide_pos[2] + asteroide_vetor[2] * deltaDia;
        // mantém o deslocamento do asteroide em distância máxima
        // para evitar falha na análise de colisão
        double dist_percorrida = distPonto(asteroide->pos, pos);
        double precisao = 2 * asteroide->raio;
        if (dist_percorrida > precisao) {
            // após algumas falhas (comum no início),
            // reduz a aceleração do tempo
            if (++falhas > 3) {
                mensagem((char *)"Reduzindo a aceleracao do tempo por causa do asteroide...");
                if (tempo > 1)
                    tempo = 1;
                else
                    tempo *= 0.95;
                // se atingir tempo mínimo, desativa o asteroide
                if (tempo < TEMPO_MIN) {
                    mensagem((char *)"Desativando movimentacao do asteroide! Notifque o problema...");
                    tempo = TEMPO_MIN;
                    desativa_asteroide(&sol);
                }
            }
        } else {
            falhas = 0;
        }
        cpVet(asteroide->pos, pos);
        // se o asteroide percorreu toda a área de visão, pode desativá-lo
        dist_percorrida = distPonto(asteroide->pos, asteroide_pos);
        if (dist_percorrida > trajeto_asteroide)
            desativa_asteroide(&sol);
    }

}

/*----------------------------------------------------------------------------
// controles de seleção por posição do mouse na janela (x,y)
//----------------------------------------------------------------------------
 Seleção considerando todas as esferas (SOL, planetas e lua)
 e a distância delas em relação à camera
 Agradecimento especial no caso ao "Tio Fred" pela ajuda na matemática e
 na verificação da colisão com os anéis de saturno (pedido no trabalho)

 Ref: Interactive Techniques in Three-dimensional Scenes (Part 1):
      Moving 3D Objects with the Mouse using OpenGL 2.1
      http://www.codeproject.com/Articles/35139/
      Interactive-Techniques-in-Three-dimensional-Scenes
      By Steve Katic, 22 Apr 2009

 Ref: Select + Mouse + OpenGL
      http://www.bfilipek.com/2012/06/select-mouse-opengl.html
      Bartlomiej Filipek, 15 June 2012
*/
void _mais_proximo_da_camera(CorpoCeleste *astro,
        double *minDist, CorpoCeleste **mais_perto)
{
    double desloc, prox[3];
    pontoMaisProximo(prox, reta_camera[0], reta_camera[1], astro->pos, &desloc);
    // se o deslocamento está dentro do espaço definido pelo segmento da reta
    if (desloc >= 0 && desloc <= 1) {
        double dist = distPonto(prox, astro->pos);
        // se a distância é menor que o raio do objeto (são esféricos)
        if (dist <= astro->raio) {
            // verifica se a câmera não está dentro do astro
            double dist3 = distPonto(astro->pos, reta_camera[0]);
            if (dist3 > astro->raio) {
                // verifica se o objeto é mais próximo que o atual selecionado
                if (dist3 < *minDist) {
                    // se estiver, muda a seleção
                    *mais_perto = astro;
                    *minDist = dist;
                }
            }
        }
    }
    // para cada satélite
    int i;
    for (i = 0; i < astro->satelites; i++) {
        _mais_proximo_da_camera(&(astro->satelite[i]), minDist, mais_perto);
    }
}

// Pendente
bool verifica_selecao_anel_saturno(CorpoCeleste *astro, double *dist)
{
    Plano p;
    p.n[0] = 0.0;
    p.n[1] = 1.0;
    p.n[2] = 0.0;
    p.d = 0.0;
    double desloc, q[3];
    double raio_anel = astro->raio * FATOR_ANEL_SATURNO;
    if(interceptaSegmentoPlano(reta_camera[0], reta_camera[1], p, desloc, q)) {
        if (distPonto(astro->pos, q) < raio_anel) {
            *dist = distPonto(reta_camera[0], q) - afastMinimo;
            return true;
        }
    }
    return false;
}


// seleção considerando todoas as esferas (SOL, planetas e lua)
// e a distância delas em relação à camera
CorpoCeleste *seleciona()
{
    double dist = DBL_MAX;
    CorpoCeleste *selecionado = NULL;
    _mais_proximo_da_camera(&sol, &dist, &selecionado);
    // verifica o anel de saturno
    double distAS;
    if (verifica_selecao_anel_saturno(&(sol.satelite[SATURNO]), &distAS)) {
        if(distAS < dist) {
            informa_selecao((char *)"Anel de saturno");
            return &(sol.satelite[SATURNO]);
        }
    }
    // ajusta o label de seleção de planeta
    if (selecionado != NULL)
        informa_selecao(selecionado->nome);

    return selecionado;
}

//-----------------------------------------------------------------------------
// controles de colisão e explosão
//-----------------------------------------------------------------------------
// desativa um astro e seus satélites
void desativa_astro(CorpoCeleste *astro)
{
    astro->ativo = false;
    for (int i = 0; i < astro->satelites; i++)
        desativa_astro(&(astro->satelite[i]));
}

void explode(CorpoCeleste *astro)
{
    desativa_asteroide(astro);
    if (astro != &sol) {
        desativa_astro(astro);
        radiusExplosion(astro->explosao, astro->raio);
        initExplosion(&(astro->explosao));
    }
}

//
CorpoCeleste *verifica_colisao(CorpoCeleste *astro, CorpoCeleste *asteroide)
{
    // se for o asteroide, não deve verificar, pois ele colide com ele mesmo!
    if (astro == asteroide)
        return NULL;

    double dist = distPonto(astro->pos, asteroide->pos);

    // ajsuta a distância de colisão pelo mínimo entre o deslocamento por
    // quadro ou o raio do asteroide

    // se a distância é menor que os raios dos planetas (são esféricos)
    if (dist <= (astro->raio + asteroide->raio) && astro->ativo) {
        // Tragédia! Catástrofe! Colisão...
        explode(astro);
        return astro;
    }
    // para cada satélite
    int i;
    for (i = 0; i < astro->satelites; i++) {
        verifica_colisao(&(astro->satelite[i]), asteroide);
    }
    return NULL;
}

// Agradecimento especial no caso ao "Tio Fred" pela ajuda na matemática
bool verifica_colisao_anel_saturno(CorpoCeleste *astro)
{
    double raio_anel = sol.satelite[SATURNO].raio * FATOR_ANEL_SATURNO;
    // plano no qual o anel é desenhado
    //!!! no caso, pode ser dispensada (pois o plano é perpendicular ao eixo Y)
    //double plano[4];
    //plano[0] = 0;
    //plano[1] = 1;
    //plano[2] = 0;
    //plano[3] = 0;
    // normal do plano
    //!!! no caso é simplesmente 1, pode ignorar
    //---double normaPlano = normaVet(plano);
    // projeção do asteroide no plano
    //!!! no caso, pode simplificar para: projecao = fabs(astro->pos[1]);
    //---double projecao = fabs(astro->pos[0]*plano[0] +
    //        astro->pos[1]*plano[1] + astro->pos[2]*plano[2] + plano[3] );
    //Verifica se o asteroide intercepta o plano
    //!!! pode simplificar!!!
    //---if (projecao/normaPlano < astro->raio)
    double dist = fabs(astro->pos[1]);
    if( dist < astro->raio) {
        // define o ponto de projeção do centro do asteróide no plano
        double pp[3];
        pp[0] = astro->pos[0];
        pp[1] = 0;
        pp[2] = astro->pos[2];
        // calcula o raio do círculo projeto pela esfera no plano
        double raioProj = sqrt(pow(astro->raio, 2)+pow(dist,2));
        // verifica se a distância entre o centro do cúrculo projetado
        // e o centro de saturno for menor que a soma dos raios
        if (distPonto(pp, sol.satelite[SATURNO].pos) <
                raioProj + raio_anel) {
            iniciar_colapso_anel_saturno();
        }
    }
    return false;
}

void continua_explosao(CorpoCeleste *astro)
{
    if (inExplosion(astro->explosao))
        idleExplosion(&(astro->explosao), tempo);
    for (int i = 0; i < astro->satelites; i++)
        continua_explosao(&(astro->satelite[i]));
}

void colisao()
{
    CorpoCeleste *colide_com = verifica_colisao(&sol, asteroide);
    if (colide_com == NULL && !em_colapso_anel_saturno())
        verifica_colisao_anel_saturno(asteroide);
}

//----------------------------------------------------------------------------
// controles do GLUT
//----------------------------------------------------------------------------
CorpoCeleste *selecionado = NULL;

void calcular()
{
    //calcula os frames por segundo
    calculaFPS();

    // faz a passagem do tempo
    if(!parado) {
        // o universo ficou velho demais?
        if (dia > FLT_MAX - tempo) {
            // renasce o sistema solar no dia 1
            dia = 1;
            mensagem((char *)"Novo universo!");
        }
        else
            // se estamos na idade do universo válida, adiciona o tempo
            dia += tempo;
        movimenta_asteroide();
        // trata movimentos das explosões
        resumeExplosion();
        continua_explosao(&sol);
        explosoes = countExplosions();
    }

    // Faz desenho sem renderização
    // apenas para cálculo da posição do astro em foco.
    // Na mudança de tempo, o astro ficará em posição diferente
    // do quadro em que ele foi desenhado anteriomente.
    // Por isso, é feita uma passagem na construção estruturada da cena
    // para calcular sua posição nesse novo quadro que representa o "dia" atual
    renderizar(false);
    desenhar_sol(&sol);
    renderizar(true);

    // define a posição da camera em relação ao astro em foco
    direcaoCameraX = em_foco->pos[0];
    direcaoCameraY = em_foco->pos[1];
    direcaoCameraZ = em_foco->pos[2];

    // infoma o dia
    t_tempo.setText("Dia: %f (%f)\n", dia, tempo);

    // informa as escalas atuais
    t_escalas.setText("Escalas: Espacial=1/%.2f, Planeta=1/%.2f",
            escalaDist, escalaRaio);

    // lida com o asteroide
    if (asteroide->ativo) {
        // informa a atividade
        if (asteroideMovendo)
            t_asteroide.setText("Asteroide!");
        else
            t_asteroide.setText("Asteroide...");
        // verifica a colisão com outros astros
        colisao();
    } else {
        t_asteroide.setText("");
    }

    // informa explosões
    if (explosoes)
        t_explosao.setText("Catrastrofe (%d)!", explosoes);
    else
        t_explosao.setText("");
    // informa FPS
    t_fps.setText("FPS: %d", fps);

    // informa astro que está no foco da câmera
    if (em_foco->ativo) {
        t_foco.setText("Em foco: %s", em_foco->nome);
        t_foco.setColor(verde);
    } else {
        t_foco.setText("Explodido: %s", em_foco->nome);
        t_foco.setColor(laranja);
    }

    glutPostRedisplay();
}

void desenhar()
{
    //********************************************************
    // Etapa de desenho efetivo
    //********************************************************
    if (!apagado)
        glEnable(GL_LIGHT0);

    // posiciona a câmera
    atualiza_camera();
    // verifica a inversibilidade da matriz de modelo e, se for inversível,
    // guarda a matriz inversa par calculos futuros
    if (!initPickRayDepth()) {
        mensagem((char *)"Impossivel fazer controle de posicionamento. Favor termine o programa!");
        return;
    }
    // obtem a reta de seleção (pick ray)
    // guarda a posição do mouse nesse momento de seleção
    ultima_posMouseX = posXmouse;
    ultima_posMouseY = posYmouse;
    UnProject(reta_camera[0], reta_camera[1], ultima_posMouseX, ultima_posMouseY);
    // desenha
    desenhar_sol(&sol);
    selecionado = seleciona();

    // desenha os textos
    t_tempo.draw2D();
    t_escalas.draw2D();
    t_asteroide.draw2D();
    t_explosao.draw2D();
    t_fps.draw2D();
    t_foco.draw2D();
    mostra_selecao();
    mostra_mensagem();

    // desenha a posição do mouse
    desenhar_mouse_cursor(ultima_posMouseX, ultima_posMouseY,
            largura_tela, altura_tela, 12.0);

    glutSwapBuffers();
}

void remodelar(int w, int h)
{
    double angulo = 60.0;
    largura_tela = w;
    altura_tela = h;
    double perspectiva = (double)(w)/(double)(h);
    glViewport(0, 0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angulo, perspectiva, limiteMinimo, limiteVolume);
    glutPostRedisplay();
}

void teclado (unsigned char key, int x, int y)
{
    posXmouse = x;
    posYmouse = y;
    switch (key) {
        case 'l':
        case 'L':
            alterna_luz();
            break;
        case 'p':
        case 'P':
            parado = !parado;
            glutPostRedisplay();
            break;
        case 'q':
        case 'Q':
            mult_escalaRaio(0.5);
            break;
        case 'a':
        case 'A':
            mult_escalaRaio(1.5);
            break;
        case 'w':
        case 'W':
            mult_escalaDist(0.5);
            break;
        case 's':
        case 'S':
            mult_escalaDist(1.5);
            break;
        case 'z':
        case 'Z':
            mudar_escalas(ESCALA_PADRAO_DIST, ESCALA_PADRAO_RAIO);
            break;
        case 'x':
        case 'X':
            mudar_escalas(ESCALA_PADRAO_DIST_MAX, ESCALA_PADRAO_RAIO_MIN);
            break;
        case 'o':
        case 'O':
            desenhaOrbita = !(desenhaOrbita);
            glutPostRedisplay();
            break;
        case 'r':
        case 'R':
            ativa(&sol);
            terminar_colapso_anel_saturno();
            glutPostRedisplay();
            break;
        case 'e':
        case 'E':
            ativa_asteroide();
            glutPostRedisplay();
            break;
        case 'd':
        case 'D':
            if(asteroide->ativo)
                dispara_asteroide();
            glutPostRedisplay();
            break;
        case 'c':
        case 'C':
            desativa_asteroide(&sol);
            glutPostRedisplay();
            break;
        case 't':
        case 'T':
            desenhaTextura = !desenhaTextura;
            glutPostRedisplay();
            break;
        case '1':
            mudar_foco_para(&sol);
            break;
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (sol.satelite[key - '2'].ativo)
                mudar_foco_para(&sol.satelite[key - '2']);
            break;
        case '0':
            if (sol.satelite[PLUTAO].ativo)
                mudar_foco_para(&sol.satelite[PLUTAO]);
            break;
        case '-':
        case '_':
            mudar_foco_para(asteroide);
            break;
        case '!':
            mudar_foco_para(&(sol.satelite[TERRA].satelite[LUA]));
            break;
        case '@':
            mudar_foco_para(&(sol.satelite[SATURNO].satelite[TITAN]));
            break;
        case '#':
            mudar_foco_para(&(sol.satelite[SATURNO].satelite[FEBE]));
            break;
        case '$':
            mudar_foco_para(&(sol.satelite[SATURNO].satelite[HIPERION]));
            break;
        case 'j':
        case 'J':
            exit(1);
            break;
        default:
            break;
    }
}


void teclado_extra(int tecla, int x, int y)
{
    posXmouse = x;
    posYmouse = y;
    switch(tecla) {
        case GLUT_KEY_UP: /* Seta para Cima */
            zoom(1);
            break;
        case GLUT_KEY_DOWN: /* Seta para Baixo */
            zoom(-1);
            break;
        case GLUT_KEY_RIGHT: /* Seta para Direita */
            mult_tempo(1.1);
            break;
        case GLUT_KEY_LEFT: /* Seta para Esquerda */
            mult_tempo(0.9);
            break;
        default:
            break;
    }
}

void move_camera(int x, int y)
{
    double dx = (double)(x - posXselecionada);
    double dy = (double)(y - posYselecionada);
    cameraLongitude -= (dx * 0.56);
    cameraLatitude += (dy * 0.56);
    posXselecionada = x;
    posYselecionada = y;
    glutPostRedisplay();
}

void move_mouse(int x, int y)
{
    posXmouse = x;
    posYmouse = y;
    if (botaoMouse == GLUT_LEFT_BUTTON && estadoBotaoMouse == GLUT_DOWN) {
        move_camera(x, y);
    } else if(botaoMouse == GLUT_RIGHT_BUTTON && estadoBotaoMouse == GLUT_DOWN
            && asteroide->ativo && !asteroideMovendo) {
        move_camera(x, y);
    }
}

void posiciona_mouse(int x, int y)
{
    posXmouse = x;
    posYmouse = y;
}

void roda_mouse(int wheel, int direction, int x, int y)
{
    posXmouse = x;
    posYmouse = y;
    id_roda_mouse = wheel;
    zoom(direction);
}

void mouse(int button, int state, int x, int y)
{
    posXmouse = x;
    posYmouse = y;
    botaoMouse = button;
    estadoBotaoMouse = state;
    if(button == GLUT_LEFT_BUTTON) {
        if(state == GLUT_DOWN) {
            posXselecionada = x;
            posYselecionada = y;
        } else if(state == GLUT_UP) {
        }
    } else if(button == GLUT_RIGHT_BUTTON) {
        if(state == GLUT_DOWN) {
            posXselecionada = x;
            posYselecionada = y;
            ativa_asteroide();
        } else if(state == GLUT_UP) {
            if (asteroide->ativo)
                dispara_asteroide();
        }
    } else if(button == GLUT_MIDDLE_BUTTON) {
        static bool egg = false;
        static int textura = 0;
        egg = !egg;
        if (egg) {
            textura = sol.textura;
            sol.textura = easter_egg;
        }
        else
            sol.textura = textura;
    }
}

//----------------------------------------------------------------------------
// inicialização dos controles
//----------------------------------------------------------------------------
void inicia_controle()
{
    inicia_sistema_solar(&sol);
    asteroide = &(sol.satelite[ASTEROIDE]);
    carregar_textura(&sol);
    configura_saturno(&sol);
    configura_visao();
    posicao_inicial();
    inicia_textos();
    // define explosão com feito "fireball"
    fireballExplosion();
    mudar_foco_para(&sol);
    easter_egg = LoadBitmapTexture((char *)("texturas/by.bmp"));

    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    glutDisplayFunc(desenhar);
    glutIdleFunc(calcular);
    glutReshapeFunc(remodelar);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(teclado_extra);

    glutMouseFunc(mouse);
    glutMotionFunc(move_mouse);
    glutPassiveMotionFunc(posiciona_mouse);
    glutMouseWheelFunc(roda_mouse);

}

