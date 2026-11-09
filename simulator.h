#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "particle.h"
#include "event.h"

/**
 * @brief Opaque type representing an event-driven simulation of particles
 * moving in a box [0, width] x [0, height] and colliding elastically with
 * each other and with the walls of the box.
 */
typedef struct Simulator_t Simulator;

/**
 * @brief Creates a new simulator, with no particle, at time 0.
 *
 * @param width            The width of the box.
 * @param height           The height of the box.
 * @param maxNumParticles  The maximum number of particles of the simulation.
 * @return A pointer to the new simulator, or NULL in case of allocation error.
 */
Simulator *simulatorCreate(double width, double height, int maxNumParticles);

/**
 * @brief Frees a simulator, as well as all the particles (and events) it
 * contains.
 *
 * @param s  The simulator to free.
 */
void simulatorFree(Simulator *s);

/**
 * @brief Returns the height of the box of a simulator.
 *
 * @param s  A simulator.
 * @return The height of the box.
 */
double simulatorGetHeight(Simulator *s);

/**
 * @brief Returns the width of the box of a simulator.
 *
 * @param s  A simulator.
 * @return The width of the box.
 */
double simulatorGetWidth(Simulator *s);

/**
 * @brief Returns the total number of collisions between particles since the
 * beginning of the simulation (collisions with the walls are not counted).
 *
 * @param s  A simulator.
 * @return The number of collisions between particles.
 */
int simulatorGetNumCollisions(Simulator *s);

/**
 * @brief Returns the current time of the simulation (initially 0).
 *
 * @param s  A simulator.
 * @return The current time of the simulation.
 */
double simulatorGetElapsedTime(Simulator *s);

/**
 * @brief Returns the number of particles in the simulation.
 *
 * @param s  A simulator.
 * @return The number of particles.
 */
int simulatorGetNumberOfParticles(Simulator *s);

/**
 * @brief Adds a particle to the simulation. The particle is not added if:
 * - the maximum number of particles is reached;
 * - the particle touches or crosses a wall of the box;
 * - the particle overlaps or touches a particle already in the simulation;
 * - the simulation has already started, i.e. simulatorStep or
 *   simulatorGetNextEvent has already been called.
 * If the particle is added, the simulator becomes responsible for freeing
 * it. Otherwise, the caller remains responsible for freeing it.
 *
 * @param s  A simulator.
 * @param p  The particle to add.
 * @return true if the particle has been added, false otherwise.
 */
bool simulatorAddParticle(Simulator *s, Particle *p);

/**
 * @brief Returns a particle of the simulation. Particles are indexed from 0
 * to N-1 (where N is the number of particles), in the order in which they
 * have been added.
 *
 * @param s      A simulator.
 * @param index  The index of the particle.
 * @return The particle of index index, or NULL if index is not valid.
 */
Particle *simulatorGetParticle(Simulator *s, int index);

/**
 * @brief Returns the next event of the simulation, i.e. the earliest
 * collision (between two particles or between a particle and a wall) that
 * will occur after the current time if all particles keep moving in a
 * straight line. The time of the event is absolute (not relative to the
 * current time). If no event can occur, an event of type NONE and of time
 * INFINITY_TIME is returned. The returned event belongs to the simulator and
 * must not be freed (nor modified) by the caller. It remains valid until the
 * next call to simulatorStep or simulatorFree.
 *
 * @param s  A simulator.
 * @return The next event of the simulation.
 */
Event *simulatorGetNextEvent(Simulator *s);

/**
 * @brief Advances the simulation from the current time t to time t + dt.
 * All the events occurring before t + dt are processed in chronological
 * order: particles are moved until the time of the event, the velocities of
 * the particles involved in the event are updated, and the next event is
 * computed. Particles are finally moved until time t + dt, which becomes the
 * new current time of the simulation.
 *
 * @param s   A simulator.
 * @param dt  The duration of the step (>= 0).
 */
void simulatorStep(Simulator *s, double dt);

/**
 * @brief Computes the average, over all particles of the simulation, of a
 * function of a particle. For example,
 * simulatorStatParticles(s, particleKineticEnergy) returns the average
 * kinetic energy of the particles.
 *
 * @param s  A simulator (with at least one particle).
 * @param f  A function computing a value from a particle.
 * @return The average of f over all particles.
 */
double simulatorStatParticles(Simulator *s, double (*f)(Particle *));

#endif
