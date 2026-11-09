#ifndef EVENT_H
#define EVENT_H

#include "particle.h"

/**
 * @brief Opaque type representing an event of the simulation, i.e. a
 * collision between two particles or between a particle and a wall. An event
 * is characterized by its type, the particle(s) involved and the (absolute)
 * time at which it occurs.
 */
typedef struct Event_t Event;

/**
 * @brief Type of an event:
 * - XWALL: collision of a particle with a vertical wall (x = 0 or x = width);
 * - YWALL: collision of a particle with a horizontal wall (y = 0 or y = height);
 * - COLLISION: collision between two particles;
 * - NONE: no event.
 */
typedef enum {XWALL, YWALL, COLLISION, NONE} EventType;

/**
 * @brief Creates a new event.
 *
 * @param t     The type of the event.
 * @param p1    The (first) particle involved in the event (NULL if t is NONE).
 * @param p2    The second particle involved in the event (NULL if t is not
 *              COLLISION).
 * @param time  The (absolute) time at which the event occurs.
 * @return A pointer to the new event, or NULL in case of allocation error.
 */
Event *eventCreate(EventType t, Particle *p1, Particle *p2, double time);

/**
 * @brief Modifies all the fields of an existing event.
 *
 * @param e     The event to modify.
 * @param type  The new type of the event.
 * @param p1    The new (first) particle involved in the event.
 * @param p2    The new second particle involved in the event.
 * @param time  The new (absolute) time of the event.
 */
void eventSet(Event *e, EventType type, Particle *p1, Particle *p2, double time);

/**
 * @brief Frees an event (but not the particles it involves).
 *
 * @param e  The event to free.
 */
void eventFree(Event *e);

/**
 * @brief Returns the type of an event.
 *
 * @param event  An event.
 * @return The type of the event.
 */
EventType eventGetType(Event *event);

/**
 * @brief Returns the (first) particle involved in an event.
 *
 * @param e  An event.
 * @return The first particle of the event.
 */
Particle *eventGetParticle1(Event *e);

/**
 * @brief Returns the second particle involved in an event.
 *
 * @param e  An event.
 * @return The second particle of the event (NULL if the event is not of
 * type COLLISION).
 */
Particle *eventGetParticle2(Event *e);

/**
 * @brief Returns the (absolute) time at which an event occurs.
 *
 * @param e  An event.
 * @return The time of the event.
 */
double eventGetTime(Event *e);

/**
 * @brief Checks whether an event occurs strictly before another one.
 *
 * @param e1  The first event.
 * @param e2  The second event.
 * @return true if e1 occurs strictly before e2, false otherwise.
 */
bool eventOccursBefore(Event *e1, Event *e2);

#endif
