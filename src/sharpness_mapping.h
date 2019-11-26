#ifndef sharpness_mapping_H
#define sharpness_mapping_H

#include <vector>

#include "rgb_types.h"
#include "data_types.h"
#include "bitmap.h"

class bitmap;

class sharpness_mapping
{
    public:
        sharpness_mapping();
        sharpness_mapping(std::string s);
        sharpness_mapping(bitmap btmp);
        void fill_sharpness_map_buffer();
        void set_image( std::vector< std::vector<rgb_16> > image);
        void set_sharpness_map(std::vector< std::vector<float> > map);
        std::vector< std::vector<float> > get_sharpness_map();
        std::vector< std::vector<rgb_16> > get_img_rgb_16();
        std::vector< std::vector<float> > *get_sharpness_map_ptr();
        std::vector< std::vector<rgb_16> > *get_img_rgb_16_ptr();
    protected:
        std::vector< std::vector<float> > sharpness_map;
        std::vector< std::vector<ushort> > grayscale;
        std::vector< std::vector<rgb_16> > img_rgb_16;
        float bilinear_interpolation_float(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y);
};


#endif
