/*
 * Adapatado por Gulherme Sampaio
 * Baseado, mas com grandes alterações, em:
 * http://www.student.nada.kth.se/~nv91-gta/OpenGL/examples/explosion.c
 *
 * explosion.c - Simple particle system based explosion demonstration.
 *                   ,
 * Author: Gustav Taxen, nv91-gta@nada.kth.se
 *
 * This program is in the Public Domain. You may freely use, modify
 * and distribute this program.
 *
 * Theory:
 *    This program uses an extremely simple particle system to create
 *    an explosion effect. Each particle is moved from the origin
 *    towards a random direction and, if activated, a random speed.
 *    The color of the particles are changed from white to orange to
 *    red to create a glowing effect. The GL_POINTS primitive is used
 *    to render the particles.
 *       The debris is similar to the particles, with the addition of
 *    orientation and orientation speed.
 *       A point light source is placed in the center of the explosion.
 *
 */
#include <GL/glut.h>

#include <math.h>

#include "explosion.h"

#define NUM_PARTICLES     5000          /* Number of particles  */
#define NUM_DEBRIS         500            /* Number of debris     */
#define DEFAULT_CHARGE     100.0
/* GLUT menu entries */

#define PAUSE              0
#define NORMALIZE_SPEED    1
#define QUIT               2

/* Light sources and material */

GLfloat  light0Amb[4] =  { 1.0, 0.6, 0.2, 1.0 };
GLfloat  light0Dif[4] =  { 1.0, 0.6, 0.2, 1.0 };
GLfloat  light0Spec[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat  light0Pos[4] =  { 0.0, 0.0, 0.0, 1.0 };

GLfloat  light1Amb[4] =  { 0.0, 0.0, 0.0, 1.0 };
GLfloat  light1Dif[4] =  { 1.0, 1.0, 1.0, 1.0 };
GLfloat  light1Spec[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat  light1Pos[4] =  { 0.0, 5.0, 5.0, 0.0 };

GLfloat  materialAmb[4] = { 0.25, 0.22, 0.26, 1.0 };
GLfloat  materialDif[4] = { 0.63, 0.57, 0.60, 1.0 };
GLfloat  materialSpec[4] = { 0.99, 0.91, 0.81, 1.0 };
GLfloat  materialShininess = 27.8;

int      wantNormalize = 0;   /* Speed vector normalization flag */
int      wantPause = 0;       /* Pause flag */
int count = 0;
/*
 * Controls
 */
void pauseExplosion()        { wantPause = true;}
void resumeExplosion()       { wantPause = false;}
void fireballExplosion()     { wantNormalize = true;}
void normalExplosion()       { wantNormalize = false;}


int countExplosions() { return count;}
/*
 * Setup OpenGL
 */
void glStartExplosion()
{
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light0Amb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light0Dif);
    glLightfv(GL_LIGHT2, GL_SPECULAR, light0Spec);
    glLightfv(GL_LIGHT2, GL_POSITION, light0Pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1Amb);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Dif);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1Spec);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Pos);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
    glEnable(GL_NORMALIZE);
}

void glStopExplosion()
{
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT1);
}

/*
 * newSpeed
 *
 * Randomize a new speed vector.
 *
 */
void newSpeed(double dice, double dest[3])
{
    double    x;
    double    y;
    double    z;
    double    len;

    x =(2.0 * dice * ((double) rand()) /((double) RAND_MAX)) - dice;
    y =(2.0 * dice * ((double) rand()) /((double) RAND_MAX)) - dice;
    z =(2.0 * dice * ((double) rand()) /((double) RAND_MAX)) - dice;

    /*
     * Normalizing the speed vectors gives a "fireball" effect
     *
     */

    if(wantNormalize)
    {
        len = sqrt(x * x + y * y + z * z);

        if(len)
        {
            x = x / len;
            y = y / len;
            z = z / len;
        }
    }

    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
}


void sphereParticlesPosition(Explosion e)
{
     /*
     * x = p * sin(phi) * cos(theta)
     * y = p * sin(phi) * sin(theta)
     * z = p * cos(phi)
     */
    double radius = e.radius;
    int step = sqrt(e.numParticles);
    double DEG = M_PI/180.0;
    double dTheta =(360.0 / step) * DEG;
    double dPhi =(360.0 / step) * DEG;
    int point = 0;
    double phi, theta;
    for(phi = -(M_PI); phi <= M_PI; phi += dPhi) {
        //Cálculo das faixas da esfera
        for(theta = 0.0; theta <=(M_PI * 2); theta += dTheta) {
            e.particles[point].position[0]=(double)(radius*sin(phi)*cos(theta));
            e.particles[point].position[1]=(double)(radius*sin(phi)*sin(theta));
            e.particles[point].position[2]=(double)(radius*cos(phi));
            if (++point >= e.numParticles)
                return;
        }
    }
}

