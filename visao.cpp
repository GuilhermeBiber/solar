/*
 * visao.cpp
 *
 *  Created on: 16/06/2013
 *      Author: Guilherme Sampaio
 */
#include <GL/glut.h>

#include <float.h>
#include <math.h>
#include <string.h>

#include "visao.h"
#include "controle.h"
#include "PickRay.h"
#include "controle.h"
#include "explosion.h"
#include "Textura.h"

//---------------------------------------------------------------------------

typedef struct Mov_hip {
    double dtransl,
            drot,
            raioT;
} Mov_hip;

//typedef double Cor[3];

GLUquadricObj *quad;
unsigned char Stipple2[128] = {
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA,
          0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA};


unsigned char Stipple1[128] = {
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44,
          0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44};

GLubyte stipple1[128];
GLubyte stipple2[128];

GLushort lineStipple = 0x8888;

bool deve_renderizar = true;

#define RENDER(x)  {if(deve_renderizar) x;}
#define CALCULO(x) {if(!deve_renderizar) x;}

void renderizar(bool m) {deve_renderizar = m;}
bool get_renderizar() { return deve_renderizar;}

//---------------------------------------------------------------------------


Mov_hip omov_hip, vmov_hip, dmov_hip, mmov_hip, mov_hip;
double dia_anterior;

void inicia_movimento_aleatorio_hiperion()
{
    // controle do movimento aleatório de Hiperion
    dia_anterior = get_dia();

    omov_hip.dtransl = 0.1;
    omov_hip.drot = 0.0;
    omov_hip.raioT = 1.0;

    mov_hip.dtransl = omov_hip.dtransl;
    mov_hip.drot = omov_hip.drot;
    mov_hip.raioT = omov_hip.raioT;

    vmov_hip.dtransl  = omov_hip.dtransl;
    vmov_hip.drot  = omov_hip.drot;
    vmov_hip.raioT = omov_hip.raioT;

    mmov_hip.dtransl  = omov_hip.dtransl * 3.0;
    mmov_hip.drot  = 90.0;
    mmov_hip.raioT = omov_hip.raioT * 2.0;

    dmov_hip.dtransl  = 0.05; //1.0/1000.0;
    dmov_hip.drot  = 1.0; //1.0/100000.0;
    dmov_hip.raioT = 0.03; //100000.0;
}

void __varia__(double *var, double *val,
        double deltaf, double min, double max, double tempo)
{
    // varia a aceleração de forma aleatória e em pequenos incrementos
    double delta = ((double)rand()/RAND_MAX) * deltaf * tempo;
    // se os valores atuais forem os sorteados, faz novo sorteio
    if (*var == *val) {
        // ou escolhe o máximo menos uma quantidade aleatória
        // ou o mínimo mais uma quantidade aleatória
        if (rand() % 2)
            *val = max - (((double)rand()/RAND_MAX) * min);
        else {
            double plus = (((double)rand()/RAND_MAX) * min);
            if (min + plus > max)
                *val = max;
            else
                *val = min + plus;
        }
    }
    // se o valor for maior que o sorteado, decrementa
    if (*var > *val) {
        *var -= delta;
        if (*var < *val)
            *var = *val;
    } else
        // se o valor for menor que o sorteado, incrementa
        if (*var < *val) {
        *var += delta;
        if (*var > *val)
            *var = *val;
    }
}

// Faz a variação dos movimentos em forma de fatores crescentes/decrescentes
void movimento_hiperion()
{
    double dia = get_dia();
    double tempo = dia - dia_anterior;
    dia_anterior = dia;
    // faz a variação aleatória dos movimentos
    __varia__(&(mov_hip.dtransl), &(vmov_hip.dtransl),
            dmov_hip.dtransl, omov_hip.dtransl, mmov_hip.dtransl, tempo);
    __varia__(&(mov_hip.drot), &(vmov_hip.drot),
            dmov_hip.drot, omov_hip.drot, mmov_hip.drot, tempo);
    __varia__(&(mov_hip.raioT), &(vmov_hip.raioT),
            dmov_hip.raioT, omov_hip.raioT, mmov_hip.raioT, tempo);
}

void glBegin2D(int l, int a)
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
            //
            glOrtho(0, l, a, 0, -1.0, 1.0);
}
void glEnd2D()
{
        glPopMatrix();
    // Retorna o sistema de coordenadas de proje��o
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    // retonra para a matriz de modelo
    glMatrixMode(GL_MODELVIEW);
}

