#include <stdlib.h>
#include <math.h>

#include "vector.h"

Vector vectorSum(Vector v1, Vector v2)
{
    return (Vector){
        v1.x + v2.x,
        v1.y + v2.y,
    };
}

Vector vectorDiff(Vector v1, Vector v2)
{
    return (Vector){
        v1.x - v2.x,
        v1.y - v2.y,
    };
}

Vector vectorMultByScalar(Vector v, double s)
{
    return (Vector){
        v.x * s,
        v.y * s,
    };
}

double vectorDotProduct(Vector v1, Vector v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

double vectorNorm(Vector v)
{
    return sqrt(vectorDotProduct(v, v));
}
