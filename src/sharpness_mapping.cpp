#include "sharpness_mapping.h"

float sharpness_mapping::bilinear_interpolation_float(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y)
{
    float F_xy1, F_xy2;
    float F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes

    if(!(c21.x == x) && (c11.x == x))
    {
        F_xy1 = (c21.x - x) * grayscale[c11.x][c11.y];
        F_xy1 += (x - c11.x) * grayscale[c21.x][c21.y];
        
        F_xy2 = (c22.x - x) * grayscale[c12.x][c12.y];
        F_xy2 += (x - c12.x) * grayscale[c22.x][c22.y];
    }
    else
    {
        F_xy1 = grayscale[c11.x][c11.y];
        
        F_xy2 = grayscale[c11.x][c22.y];
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



void sharpness_mapping::set_image( std::vector< std::vector<rgb_16> > image)
{
    img_rgb_16 = image;
}
void sharpness_mapping::set_sharpness_map(std::vector< std::vector<float> > map)
{
    sharpness_map = map;
}

sharpness_mapping::sharpness_mapping()
{
    
}

sharpness_mapping::sharpness_mapping(bitmap btmp)
{
    
}


std::vector< std::vector<float> > sharpness_mapping::get_sharpness_map()
{
    return sharpness_map;
}
std::vector< std::vector<rgb_16> > sharpness_mapping::get_img_rgb_16()
{
    return img_rgb_16;
}

std::vector< std::vector<float> > *sharpness_mapping::get_sharpness_map_ptr()
{
    return &sharpness_map;
}
std::vector< std::vector<rgb_16> > *sharpness_mapping::get_img_rgb_16_ptr()
{
    return &img_rgb_16;
}
