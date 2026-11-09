#include "particle.h"
#include <float.h>
#include <stdbool.h>
#include <stdlib.h>
#include "color.h"
#include <math.h>
#include "vector.h"


struct Particle_t  {
    double mass, radius;    //   The mass of the particle and radius (> 0).
    Vector position, velocity ; // The velocity and position  of the particle.
    Color color;  //   The color of the particle (used only for display).
    double numcollision; 
};


Particle *particleCreate(double radius, double mass, Vector position, Vector velocity, Color color){
    Particle *p = malloc(sizeof(Particle));
    if (p == NULL) {
        return NULL; 
    
    }
    p-> radius = radius;
    p-> mass = mass;
    p-> position = position;
    p-> velocity = velocity;
    p-> color = color;
    return p;

}

void particleFree(Particle *p){
    free (p);
}

/**
 * @brief Returns the radius of a particle.
 *
 * @param p  A particle.
 * @return The radius of the particle.
 */
double particleGetRadius(Particle *p)
{
    double radius = p->radius ; 
    return radius ; 
}

/**
 * @brief Returns the mass of a particle.
 *
 * @param p  A particle.
 * @return The mass of the particle.
 */
double particleGetMass(Particle *p)
{
    double mass = p->mass ; 
    return mass ;
}

/**
 * @brief Returns the current position of (the center of) a particle.
 *
 * @param p  A particle.
 * @return The position of the particle.
 */
Vector particleGetPosition(Particle *p)
{
    Vector position = p->position ;
    return position ;
}

/**
 * @brief Returns the current velocity of a particle.
 *
 * @param p  A particle.
 * @return The velocity of the particle.
 */
Vector particleGetVelocity(Particle *p){
    Vector velocity = p->velocity ; 
    return velocity ;
}

/**
 * @brief Returns the color of a particle.
 *
 * @param p  A particle.
 * @return The color of the particle.
 */
Color particleGetColor(Particle *p)
{
    Color color = p->color ; 
    return color ; 
}

/**
 * @brief Returns the number of collisions with other particles undergone by
 * a particle since its creation (collisions with the walls are not counted).
 * The number is returned as a double so that this function can be passed
 * as argument to simulatorStatParticles.
 *
 * @param p  A particle.
 * @return The number of collisions of the particle.
 */
double particleGetNumCollisions(Particle *p)
{
    double numcollisions = p->numcollision ;
    return numcollisions ; 
}


/**
 * @brief Returns the kinetic energy of a particle, i.e. m * ||v||^2 / 2,
 * where m is its mass and v its velocity.
 *
 * @param p  A particle.
 * @return The kinetic energy of the particle.
 */
double particleKineticEnergy(Particle *p)
{
   double KineticEnergy = 0 ; 
   double mass  = particleGetMass(p); 
   Vector velocity = particleGetVelocity(p);
   double v_squared = vectorNorm(velocity);

    KineticEnergy = 0.5 * mass * v_squared ; 

    return  KineticEnergy ; 
}

/**
 * @brief Moves a particle in a straight line at constant velocity during
 * a time dt. Collisions are not taken into account.
 *
 * @param p   A particle.
 * @param dt  The duration of the move (>= 0).
 */
void particleMove(Particle *p, double dt)
{
    Vector position = particleGetPosition(p); 
    Vector velocity = particleGetVelocity(p); 
    
    Vector meters = vectorMultByScalar( velocity , dt) ;

    position = vectorSum( position , meters) ; 
}
/**
 * @brief Updates the velocities of two particles that are in contact,
 * according to the laws of the elastic collision between two disks, and
 * increments the number of collisions of both particles. Does nothing if
 * p1 and p2 are the same particle.
 *
 * @param p1  The first particle.
 * @param p2  The second particle.
 */
void particleHandleCollision(Particle *p1, Particle *p2)
{
    if (p1 == p2) {
        return;
    }
    Vector pos1 = particleGetPosition(p1);
    Vector pos2 = particleGetPosition(p2);
    Vector vel1 = particleGetVelocity(p1);
    Vector vel2 = particleGetVelocity(p2);
    double r1 = particleGetRadius(p1);
    double r2 = particleGetRadius(p2);
    double m1 = particleGetMass(p1);
    double m2 = particleGetMass(p2);
    Vector dp = vectorDiff(pos2,pos1);
    Vector dv = vectorDiff(vel2,vel1);

    double dot_dv_dp = vectorDotProduct(dv, dp);
    double norm_dp_sq = vectorDotProduct(dp, dp); // ||Δp||²
    double mass_sum = m1 + m2;


    if (norm_dp_sq == 0.0) {
        return;
    }

    double common_factor = dot_dv_dp / norm_dp_sq;

    // Facteurs scalaires pour chaque particule selon les formules :
    // Pour p1 (i) : + (2*m2 / (m1+m2)) * (Δv·Δp / ||Δp||²) * Δp
    // Pour p2 (j) : - (2*m1 / (m1+m2)) * (Δv·Δp / ||Δp||²) * Δp
    double factor1 = (2.0 * m2 / mass_sum) * common_factor;
    double factor2 = (2.0 * m1 / mass_sum) * common_factor; 

    Vector facV1 = vectorMultByScalar(dp,factor1);
    Vector facV2 = vectorMultByScalar(dp,factor2);


    vel1 = vectorSum (vel1 ,facV1 );
    vel2 = vectorDiff(vel2 , facV2 );


}