void desenhar_mouse_cursor(int x, int y, int l, int a, double dimensao)
{
    glBegin2D(l, a);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glTranslated(x, y, 0);
        glBegin(GL_TRIANGLES);
            glColor4f(0.0, 0.75, 0.0, 0.75);
            //glColor3d(0.0, 1.0, 0.0);
            glVertex2f(0, 0);
            glVertex2f(0, sqrt(2*dimensao*dimensao));
            glVertex2f(dimensao, dimensao);
        glEnd();
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
    glEnd2D();
}

// configura as caracteristicas gerais de visualização
void configura_visao() {

    // Especifica que a cor de fundo da janela será preta
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Capacidade de brilho do material
    GLfloat especularidade[4]={1.0, 1.0, 1.0, 1.0};
    GLint especMaterial = 60;

    // Habilita o modelo de colorização de Gouraud
    glShadeModel(GL_SMOOTH);

    // Define a refletância do material
    glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
    // Define a concentração do brilho
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
    //
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Ativa o uso da luz ambiente
    GLfloat luzAmbiente[4]={0.2, 0.2, 0.2, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);

    // Define os parâmetros da luz de número 0 como sendo o SOL
    // Estabelecer/Variar o posicionamento da luz
    GLfloat luzDifusa[4]={0.8, 0.8, 0.8, 1.0}; // "cor"
    GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0}; // "brilho"
    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    // deixa a ativação da luz o para o programa de controle
    //glEnable(GL_LIGHT0);

    // Habilita a definição da cor do material a partir da cor corrente
    // Usar as cores correntes para colorir os materiais perceptíveis no escuro
    glEnable(GL_COLOR_MATERIAL);
    //Habilita o uso de iluminação
    glEnable(GL_LIGHTING); // Declara iluminação
    // Habilita o depth-buffering
    // Faz a luz preencher o buffer de pronfundidade e não só o fundo
    glEnable(GL_DEPTH_TEST);
    // habilita iluminação nas texturas
    // funde a imagem sem textura com a imagem com textura
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // fixa a posição da luz 0
    GLfloat posicaoLuz[4]={0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);
    glDisable(GL_DITHER);
}

// desenha uma orbita
void circuloY(double x, double y, double raio)
{
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    //glEnable (GL_LINE_SMOOTH);
    //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineStipple(1, lineStipple);
    glEnable(GL_LINE_STIPPLE);
    double precisao = precisao_orbita();
    glBegin(GL_LINE_LOOP);
        for(double j = 0; j < 360.0; j += precisao)
        {
            x = raio * cos(j * M_PI/180.0f);
            y = raio * sin(j * M_PI/180.0f);
            glVertex3d(x, 0, y);
        }
    glEnd();
    glPopAttrib();
    glPopMatrix();
}

// desenha uma faixa no planeta para destacar a rotação
void circuloZ(double x, double y, double raio)
{
    glBegin(GL_LINES);
        x = raio * cos(359 * M_PI/180.0f);
        y = raio * sin(359 * M_PI/180.0f);
        for(int j = 0; j < 360; j++)
        {
            glVertex2d(x, y);
            x = raio * cos(j * M_PI/180.0f);
            y = raio * sin(j * M_PI/180.0f);
            glVertex2d(x, y);
        }
    glEnd();
}

// desenha linhas representado os eixos no atual ponto (0,0,0)
void showAxis()
{
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    glColor3d(1, 0, 0);
    glVertex3d(00.0, 00.0, 00.0);
    glVertex3d(1000.0, 00.0, 00.0);
    glColor3d(0, 1, 0);
    glVertex3d(00.0, 00.0, 00.0);
    glVertex3d(00.0, 1000.0, 00.0);
    glColor3d(0, 0, 1);
    glVertex3d(00.0, 00.0, 00.0);
    glVertex3d(00.0, 00.0, 1000.0);
    glEnd();
    glEnable(GL_LIGHTING);
    glPopMatrix();
}

// desenha uma esfera com textura
void desenha_esfera(int idTextura, double raio, int precisao)
{
    glColor3d(1.0, 1.0, 1.0);
    static GLUquadric *qobj = gluNewQuadric();
    gluQuadricTexture(qobj,GL_TRUE);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, idTextura);
    gluSphere(qobj, raio, precisao, precisao);
    glDisable(GL_TEXTURE_2D);
}

