#pragma once

#include <math.h>

constexpr float RAD_CONVERSION = M_PI / 180.f;
constexpr float DEG_CONVERSION = 180.f / M_PI;
#define DegToRad(X) X * RAD_CONVERSION
#define RadToDeg(X) X * DEG_CONVERSION