/**
 * @brief Updates the velocity of a particle that hits a vertical wall
 * (the x component of its velocity is reversed).
 *
 * @param p  A particle.
 */
void particleHandleXWallCollision(Particle *p)
{
    Vector velocity = particleGetVelocity(p);

    velocity.x = -velocity.x;

    p->velocity = velocity;
}
/**
 * @brief Updates the velocity of a particle that hits a horizontal wall
 * (the y component of its velocity is reversed).
 *
 * @param p  A particle.
 */
void particleHandleYWallCollision(Particle *p)
{
    Vector velocity = particleGetVelocity(p);

    velocity.y = -velocity.y;

    p->velocity = velocity;
}

/**
 * @brief Computes the time before two particles collide, assuming that they
 * both keep moving in a straight line at constant velocity.
 *
 * @param p1  The first particle.
 * @param p2  The second particle.
 * @return The time (relative to the current time) before the collision, or
 * INFINITY_TIME if the particles will never collide (in particular if they
 * are moving away from each other).
 */
double particleGetCollisionTime(Particle *p1, Particle *p2)
{
    if (p1 == p2)
    {
        return INFINITY_TIME;
    }

    Vector pos1 = particleGetPosition(p1);
    Vector pos2 = particleGetPosition(p2);

    Vector vel1 = particleGetVelocity(p1);
    Vector vel2 = particleGetVelocity(p2);

    Vector dp = vectorDiff(pos2, pos1);
    Vector dv = vectorDiff(vel2, vel1);

    double sigma = particleGetRadius(p1) + particleGetRadius(p2);

    double dv_dp = vectorDotProduct(dv, dp);
    double dv_dv = vectorDotProduct(dv, dv);
    double dp_dp = vectorDotProduct(dp, dp);

    if (dv_dp >= 0)
    {
        return INFINITY_TIME;
    }

    double d = dv_dp * dv_dp
             - dv_dv * (dp_dp - sigma * sigma);

    if (d < 0)
    {
        return INFINITY_TIME;
    }

    return (-dv_dp - sqrt(d)) / dv_dv;
}

/**
 * @brief Computes the time before a particle hits one of the vertical walls
 * (x = 0 or x = width) of the box, assuming that it keeps moving in a
 * straight line at constant velocity.
 *
 * @param p      A particle.
 * @param width  The width of the box.
 * @return The time (relative to the current time) before the particle hits
 * a vertical wall, or INFINITY_TIME if it never will.
 */
double particleGetXWallCollisionTime(Particle *p, double width)
{
    Vector position = particleGetPosition(p);
    Vector velocity = particleGetVelocity(p);

    double radius = particleGetRadius(p);

    if (velocity.x > 0)
    {
        return (width - radius - position.x) / velocity.x;
    }

    if (velocity.x < 0)
    {
        return (radius - position.x) / velocity.x;
    }

    return INFINITY_TIME;
}

/**
 * @brief Computes the time before a particle hits one of the horizontal walls
 * (y = 0 or y = height) of the box, assuming that it keeps moving in a
 * straight line at constant velocity.
 *
 * @param p       A particle.
 * @param height  The height of the box.
 * @return The time (relative to the current time) before the particle hits
 * a horizontal wall, or INFINITY_TIME if it never will.
 */
double particleGetYWallCollisionTime(Particle *p, double height)
{
    Vector position = particleGetPosition(p);
    Vector velocity = particleGetVelocity(p);

    double radius = particleGetRadius(p);

    if (velocity.y > 0)
    {
        return (height - radius - position.y) / velocity.y;
    }

    if (velocity.y < 0)
    {
        return (radius - position.y) / velocity.y;
    }

    return INFINITY_TIME;
}

/**
 * @brief Checks whether two particles overlap or touch each other, i.e.
 * whether the distance between their centers is lower than or equal to the
 * sum of their radii.
 *
 * @param p1  The first particle.
 * @param p2  The second particle.
 * @return true if the particles overlap or touch, false otherwise.
 */
bool particleOverlap(Particle *p1, Particle *p2)
{
    Vector pos1 = particleGetPosition(p1);
    Vector pos2 = particleGetPosition(p2);

    Vector dp = vectorDiff(pos2, pos1);

    double distance = vectorNorm(dp);

    double rayon = particleGetRadius(p1) + particleGetRadius(p2);

    return distance <= rayon;
}

/**
 * @brief Checks whether a particle touches or crosses one of the walls of
 * the box [0, width] x [0, height].
 *
 * @param p       A particle.
 * @param width   The width of the box.
 * @param height  The height of the box.
 * @return true if the particle touches or crosses a wall, false otherwise.
 */
bool particleOutside(Particle *p, double width, double height)
{
    Vector position = particleGetPosition(p);

    double radius = particleGetRadius(p);

    if (position.x - radius <= 0)
    {
        return true;
    }

    if (position.x + radius >= width)
    {
        return true;
    }

    if (position.y - radius <= 0)
    {
        return true;
    }

    if (position.y + radius >= height)
    {
        return true;
    }

    return false;
}