void sphereDebrisPosition(Explosion e)
{
     /*
     * x = p * sin(phi) * cos(theta)
     * y = p * sin(phi) * sin(theta)
     * z = p * cos(phi)
     */
    double radius = e.radius;
    int step = sqrt(e.numDebris);
    double DEG = M_PI/180.0;
    double dTheta =(360.0 / step) * DEG;
    double dPhi =(360.0 / step) * DEG;
    int point = 0;
    double phi, theta;
    for(phi = -(M_PI); phi <= M_PI; phi += dPhi) {
        //Cálculo das faixas da esfera
        for(theta = 0.0; theta <=(M_PI * 2); theta += dTheta) {
            e.debris[point].position[0]=(double)(radius*sin(phi)*cos(theta));
            e.debris[point].position[1]=(double)(radius*sin(phi)*sin(theta));
            e.debris[point].position[2]=(double)(radius*cos(phi));
            if (++point >= e.numDebris)
                return;
        }
    }
}


/*
 * feeeExplosion
 *
 * Stop and deallocate a explosion.
 *
 */
void freeExplosion(Explosion *ptr_e)
{
    ptr_e->fuel = 0;
    free(ptr_e->debris);
    free(ptr_e->particles);
    ptr_e->debris = NULL;
    ptr_e->particles = NULL;
}

/*
 * initExplosion
 *
 * Configure a explosion.
 *
 */
void initExplosion(Explosion *ptr_e)
{
    int    i;
    count++;
    Explosion e = *ptr_e;
    if (e.numParticles && e.particles != NULL )
    {
        for(i = 0; i < e.numParticles; i++)
        {
            e.particles[i].position[0] = 0.0;
            e.particles[i].position[1] = 0.0;
            e.particles[i].position[2] = 0.0;

            e.particles[i].color[0] = 1.0;
            e.particles[i].color[1] = 1.0;
            e.particles[i].color[2] = 0.5;

            newSpeed(1.0, e.particles[i].speed);
        }
        if (e.radius)
            sphereParticlesPosition(e);
    }

    if (e.numDebris && e.debris != NULL)
    {
        double radius = (e.radius? e.radius: 1.0);
        for(i = 0; i < e.numDebris; i++)
        {
            e.debris[i].position[0] = 0.0;
            e.debris[i].position[1] = 0.0;
            e.debris[i].position[2] = 0.0;

            e.debris[i].orientation[0] = 0.0;
            e.debris[i].orientation[1] = 0.0;
            e.debris[i].orientation[2] = 0.0;

            e.debris[i].color[0] = 0.7;
            e.debris[i].color[1] = 0.7;
            e.debris[i].color[2] = 0.7;

            e.debris[i].scale[0] =(2.0 * radius *
                    ((double) rand()) /((double) RAND_MAX)) - radius;
            e.debris[i].scale[1] =(2.0 * radius *
                    ((double) rand()) /((double) RAND_MAX)) - radius;
            e.debris[i].scale[2] =(2.0 * radius *
                    ((double) rand()) /((double) RAND_MAX)) - radius;

            newSpeed(radius, e.debris[i].speed);
            newSpeed(radius, e.debris[i].orientationSpeed);
        }
        if (e.radius)
            sphereDebrisPosition(e);
    }
    ptr_e->fuel = ptr_e->charge;
}


/*
 * newExplosion
 *
 * Create a new explosion.
 *
 */
Explosion newExplosion(double charge, int num_particles, int num_debris)
{
    Explosion e;
    e.numParticles = num_particles;
    e.numDebris = num_debris;
    e.particles = (particleData*)malloc(num_particles * sizeof(particleData));
    e.debris = (debrisData*)malloc(num_debris * sizeof(debrisData));
    e.radius = 0.0;
    e.charge = (charge < 60? 60: charge);
    e.fuel = 0.0;
    return e;
}

/*
 * newExplosion
 *
 * Create a new default explosion.
 *
 */
Explosion newExplosion()
{
    return newExplosion(DEFAULT_CHARGE, NUM_PARTICLES, NUM_DEBRIS);
}

/*
 * newExplosion
 *
 * Create a new default explosion with specific charge.
 *
 */
Explosion newExplosion(double charge)
{
    return newExplosion(charge, NUM_PARTICLES, NUM_DEBRIS);
}

