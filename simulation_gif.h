#ifndef SIMULATION_GIF_H
#define SIMULATION_GIF_H

#include "simulator.h"
#include "gifenc.h"

/**
 * @brief Type representing an animated GIF of a simulation.
 */
typedef ge_GIF SimGIF;

/**
 * @brief Creates an animated GIF file to display a simulation. The size of
 * the image is proportional to the size of the box of the simulation.
 *
 * @param s         A simulator.
 * @param filename  The name of the GIF file to create.
 * @param maxsize   The size in pixels of the largest dimension of the image.
 * @return A pointer to the new GIF.
 */
SimGIF *createGif(Simulator *s, const char *filename, int maxsize);

/**
 * @brief Adds a frame to a GIF, representing the particles of a simulation at
 * the current time.
 *
 * @param s      A simulator.
 * @param gif    A GIF created with createGif.
 * @param delay  The display duration of the frame (in hundredths of second).
 */
void addFrame(Simulator *s, SimGIF *gif, uint16_t delay);

/**
 * @brief Writes the end of a GIF file, closes it and frees the GIF.
 *
 * @param gif  A GIF created with createGif.
 */
void closeGif(SimGIF *gif);

#endif
