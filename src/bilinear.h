#ifndef bilinear_H
#define bilinear_H

#include <vector>

#include "data_types.h"
#include "rgb_types.h"

float bilinear_interpolation_f(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<ushort> > *grayscale);
float bilinear_interpolation_f(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<float> > *map);
ushort bilinear_interpolation_u(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<ushort> > *grayscale);
rgb_16 bilinear_interpolation_rgb_16(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<rgb_16> > *image);
rgba_16 bilinear_interpolation_rgba_16(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<rgba_16> > *image);
float linear_interpolation_f(float x1, float x2, float x);
rgba_16 linear_interpolation_rgba_16(float x1, float x2, float x, rgba_16 c1,  rgba_16 c2);
rgb_16 linear_interpolation_rgb_16(float x1, float x2, float x, rgba_16 c1,  rgba_16 c2);


#endif
