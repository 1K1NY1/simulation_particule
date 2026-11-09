#include <stdlib.h>
#include <stdio.h>
#include "particle.h"
#include "event.h"

struct Event_t
{
    EventType type;
    Particle *p1, *p2;
    double time;
};

Event *eventCreate(EventType type, Particle *p1, Particle *p2, double time)
{
    Event *e = malloc(sizeof(Event));

    if (!e)
    {
        fprintf(stderr, "eventCreate: allocation error.\n");
        exit(1);
    }

    e->type = type;
    e->p1 = p1;
    e->p2 = p2;
    e->time = time;

    return e;
}

void eventSet(Event *e, EventType type, Particle *p1, Particle *p2, double time)
{
    e->type = type;
    e->p1 = p1;
    e->p2 = p2;
    e->time = time;
}

void eventFree(Event *e)
{
    free(e);
}

EventType eventGetType(Event *e)
{
    return e->type;
}

Particle *eventGetParticle1(Event *e)
{
    return e->p1;
}

Particle *eventGetParticle2(Event *e)
{
    return e->p2;
}

double eventGetTime(Event *e)
{
    return e->time;
}

bool eventOccursBefore(Event *e1, Event *e2)
{
    return e1->time < e2->time;
}