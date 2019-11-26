#include "bilinear.h"

float bilinear_interpolation_f(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<ushort> > *grayscale)
{
    float F_xy1, F_xy2;
    float F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes
    
    if(!(c21.x == x) && !(c11.x == x))
    {
        F_xy1 = (c21.x - x) * (*grayscale)[c11.x][c11.y];
        F_xy1 += (x - c11.x) * (*grayscale)[c21.x][c21.y];
        
        F_xy2 = (c22.x - x) * (*grayscale)[c12.x][c12.y];
        F_xy2 += (x - c12.x) * (*grayscale)[c22.x][c22.y];
    }
    else
    {
        F_xy1 = (*grayscale)[c11.x][c11.y];
        
        F_xy2 = (*grayscale)[c11.x][c22.y];
    }
    //perform linear interpolation in the y direction
    if(!(c22.y == y) && (c11.y == y))
    {
        F_xy = (c22.y - y) * F_xy1 + (y - c11.y) *F_xy2;
    }
    else
    {
        F_xy = F_xy1;
        
    }
    
    
    return F_xy;
}


float bilinear_interpolation_f(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<float> > *map)
{
    float F_xy1, F_xy2;
    float F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes
    
    if(!(c21.x == x) && !(c11.x == x))
    {
        F_xy1 = (c21.x - x) * (*map)[c11.x][c11.y];
        F_xy1 += (x - c11.x) * (*map)[c21.x][c21.y];
        
        F_xy2 = (c22.x - x) * (*map)[c12.x][c12.y];
        F_xy2 += (x - c12.x) * (*map)[c22.x][c22.y];
    }
    else
    {
        F_xy1 = (*map)[c11.x][c11.y];
        
        F_xy2 = (*map)[c11.x][c22.y];
    }
    //perform linear interpolation in the y direction
    if(!(c22.y == y) && !(c11.y == y))
    {
        F_xy = (c22.y - y) * F_xy1 + (y - c11.y) *F_xy2;
    }
    else
    {
        F_xy = F_xy1;
        
    }
    
//     std::cout << "Map" << std::endl;
//     std::cout << "c11: " << (*map)[c11.x][c11.y] << std::endl;
//     std::cout << "c12: " << (*map)[c11.x][c11.y] << std::endl;
//     std::cout << "c21: " << (*map)[c11.x][c11.y] << std::endl;
//     std::cout << "c22: " << (*map)[c11.x][c11.y] << std::endl;
//     
//     
//     std::cout << "F_xy1: " << F_xy1 << std::endl;
//     std::cout << "F_xy2: " << F_xy2 << std::endl;
//     std::cout << "F_xy: " << F_xy << std::endl;
    
    
    return F_xy;
}


ushort bilinear_interpolation_u(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<ushort> > *grayscale)
{
    float f = bilinear_interpolation_f(c11, c12, c21, c22, x, y, grayscale);
    return (ushort) f;
}


rgb_16 bilinear_interpolation_rgb_16(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<rgb_16> > *img)
{
    rgb_16 F_xy1, F_xy2;
    rgb_16 F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes
    
    if(!(c21.x == x) && !(c11.x == x))
    {
        F_xy1 = (c21.x - x) * (*img)[c11.x][c11.y];
        F_xy1 += (x - c11.x) * (*img)[c21.x][c21.y];
        
        F_xy2 = (c22.x - x) * (*img)[c12.x][c12.y];
        F_xy2 += (x - c12.x) * (*img)[c22.x][c22.y];
    }
    else
    {
        F_xy1 = (*img)[c11.x][c11.y];
        
        F_xy2 = (*img)[c11.x][c22.y];
    }
    //perform linear interpolation in the y direction
    if(!(c22.y == y) && (c11.y == 0))
    {
        F_xy = (c22.y - y) * F_xy1 + (y - c11.y) *F_xy2;
    }
    else
    {
        F_xy = F_xy1;
        
    }
    return F_xy;
}



rgba_16 bilinear_interpolation_rgba_16(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<rgba_16> > *img)
{
    rgba_16 F_xy1, F_xy2;
    rgba_16 F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes
    
    if(!(c21.x == x) && !(c11.x == x))
    {
        F_xy1 = (c21.x - x) * (*img)[c11.x][c11.y];
        F_xy1 += (x - c11.x) * (*img)[c21.x][c21.y];
        
        F_xy2 = (c22.x - x) * (*img)[c12.x][c12.y];
        F_xy2 += (x - c12.x) * (*img)[c22.x][c22.y];
    }
    else
    {
        F_xy1 = (*img)[c11.x][c11.y];
        
        F_xy2 = (*img)[c11.x][c22.y];
    }
    //perform linear interpolation in the y direction
    if(!(c22.y == y) && (c11.y == 0))
    {
        F_xy = (c22.y - y) * F_xy1 + (y - c11.y) *F_xy2;
    }
    else
    {
        F_xy = F_xy1;
        
    }
    
    
    return F_xy;
}



float linear_interpolation(float x1, float x2, float x)
{
    return (x - x1) / (x2 - x1);
    
}

rgba_16 linear_interpolation_rgba_16(float x1, float x2, float x, rgba_16 c1,  rgba_16 c2)
{
    return (x-x1)*c1 + (x2-x)*c2;
}
