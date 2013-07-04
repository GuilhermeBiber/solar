/*
 * explosion.h
 *
 *  Created on: 28/06/2013
 *      Author: Guilherme Sampaio
 */

#ifndef EXPLODE_H_
#define EXPLODE_H_

/* A particle */

struct particleData
{
        double   position[3];
        double   speed[3];
        double   color[3];
};
typedef struct particleData    particleData;


/* A piece of debris */

struct debrisData
{
        double   position[3];
        double   speed[3];
        double   color[3];
        double   orientation[3];        /* Rotation angles around x, y, and z axes */
        double   orientationSpeed[3];
        double   scale[3];
};
typedef struct debrisData    debrisData;

struct explosion
{
        double fuel;                /* "fuel" of the active explosion */
        double charge;              /* default "fuel" of explosion */
        int numParticles;
        int numDebris;
        double radius;
        particleData *particles;
        debrisData *debris;
};
typedef struct explosion Explosion;

Explosion newExplosion();
Explosion newExplosion(double charge, int num_particles, int num_debris);
Explosion newExplosion(double charge);
void initExplosion(Explosion *ptr_e);
void freeExplosion(Explosion *ptr_e);
void displayPartcles(Explosion e);
void displayDebris(Explosion e);
void displayExplosion(Explosion e);
int idleExplosion(Explosion *ptr_e, double tempo);
void pauseExplosion();
void resumeExplosion();
void fireballExplosion();
void normalExplosion();
int countExplosions();

#define inExplosion(e)       (e.fuel > 0)
#define radiusExplosion(e,r) (e.radius = r)

#endif /* EXPLODE_H_ */
