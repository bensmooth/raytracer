#pragma once

#include "IShader.h"


/**
 * This shader alternates between white and black every 1 unit.
 */
class ReferenceTileShader : public IShader
{
public:
    virtual Color Shade(Intersection& intersection);
};
