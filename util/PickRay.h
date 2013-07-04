/*
 * PickRay.h
 */

#ifndef PLANETASPICKRAY_H_
#define PLANETASPICKRAY_H_

char initPickRayDepth();
void unProjectPoint(GLdouble ponto[3]);
void UnProject(GLdouble Q[3], GLdouble R[3], int x, int y);
GLfloat depth(int x, int y);
void setPosition(GLdouble pos[3]);

#endif /* PLANETASPICKRAY_H_ */
