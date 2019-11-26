#ifndef lbp_H
#define lbp_H

#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#include "bitmap.h"
#include "sharpness_mapping.h"
#include "data_types.h"
#include "gui.h"

//add if work has been done for a larger region radius no need to redo everything 
class lbp: public sharpness_mapping
{
    public:
        lbp(bitmap* image, MyFrame *f);
        lbp(std::string filename);
        ~lbp();
        void generate_sharpness_map(float threshold, float r, float region_radius, bool updateGaugeBar = false);
        
    private:      
        MyFrame *frame;
        bitmap *image;
        int pattern;
        float radius;
        float region_radius;
        float threashold;
        int w;
        int h;
        bool updateGauge;
        bool inner_pointer;
        std::vector< std::vector < ushort> > code_map;
        
        float measure(int x, int y);
        
        int point_measure(int pixel, int current_pixel, float threshold);
        int measure(int x, int y, float radius, float threshold);
        void generate_lbp_per_pixel(float radius, float threshold);
        void generate_sharpness_per_pixel(float region_radius);
        float sharpness(int x, int y);
    
};

#endif
