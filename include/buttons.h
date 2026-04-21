#pragma once
#include "particle.h"

struct Button
{
    int x, y, w, h;      // window-space rect, top-left origin
    Particle particle;
};
