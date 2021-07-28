#ifndef COLISION_H_INCLUDED
#define COLISION_H_INCLUDED

#include <stdbool.h>

inline static bool isColision(float x1, float y1, float z1,
                       float w1, float h1, float d1,
                       float x2, float y2, float z2,
                       float w2, float h2, float d2)
{

    return  (((x1 + w1) >= x2) && ((x2 + w2) >= x1))
            &&
            (((y1 + h1) >= y2) && ((y2 + h2) >= y1))
            &&
            (((z1 + d1) >= z2) && ((z2 + d2) >= z1));
}

#endif // COLISION_H_INCLUDED
