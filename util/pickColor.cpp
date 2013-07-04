/*
 * pickColor.cpp
 *
 *  Created on: 01/07/2013
 *      Author: Frederico Sampaio
 */
#include <GL/glut.h>
#include <stdio.h>

void init_ColorPick()
{
    // desabilita efeitos que modificam a cor
    glDisable(GL_BLEND);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_1D);
    glDisable(GL_TEXTURE_2D);
    //glDisable(GL_TEXTURE_3D);
    glDisable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT);
}

void selecionar_ColorPick(GLint x, GLint y, double cor[4])
{
    glFinish();

    // retira a informação de cor do ponto selecionado
    GLint vp[4];            //Viewport
    glGetIntegerv(GL_VIEWPORT, vp);
    y = (vp[3] - y);       //inverte o y
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_FLOAT, cor);

}

