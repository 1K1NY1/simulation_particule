#ifndef PARTICLE_H
#define PARTICLE_H

#include <float.h>
#include <stdbool.h>

#include "color.h"
#include "vector.h"

/**
 * @brief Value representing an infinite time (e.g., the time before a
 * collision that will never occur).
 */
#define INFINITY_TIME DBL_MAX

/**
 * @brief Opaque type representing a particle, i.e. a rigid disk moving in a
 * 2D box. A particle is characterized by its radius, its mass, the position
 * of its center, its velocity and its color.
 */
typedef struct Particle_t Particle;

/**
 * @brief Creates a new particle.
 *
 * @param radius    The radius of the particle (> 0).
 * @param mass      The mass of the particle (> 0).
 * @param position  The position of the center of the particle.
 * @param velocity  The velocity of the particle.
 * @param color     The color of the particle (used only for display).
 * @return A pointer to the new particle, or NULL in case of allocation error.
 */
Particle *particleCreate(double radius, double mass, Vector position, Vector velocity, Color color);

/**
 * @brief Frees a particle.
 *
 * @param p  The particle to free.
 */
void particleFree(Particle *p);

/**
 * @brief Returns the radius of a particle.
 *
 * @param p  A particle.
 * @return The radius of the particle.
 */
double particleGetRadius(Particle *p);

/**
 * @brief Returns the mass of a particle.
 *
 * @param p  A particle.
 * @return The mass of the particle.
 */
double particleGetMass(Particle *p);

/**
 * @brief Returns the current position of (the center of) a particle.
 *
 * @param p  A particle.
 * @return The position of the particle.
 */
Vector particleGetPosition(Particle *p);

/**
 * @brief Returns the current velocity of a particle.
 *
 * @param p  A particle.
 * @return The velocity of the particle.
 */
Vector particleGetVelocity(Particle *p);

/**
 * @brief Returns the color of a particle.
 *
 * @param p  A particle.
 * @return The color of the particle.
 */
Color particleGetColor(Particle *p);

/**
 * @brief Returns the number of collisions with other particles undergone by
 * a particle since its creation (collisions with the walls are not counted).
 * The number is returned as a double so that this function can be passed
 * as argument to simulatorStatParticles.
 *
 * @param p  A particle.
 * @return The number of collisions of the particle.
 */
double particleGetNumCollisions(Particle *p);

/**
 * @brief Returns the kinetic energy of a particle, i.e. m * ||v||^2 / 2,
 * where m is its mass and v its velocity.
 *
 * @param p  A particle.
 * @return The kinetic energy of the particle.
 */
double particleKineticEnergy(Particle *p);

/**
 * @brief Moves a particle in a straight line at constant velocity during
 * a time dt. Collisions are not taken into account.
 *
 * @param p   A particle.
 * @param dt  The duration of the move (>= 0).
 */
void particleMove(Particle *p, double dt);

/**
 * @brief Updates the velocities of two particles that are in contact,
 * according to the laws of the elastic collision between two disks, and
 * increments the number of collisions of both particles. Does nothing if
 * p1 and p2 are the same particle.
 *
 * @param p1  The first particle.
 * @param p2  The second particle.
 */
void particleHandleCollision(Particle *p1, Particle *p2);

/**
 * @brief Updates the velocity of a particle that hits a vertical wall
 * (the x component of its velocity is reversed).
 *
 * @param p  A particle.
 */
void particleHandleXWallCollision(Particle *p);

/**
 * @brief Updates the velocity of a particle that hits a horizontal wall
 * (the y component of its velocity is reversed).
 *
 * @param p  A particle.
 */
void particleHandleYWallCollision(Particle *p);

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
double particleGetCollisionTime(Particle *p1, Particle *p2);

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
double particleGetXWallCollisionTime(Particle *p, double width);

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
double particleGetYWallCollisionTime(Particle *p, double height);

/**
 * @brief Checks whether two particles overlap or touch each other, i.e.
 * whether the distance between their centers is lower than or equal to the
 * sum of their radii.
 *
 * @param p1  The first particle.
 * @param p2  The second particle.
 * @return true if the particles overlap or touch, false otherwise.
 */
bool particleOverlap(Particle *p1, Particle *p2);

/**
 * @brief Checks whether a particle touches or crosses one of the walls of
 * the box [0, width] x [0, height].
 *
 * @param p       A particle.
 * @param width   The width of the box.
 * @param height  The height of the box.
 * @return true if the particle touches or crosses a wall, false otherwise.
 */
bool particleOutside(Particle *p, double width, double height);

#endif