// desenha uma esfera sem textura e com reaçe de rotação
void desenha_esfera(CorpoCeleste *astro)
{
    // desenha uma linha para reaçar a rotação
    if (desenha_orbita()) {
        glPushMatrix();
            glRotated(90, 1.0, 0.0, 0.0);
            glColor3d (0.0, 1.0, 0.0);
            circuloZ(0, 0, astro->raio);
        glPopMatrix();
    }
    glColor3d (astro->cor[C_R], astro->cor[C_G], astro->cor[C_B]);
    glutSolidSphere(astro->raio, 30, 30);
}

// desenha uma esfera representando o planeta
void desenhar_corpo_celeste(CorpoCeleste *astro, double aRotacao, int precisao)
{
    // se o astro está ativo, desenha com ou sem textura
    // se não está, verifica e trata explosão
    if (astro->ativo)
    {
        glPushMatrix();
            glRotated(aRotacao, 0.0, 1.0, 0.0);
            glRotated(-90, 1.0, 0.0, 0.0);
            if (desenha_textura() && astro->textura >= 0) {
                desenha_esfera(astro->textura, astro->raio, precisao);
            } else {
                desenha_esfera(astro);
            }
        glPopMatrix();
    } else if(inExplosion(astro->explosao)) {
        displayExplosion(astro->explosao);
    }
}

