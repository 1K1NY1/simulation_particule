#ifndef COLOR_H
#define COLOR_H

/**
 * @brief Color of a particle. The value of each color is its index in the
 * palette used to generate the GIF.
 */
typedef enum
{
    COLOR_WHITE = 0,
    COLOR_BLACK = 1,
    COLOR_GREEN = 2,
    COLOR_RED = 3,
    COLOR_BLUE = 4,
    COLOR_ORANGE = 5,
    COLOR_PURPLE = 6
} Color;

#endif
