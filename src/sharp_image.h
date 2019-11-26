#ifndef sharp_image_H
#define sharp_image_H

#include <vector>
#include <thread>
#include <mutex>
#include <cmath>


#include "tiffio.h"

#include "data_types.h"
#include "rgb_types.h"
#include "bilinear.h"

class sharp_image
{
    public:
        sharp_image();
        sharp_image(int width, int height);
        sharp_image(std::vector< std::vector<rgb_16> > img, std::vector< std::vector<float> > sharpness);
        void create_empty_image(int width, int height);
        void set_image_only(std::vector< std::vector<rgb_16> > img);
        void set_image(std::vector< std::vector<rgb_16> > img, std::vector< std::vector<float> > sharpness);
        void update_image(std::vector< std::vector<rgb_16> > *img, std::vector< std::vector<float> > *map);
        void fill_img_buffer(int width, int height, ushort alpha ,ushort *buffer);
        void fill_img_buffer(int width, int height, ushort *buffer);
        void fill_map_buffer(int width, int height,ushort *buffer, std::vector<rgba_16> mapping);
        void get_dimensions(int &width, int &height);
        void getOriginalDimensions(int &width, int &height);
        float getSharpness(int x, int y, int width, int height);
        void set_min_sharpness(float min);
        void save_file(std::string s);
    private:
        int w;
        int h;
        float min_sharpness; 
        std::mutex mutex;
        
        std::vector< std::vector<float> > sharpness_map;
        std::vector< std::vector<rgb_16> > image;
};

#endif
