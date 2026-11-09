#ifndef VECTOR_H
#define VECTOR_H

/**
 * @brief A 2D vector (used for positions and velocities). This type is not
 * opaque: its fields can be accessed directly, and a vector can be created
 * with, e.g., (Vector){1.0, 2.0}.
 */
struct Vector_t
{
    double x;
    double y;
};

typedef struct Vector_t Vector;

/**
 * @brief Computes the sum of two vectors.
 *
 * @param v1  The first vector.
 * @param v2  The second vector.
 * @return v1 + v2.
 */
Vector vectorSum(Vector v1, Vector v2);

/**
 * @brief Computes the difference of two vectors.
 *
 * @param v1  The first vector.
 * @param v2  The second vector.
 * @return v1 - v2.
 */
Vector vectorDiff(Vector v1, Vector v2);

/**
 * @brief Multiplies a vector by a scalar.
 *
 * @param v  A vector.
 * @param s  A scalar.
 * @return s * v.
 */
Vector vectorMultByScalar(Vector v, double s);

/**
 * @brief Computes the dot product of two vectors.
 *
 * @param v1  The first vector.
 * @param v2  The second vector.
 * @return v1.x * v2.x + v1.y * v2.y.
 */
double vectorDotProduct(Vector v1, Vector v2);

/**
 * @brief Computes the (Euclidean) norm of a vector.
 *
 * @param v  A vector.
 * @return The norm of v.
 */
double vectorNorm(Vector v);

#endif
