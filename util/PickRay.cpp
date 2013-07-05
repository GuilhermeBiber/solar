/*
 * PPickRay.cpp
 *
 *  Created on: 11/05/2012
 *      Author: Frederico Martins Biber Sampaio
 *      Adaptação: Guilherme Sampaio
 */

#include <GL/glut.h>
#include <stdio.h>
#include <float.h>

#include "geometria.h"

GLdouble invMv[16];
GLdouble iniciado = false;

/*
 Gera a iversa da atual matriz de modelo  para iniciar a obtenção de posições
 durante a renderização.
 No momento da iniciação, o ponto (0,0,0) será o de referência para os
 cálculos da demais posições após qualquer transformação.

 Sendo A e B matrizes 4x4 e C um vetor de 4:

 A*B*C = D    =>  inv(A)*D = B*C

 Assim, considere A a matriz inicial (com o posicionamento da câmera e
 eventuais outras transofmrações da cena).
 Considere B a matriz com todas as transformações desde A e C um ponto
 a ser transformado para o modelo original na forma P = (B*C).
 A atual matriz de modelo é sempre (A*B). Multiplicando-se por um ponto C,
 obtem-se D. Para obter P, relativo ao modelo original, P = (B*C) = inv(A)*D.

 Útil para remover a relatividade da posição da câmera em relação ao modelo.
 Retorna FALSO (0) se a matriz de modelo não for inversível, impossibilitando
 o calculo apurado.
 No caso de não ser possível inverter a matriz, a melhor alternativa é
 fazer os cálculos das tranformações para cada objeto separadamente, que
 provavelmente é a solução mais adequada para todos os casos, ou pode-se
 usar uma posição de câmera com dois ou três eixos igauais a zero.
 O eixo que não for zerado, basta somar ou subtrair sua posição,
 lembrado que a projeção é feita multiplicando-se pelo vetor (0, 0, -1, 0).
*/
char initPickRayDepth()
{
	/* possível otimização: como a matriz de modelo só muda na mudança de
	   posicionamento de câmera ou de reposicionamento de cenário
	   (se for o caso), essa função não precisa ser ativada a cada seleção.
	   Bastaria verifica se iniciado == TRUE antes da seleção e
	   chamar essa função a cada mudança de camera ou cenário.
	   Como o software optou por um preview da reta do raio de seleção
	   (pick ray), a rotação e a translação do sol foi colocada em
	   função da camera w toda rotação ou translação muda a perspectiva do
	   cenário. Por isso, o cálculo foi mantido a cada seleção.
	*/
	GLdouble mv[16];		//Modelview matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	iniciado = InvertMatrixf2(invMv, mv);
	return iniciado;
}

// inverte a projeção de um ponto do modelo (usado nas funções OpenGL)
// de acordo com a atual matrize de modelo removendo a matriz inicial
// Veja comentário ma função initPickRayDepth (acima)
void unProjectPoint(GLdouble ponto[3])
{
	GLdouble mv[16];		//Modelview matrix
	glGetDoublev(GL_MODELVIEW_MATRIX, mv);

	GLdouble tmp[4];

	MultiplyMatrixByVector4by4(tmp, mv, ponto);
	MultiplyMatrixByVector4by4(ponto, invMv, tmp);
}

// define a posição do ponto (0,0,0) do modelo em termos das atuais matrizes
// GL_MODELVIEW_MATRIX GL_PROJECTION_MATRIX e registra no buffer de posições.
// Como o que se quer são as coordenadas absoluta, e não homogêneas,
// basta dividir pelo h (o valor do quarto elemento do vetor de posição).
void setPosition(GLdouble pos[3])
{
	// verifica se existe uma matriz inversa e
	// se o indice do objeto é válido
	if (iniciado) {
		GLdouble ponto[4];
		// adota o ponto zero atual, que será o centro das esferas
		ponto[0]=0;
		ponto[1]=0;
		ponto[2]=0;
		ponto[3]=1;
		unProjectPoint(ponto);
		// trasnforma as coordenadas homogêneas em absolutas (normalizar)
		pos[0] = ponto[0] / ponto[3];
		pos[1] = ponto[1] / ponto[3];
		pos[2] = ponto[2] / ponto[3];
	} else {
		// se não puder calcular, retorna um ponto qualquer
		// (já deve ter mostrado erro antes...)
		pos[0] = 0;
		pos[1] = 0;
		pos[2] = 0;
	}
}

// retona o ponto Q relativo ao modelo baseado em um ponto da imagem (janela)
void UnProject(GLdouble Q[3], GLdouble R[3], int x, int y)
{
	GLfloat wx, wy;
	GLdouble mv[16];		//Modelview matrix
	GLdouble proj[16];		//Projection matrix
	GLint vp[4];			//Viewport

	glGetDoublev(GL_MODELVIEW_MATRIX, mv);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glGetIntegerv(GL_VIEWPORT, vp);

	wx = (GLfloat)x;
	wy = (GLfloat)(vp[3] - y);	//inverte o y

    // ponto mais próximo: wz = 0
	gluUnProject(wx, wy, 0, mv, proj, vp, Q, Q+1, Q+2);
	// ponto mais distante: wz = 1
	gluUnProject(wx, wy, 1, mv, proj, vp, R, R+1, R+2);
}

// retira a informação de profundidade do Z buffer do OpenGL
GLfloat depth(int x, int y)
{
	//re-enable various settings we may have turned off
	//glEnable(GL_DEPTH_TEST);	//Re-enable depth testing and masking
	//glDepthMask(GL_TRUE);
	//glDepthRange(0.0f,1.0f);
	GLfloat wz;
	glFinish();
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
	return wz;
}