/*
 * displayParticles
 *
 * Draw particles of the scene.
 *
 */
void displayPartcles(Explosion e)
{
    if(e.fuel > 0)
    {
        glStartExplosion();
        if (e.numParticles && e.particles != NULL)
        {
            glPushMatrix();

                glDisable(GL_LIGHTING);
                //glDisable(GL_DEPTH_TEST);

                glBegin(GL_POINTS);

                for(int i = 0; i < e.numParticles; i++)
                {
                    glColor3d(e.particles[i].color[0],
                            e.particles[i].color[1],
                            e.particles[i].color[2]);
                    glVertex3d(e.particles[i].position[0],
                            e.particles[i].position[1],
                            e.particles[i].position[2]);
                }

                glEnd();

            glPopMatrix();
        }
    }
}

/*
 * displayDebris
 *
 * Draw particles of the scene.
 *
 */
void displayDebris(Explosion e)
{
    if(e.fuel > 0)
    {
        glStartExplosion();
        if (e.numDebris && e.debris != NULL)
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT2);
            glEnable(GL_DEPTH_TEST);

            glNormal3f(0.0, 0.0, 1.0);

            for(int i = 0; i < e.numDebris; i++)
            {
                glColor3d(e.debris[i].color[0],
                        e.debris[i].color[1],
                        e.debris[i].color[2]);

                glPushMatrix();
                    glTranslatef(e.debris[i].position[0],
                            e.debris[i].position[1],
                            e.debris[i].position[2]);

                    glRotatef(e.debris[i].orientation[0], 1.0, 0.0, 0.0);
                    glRotatef(e.debris[i].orientation[1], 0.0, 1.0, 0.0);
                    glRotatef(e.debris[i].orientation[2], 0.0, 0.0, 1.0);

                    glScalef(e.debris[i].scale[0],
                            e.debris[i].scale[1],
                            e.debris[i].scale[2]);

                    glBegin(GL_TRIANGLES);
                        glVertex3f(0.0, 0.5, 0.0);
                        glVertex3f(-0.25, 0.0, 0.0);
                        glVertex3f(0.25, 0.0, 0.0);
                    glEnd();

                glPopMatrix();
            }
            glDisable(GL_LIGHT2);
        }
        glStopExplosion();
    }
}


/*
 * displayParticles
 *
 * Draw particles of the scene.
 *
 */
void displayExplosion(Explosion e)
{
    displayPartcles(e);
    displayDebris(e);
}
/*
 * idle
 *
 * Update animation variables.
 *
 */

int idleExplosion(Explosion *ptr_e, double tempo)
{
    if(!wantPause)
    {
        Explosion e = *ptr_e;
        if(e.fuel > 0)
        {
            int    i;
            for(i = 0; i < e.numParticles; i++)
            {
                e.particles[i].position[0] +=
                        e.particles[i].speed[0] * 2 * tempo;
                e.particles[i].position[1] +=
                        e.particles[i].speed[1] * 2 * tempo;
                e.particles[i].position[2] +=
                        e.particles[i].speed[2] * 2 * tempo;

                // vai "avermelhando" as partículas com o passar do tempo
                e.particles[i].color[0] -= (1.0 / e.charge) * tempo;
                if(e.particles[i].color[0] < 0.0)
                {
                    e.particles[i].color[0] = 0.0;
                }

                e.particles[i].color[1] -= (4.0 / e.charge) * tempo;
                if(e.particles[i].color[1] < 0.0)
                {
                    e.particles[i].color[1] = 0.0;
                }

                e.particles[i].color[2] -= (8.0 / e.charge) * tempo;
                if(e.particles[i].color[2] < 0.0)
                {
                    e.particles[i].color[2] = 0.0;
                }
            }

            for(i = 0; i < e.numDebris; i++)
            {
                e.debris[i].position[0] += e.debris[i].speed[0] * tempo;
                e.debris[i].position[1] += e.debris[i].speed[1] * tempo;
                e.debris[i].position[2] += e.debris[i].speed[2] * tempo;

                e.debris[i].orientation[0] +=
                        e.debris[i].orientationSpeed[0] * 100 * tempo;
                e.debris[i].orientation[1] +=
                        e.debris[i].orientationSpeed[1] * 100 * tempo;
                e.debris[i].orientation[2] +=
                        e.debris[i].orientationSpeed[2] * 100 * tempo;
            }

            ptr_e->fuel -= tempo;
            if (ptr_e->fuel < tempo) {
                ptr_e->fuel = 0.0;
                count--;
            }
        }
    }

    return ptr_e->fuel;
}

