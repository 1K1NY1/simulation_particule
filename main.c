#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "color.h"
#include "vector.h"
#include "particle.h"
#include "event.h"
#include "simulator.h"
#include "simulation_gif.h"

static double sample(double min, double max)
{
    double r = ((double)rand()) / RAND_MAX;

    return r * (max - min) + min;
}

static Simulator *createRandomSim(int num_particles)
{
    srand(time(NULL));
    const int height = 30;
    const int width = 30;
    const double min_speed = 0.8;
    const double max_speed = 1.2;
    const double min_radius = 0.5;
    const double max_radius = 1.2;

    Simulator *sim = simulatorCreate(width, height, num_particles);

    // Maximum number of random positions tried for each particle before
    // giving up (the box may be too small to contain all particles)
    const int max_attempts = 10000;

    double speed, ratio, radius;
    for (int i = 0; i < num_particles; i++)
    {
        speed = sample(min_speed, max_speed);
        ratio = sample(0.0, 1.0);
        radius = sample(min_radius, max_radius);
        bool added = false;
        for (int attempt = 0; attempt < max_attempts && !added; attempt++)
        {
            Particle *p = particleCreate(
                radius, radius * radius,
                (Vector){sample(0.0, width), sample(0.0, height)},
                (Vector){speed * ratio * (rand() % 2 ? -1 : 1), speed * (1 - ratio) * (rand() % 2 ? -1 : 1)},
                (Color)(COLOR_BLACK + rand() % 6));
            added = simulatorAddParticle(sim, p);
            if (!added)
                particleFree(p);
        }
        if (!added)
        {
            fprintf(stderr, "Warning: could only place %d particles out of %d.\n",
                    i, num_particles);
            break;
        }
    }
    return sim;
}

static Simulator *LoadSimFromFile(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return NULL;
    }

    double width, height;
    int n;

    if (fscanf(file, "%lf %lf %d", &width, &height, &n) != 3)
    {
        fprintf(stderr, "Error: Failed to read header (width, height, N).\n");
        fclose(file);
        return NULL;
    }

    if (width <= 0 || height <= 0 || n <= 0)
    {
        fprintf(stderr, "Error: Invalid header values.\n");
        fclose(file);
        return NULL;
    }

    fprintf(stderr, "w=%f, h=%f, n=%d\n", width, height, n);

    Simulator *s = simulatorCreate(width, height, n);

    if (s == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for simulator.\n");
        fclose(file);
        return NULL;
    }

    int num_ignored = 0;
    for (int i = 0; i < n; i++)
    {
        double px, py, vx, vy, mass, radius;
        int color;
        int read_items = fscanf(file, "%lf %lf %lf %lf %lf %lf %d",
                                &px, &py, &vx, &vy, &mass, &radius, &color);
        if (read_items != 7)
        {
            fprintf(stderr, "Error: Malformed file.\n");
            simulatorFree(s);
            fclose(file);
            return NULL;
        }
        if (color < COLOR_WHITE || color > COLOR_PURPLE)
        {
            fprintf(stderr, "Warning: invalid color %d for particle %d, black used instead.\n", color, i);
            color = COLOR_BLACK;
        }
        Particle *p = particleCreate(radius, mass, (Vector){px, py}, (Vector){vx, vy}, (Color)color);
        if (!simulatorAddParticle(s, p))
        {
            num_ignored++;
            particleFree(p);
        }
    }

    if (num_ignored > 0)
        fprintf(stderr, "Warning: %d particles overlapping another particle or a wall were ignored.\n",
                num_ignored);

    fclose(file);
    return s;
}

static void print_usage(const char *prog_name)
{
    printf("Usage: %s [options]\n", prog_name);
    printf("Options:\n");
    printf("  -f <file>      Load particles from txt file\n");
    printf("  -n <number>    Number of random particles if -f is not specified (default: 20)\n");
    printf("  -s <steps>     Number of simulation iterations (default: 500)\n");
    printf("  -t <dt>        Time step size (default: 0.5)\n");
    printf("  -g <size>      GIF max size in pixels (default: 1000)\n");
    printf("  -o <file>      The name of the generated GIF file (default: simulation.gif)\n");
    printf("  -h             Show this help message\n");
}

int main(int argc, char *argv[])
{
    // default parameter values
    double dt = 0.5;
    int nb_steps = 500;
    int num_particles = 20;
    char *filename = NULL;
    char *giffilename = "simulation.gif";
    int gifmaxsize = 1000;

    // Command line argument handling
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc)
        {
            filename = argv[++i];
        }
        else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc)
        {
            num_particles = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
        {
            nb_steps = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc)
        {
            dt = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-g") == 0 && i + 1 < argc)
        {
            gifmaxsize = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
        {
            giffilename = argv[++i];
        }
        else if (strcmp(argv[i], "-h") == 0)
        {
            print_usage(argv[0]);
            return 0;
        }
    }

    Simulator *sim = NULL;
    if (filename != NULL)
    {
        sim = LoadSimFromFile(filename);
    }
    else
    {
        sim = createRandomSim(num_particles);
    }

    if (sim == NULL)
    {
        fprintf(stderr, "Error: Could not initialize simulation.\n");
        return 1;
    }

    // Simulator *sim = createRandomSim(20);

    fprintf(stderr, "Number of particles that will be simulated: %d\n", simulatorGetNumberOfParticles(sim));

    SimGIF *gif = createGif(sim, giffilename, gifmaxsize);

    addFrame(sim, gif, 5);

    printf("Initial average particule energy: %.2f\n",
           simulatorStatParticles(sim, particleKineticEnergy));

    for (int t = 0; t < nb_steps; t++)
    {
        fprintf(stderr, "t = %d\r", t);

        simulatorStep(sim, dt);

        addFrame(sim, gif, 5);
    }

    closeGif(gif);

    printf("Number of collisions between particles during simulation: %d\n",
           simulatorGetNumCollisions(sim));
    printf("Average number of collisions per particle: %.2f\n",
           simulatorStatParticles(sim, particleGetNumCollisions));
    printf("Average particule energy: %.2f\n",
           simulatorStatParticles(sim, particleKineticEnergy));

    simulatorFree(sim);

    return 0;
}
