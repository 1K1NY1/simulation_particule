#include <stdio.h>
#include <string.h>
#include <math.h>

#include "vector.h"
#include "particle.h"
#include "simulator.h"
#include "gifenc.h"

#include "simulation_gif.h"

static double _scale;

static void draw_circle(SimGIF *gif, Simulator *s, Vector center, double radius, Color color);

SimGIF *createGif(Simulator *s, const char *filename, int maxsize)
{
    double simw = simulatorGetWidth(s);
    double simh = simulatorGetHeight(s);
    double maxsimsize = simw > simh ? simw : simh;

    _scale = maxsize / maxsimsize;

    fprintf(stderr, "GIF: w=%d, h=%d\n", (uint16_t)(simulatorGetWidth(s) * _scale), (uint16_t)(simulatorGetHeight(s) * _scale));

    SimGIF *gif = ge_new_gif(
        filename,
        (uint16_t)(simulatorGetWidth(s) * _scale),
        (uint16_t)(simulatorGetHeight(s) * _scale),
        (uint8_t[]){
            /* palette */
            0xFF,
            0xFF,
            0xFF, /* 0 -> white */
            0x00,
            0x00,
            0x00, /* 1 -> black */
            0x20,
            0x98,
            0x4D, /* 2 -> green: #20984d */
            0x92,
            0x11,
            0x1C, /* 3 -> red: #92111c */
            0x47,
            0x62,
            0xFF, /* 4 -> blue: #4762ff*/
            0xFD,
            0x82,
            0x08, /* 5 -> orange: #fd8208*/
            0xA7,
            0x00,
            0xFF, /* 6 -> purple: #a700ff */
        },
        3, /* palette depth == log2(# of colors) */
        1  /* Play it once */
    );
    return gif;
}

void addFrame(Simulator *s, SimGIF *gif, uint16_t delay)
{
    for (int w = 0; w < gif->w; w++)
    {
        for (int h = 0; h < gif->h; h++)
        {
            gif->frame[h * gif->w + w] = 0;
        }
    }

    Particle *p;
    for (int i = 0; i < simulatorGetNumberOfParticles(s); i++)
    {
        p = simulatorGetParticle(s, i);

        draw_circle(
            gif,
            s,
            particleGetPosition(p),
            particleGetRadius(p),
            particleGetColor(p));
    }

    ge_add_frame(gif, delay);
}

void closeGif(SimGIF *gif)
{
    ge_close_gif(gif);
}

static void draw_circle(SimGIF *gif, Simulator *s, Vector center, double radius, Color color)
{
    int lower_x = floor(center.x - radius);
    if (lower_x < 0)
        lower_x = 0;

    int upper_x = ceil(center.x + radius);
    if (upper_x > simulatorGetWidth(s))
        upper_x = simulatorGetWidth(s);

    int lower_y = floor(center.y - radius);
    if (lower_y < 0)
        lower_y = 0;

    int upper_y = ceil(center.y + radius);
    if (upper_y > simulatorGetHeight(s))
        upper_y = simulatorGetHeight(s);

    double x_dist, y_dist;

    // fprintf(stderr,"wlow: %d whigh: %d, hlow: %d, hhigh: %d (h:%d, w:%d)\n", lower_x * _scale, upper_x*_scale, lower_y * _scale, upper_y * _scale, gif->h, gif->w);

    for (int w = lower_x * _scale; w < (int)(upper_x * _scale); w++)
    {
        for (int h = lower_y * _scale; h < (int)(upper_y * _scale); h++)
        {
            x_dist = ((((double)w) / _scale) - center.x);
            y_dist = ((((double)h) / _scale) - center.y);
            if ((x_dist * x_dist + y_dist * y_dist) <= (radius * radius))
            {
                gif->frame[(gif->h - h - 1) * gif->w + w] = color;
            }
        }
    }
}