// faz a iluminação do sol
void ilumina_sol()
{
    // faz a ilumização do SOL diferente das demais
    // colocando a fonte de emissão de luz na câmera
    GLdouble pos[3];
    posicao_camera(pos);
    GLfloat posicaoLuz[4]={(GLfloat)pos[0],(GLfloat)pos[1],(GLfloat)pos[2],1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    //GLfloat mat_emission[] = {0.2, 0.2, 0.2, 0.2};
    //glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}

// faz a iluminação dos demais astros
// retorna a posição da luz para o SOL
void ilumina_astros()
{
    GLfloat posicaoLuz[4]={0.0, 0.0, 0.0, 1.0};
   glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    GLfloat mat_emission[] = {0.0, 0.0, 0.0, 0.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}

void desenha_orbita_circular(CorpoCeleste *astro, double dist)
{
    // desenha a órbita circular
    if (astro->ativo && !astro->especial && desenha_orbita()) {
        glDisable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable (GL_BLEND);
        glColor3d (astro->cor[C_R], astro->cor[C_G], astro->cor[C_B]);
        circuloY(90.0, 90.0, dist);
        glDisable (GL_BLEND);
        glEnable(GL_LIGHTING);
    }
}

double customiza_astro(CorpoCeleste *astro)
{
    if (astro->customizar != NULL)
        return astro->customizar(astro);
    else
        return astro->raio;
}

//---------------------------------------------------------------------------
// define o protópipo do desenhar_astro
void desenhar_astro(CorpoCeleste *astro,
        CorpoCeleste *principal, double afastamento);
//---------------------------------------------------------------------------

// desenha cada satélite do astro
void desenhar_satelites(CorpoCeleste *astro)
{
    // faz a customização para desenhar elementos extras
    // e retorna uma distancia de afastamento
    double afastamento =  customiza_astro(astro);
    // Desenha cada satélide
    for (int i = 0; i < astro->satelites; i++)
        // se o astro tem controle de desenho, chama sua função,
        // senão, faz o desenho padrão
        if (astro->satelite[i].desenhar != NULL)
            astro->satelite[i].desenhar(
                    &(astro->satelite[i]), astro, afastamento);
        else
            desenhar_astro(&(astro->satelite[i]), astro, afastamento);
}
// desenha o sistema solar recursivamente (mantendo a hierarquia dos objetos)
// desenha todos os astros, as explosões e outros efeitos (exceto o asteróide)
// também tem parâmeteos não usados (veja comentário de desenha_hiperion)
void desenhar_astro(CorpoCeleste *astro,
        CorpoCeleste *principal, double afastamento)
{
    if (astro->inexistente)
        return;
    glPushMatrix();
        // afasta o astro de sua orbita para evitar colisão visual
        // garante um afastamento mínimo
        double dist = afastamento + astro->dist + astro->raio;
        RENDER(desenha_orbita_circular(astro, dist));
        // posiciona o planeta na órbita correta
        glRotated(angulo_translacao(astro), 0.0, 1.0, 0.0);
        glTranslated(dist, 0.0, 0.0);
        CALCULO(setPosition(astro->pos));
        // se renderiza faz a iluminação e o desenho da órbita
        RENDER(desenhar_corpo_celeste(astro, angulo_rotacao(astro), 60));
        // Desenha cada satélide (recursão)
        desenhar_satelites(astro);
    glPopMatrix();
}


// desenha o sistema solar recursivamente (mantendo a hierarquia dos objetos)
// desenha todos os astros, as explosões e outros efeitos (exceto o asteróide)
void desenhar_sol(CorpoCeleste *astro)
{
    configura_visao();
    if (astro->inexistente)
        return;
    // posiciona o planeta na órbita correta
    glRotated(angulo_translacao(astro), 0.0, 1.0, 0.0);
    // desenha a esfera com iluminação
    if(deve_renderizar) {
        ilumina_sol();
        desenhar_corpo_celeste(astro, angulo_rotacao(astro), 180);
        ilumina_astros();
    } else
        setPosition(astro->pos);
    // Desenha cada satélide (recursão)
    desenhar_satelites(astro);
}

// desenha uma elípse representando uma orbita
void elipse(double raioX, double raioY, bool pontos)
{
    glPushAttrib(GL_ENABLE_BIT);
        if (pontos) {
            glLineStipple(1, lineStipple);
            glEnable(GL_LINE_STIPPLE);
        }
        double circle_points = 180.0f;
        double angulo;
        glBegin(GL_LINE_LOOP);
            for (int i = 0; i < circle_points; i++) {
                angulo = 2*M_PI*i/circle_points;
                glVertex3d(cos(angulo)*raioX, sin(angulo)*raioY, 0.0);
            }
        glEnd();
        glDisable(GL_LINE_STIPPLE);
    glPopAttrib();
}

void desenha_obita_eliptica(CorpoCeleste *astro,
        double raioX, double raioY, bool pontos)
{
    // desenha a órbita
    if (astro->ativo && desenha_orbita()){
        glColor3d (astro->cor[C_R], astro->cor[C_G], astro->cor[C_B]);
        elipse(raioX, raioY, pontos);
    }
}


// desenha a lua FEBE
// também tem parâmeteos não usados (veja comentário de desenha_hiperion)
void desenha_febe(CorpoCeleste *astro,
        CorpoCeleste *principal, double afastamento)
{
    double raioX = afastamento * 1.2;
    double raioY = afastamento * 2;
    glPushMatrix();
        glRotated(90, 1.0, 0.0, 0.0);
        glRotated(15, 0.0, 1.0, 1.0);
        double aTransl = -angulo_translacao(astro);
        RENDER(desenha_obita_eliptica(astro, raioX, raioY, true));
        glPushMatrix();
            glTranslated(cos(aTransl)*raioX, sin(aTransl)*raioY, 0.0);
            glRotated(-90, 1.0, 0.0, 0.0);
            CALCULO(setPosition(astro->pos));
            RENDER(desenhar_corpo_celeste(astro, angulo_rotacao(astro), 20));
        glPopMatrix();
    glPopMatrix();

}

// desenha a lua HIPERION
// Gera alerta no compilador caso use a opção [-Wunused-parameter]:
// unused parameter 'afastamento'
// Mas a interface é geral e não pode ser mudada por motivo de compatibilidade
void desenha_hiperion(CorpoCeleste *astro,
        CorpoCeleste *principal, double afastamento)
{
    double raioX = principal->raio * 1.1;
    double raioY = principal->raio * 1.9;
    // função de sorteio do movimento: fiza uma meta (vetor: aumenta ou diminui)
    // pelo vetor, aplica modificações de valor aletaório e em pequena escala
    if (!deve_renderizar)
        movimento_hiperion();
    glPushMatrix();
        glRotated(90, 1.0, 0.0, 0.0);
        // define um deslocamento da lua em relação à saturno
        double d = principal->raio / 2;
        glTranslated(0, 0, -d);
        glRotated(90, 0.0, 0.0, 1.0);
        // translação e rotação aleatórias
        double aTransl = -angulo_translacao(astro) + mov_hip.dtransl;
        double aRotacao = angulo_rotacao(astro) + mov_hip.drot;
        raioX = raioX * mov_hip.raioT;
        // desenha a lua em órbita elíptica
        RENDER(desenha_obita_eliptica(astro, raioX, raioY,
                get_em_foco() != principal));
        // desenha o satélite
        glPushMatrix();
            glTranslated(cos(aTransl)*raioX, sin(aTransl)*raioY, 0.0);
            glRotated(-90, 1.0, 0.0, 0.0);
            CALCULO(setPosition(astro->pos));
            RENDER(desenhar_corpo_celeste(astro, aRotacao, 30));
        glPopMatrix();
    glPopMatrix();
}

double anel_alfa_color = 1.0;

// desenha uma parte (um anél específico) de saturno
void desenha_anel_saturno(double r1, double r2, GLubyte *stipple, double gray)
{
    // efeito de explosão
    if (anel_alfa_color < 1.0) {
        gray = anel_alfa_color;
        glColor4d(gray, gray/4, gray/8, gray);
        r2 /= anel_alfa_color;
        r1 /= anel_alfa_color;
    } else
        glColor4d(gray, gray, gray, 1.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(stipple);
    gluDisk(quad, r1, r2, 60, 60);
    glDisable(GL_POLYGON_STIPPLE);
    glDisable(GL_BLEND);
}

// colapso do anel de saturno
bool anel_em_colapso = false;
double dia_colapso = 0.0;
double duracao_colapso = 3;

bool em_colapso_anel_saturno() { return anel_em_colapso; }
void colapso_anel_saturno(bool c) { anel_em_colapso = c; }

void terminar_colapso_anel_saturno() {
    anel_em_colapso = false;
    anel_alfa_color = 1.0;
}
void iniciar_colapso_anel_saturno()
{
    anel_em_colapso = true;
    dia_colapso = get_dia();
}

bool anel_saturno_ativo() { return anel_alfa_color == 1.0; }

// desenha o conjunto de anéis de saturno
double desenha_aneis(CorpoCeleste *astro)
{
    if(deve_renderizar) {
        if (astro->ativo && anel_alfa_color > 0.0) {
            glPushMatrix();
                glDisable(GL_CULL_FACE);
                glRotated(90, 1.0, 0.0, 0.0);
                //glRotated(rotacao, 0.0, 0.0, 1.0);
                desenha_anel_saturno(astro->raio * 1.1, astro->raio * 1.3,
                    stipple1, 83.0/255.0);
                desenha_anel_saturno(astro->raio * 1.3, astro->raio * 1.8,
                    stipple2, 1.0);
                desenha_anel_saturno(astro->raio * 1.8, astro->raio * 1.85,
                    stipple1, 160.0/255.0);
                desenha_anel_saturno(astro->raio * 1.85, astro->raio * 2.05,
                    stipple2, 211.0/255.0);
                glEnable(GL_CULL_FACE);
            glPopMatrix();
        }
    } else {
        if(anel_em_colapso) {
            // vai "avermelhando" o anel com o passar do tempo
            //double dia = get_dia();
            if (anel_alfa_color > 0.0) {
                double reducao = (get_dia() - dia_colapso) / duracao_colapso;
                anel_alfa_color = 1.0 - reducao;
            } else {
                terminar_colapso_anel_saturno();
                anel_alfa_color = 0.0;
            }
        }
    }
    return astro->raio * FATOR_ANEL_SATURNO;
}

//desenha orbita linear do asteroide
void desenha_orbita_linear(CorpoCeleste *astro)
{
    if (desenha_orbita()) {
        glDisable(GL_LIGHTING);
        glPushAttrib(GL_ENABLE_BIT);
        glLineStipple(1, lineStipple);
        glEnable(GL_LINE_STIPPLE);
        glBegin(GL_LINES);
            double Q[3], R[3];
            glColor3d (astro->cor[C_R], astro->cor[C_G], astro->cor[C_B]);
            orbita_asteroide(Q, R);
            glVertex3d(Q[0], Q[1], Q[2]);
        glVertex3d(R[0], R[1], R[2]);
        glEnd();
        glPopAttrib();
        glEnable(GL_LIGHTING);
    }
}
// desenha o asteróide
// também tem parâmeteos não usados (veja comentário de desenha_hiperion)
void desenhar_asteroide(CorpoCeleste *astro,
        CorpoCeleste *principal, double afastamento)
{
    if (astro->ativo) {
        glPushMatrix();
            RENDER(desenha_orbita_linear(astro));
            // desloca para a posição do asteróide
            glTranslated(astro->pos[0], astro->pos[1], astro->pos[2]);
            CALCULO(setPosition(astro->pos));
            // desenha o asteróide
            RENDER(desenhar_corpo_celeste(astro, angulo_rotacao(astro), 50));
        glPopMatrix();
    }
}

// inicia o planeta saturno
void configura_saturno(CorpoCeleste *sol)
{
    memcpy (stipple1, Stipple1, 128);
    memcpy (stipple2, Stipple2, 128);
    quad = gluNewQuadric();
    sol->satelite[SATURNO].customizar = &desenha_aneis;
    sol->satelite[SATURNO].satelite[FEBE].desenhar = &desenha_febe;
    sol->satelite[SATURNO].satelite[HIPERION].desenhar = &desenha_hiperion;
    terminar_colapso_anel_saturno();
    inicia_movimento_aleatorio_hiperion();
}
