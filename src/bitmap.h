#ifndef bitmap_h
#define bitmap_h

#include <vector>
#include <string>
#include <math.h>

#undef Success
#include <Eigen/Dense>

#include "imageReader.h"
#include "sharpness_mapping.h"

class MyFrame;

enum padding {
    gray,
    mirror
};

class bitmap: public rawReader
{
    public:
        bitmap(std::string s);
        bitmap();
        void generateSharpnessMapSVD(MyFrame *f, int N = 32, int k = 6, padding p = mirror, bool updateGaugeBar = false);
        void generateGrayscale(int N, int k, padding p);
        void generateGrayscale(int padding_length, padding p);
        void openFileStr(std::string s);
        std::vector< std::vector<ushort> > get_grayscale();
        std::vector< std::vector<rgb_16> > get_rgb_image();
        std::vector< std::vector<float> > get_sharpness_map();
        void getDimensions(int &width, int &height);
        void fillMapBuffer(int w, int h, ushort* buffer);
        void fillOverlayBuffer(int width, int height, ushort alpha, ushort*buffer);
        void fillGrayscaleBuffer(int width, int height, ushort*buffer);
        
    private:
        
        Eigen::VectorXf SVs(int x, int y, int N);
        float svRatio(Eigen::VectorXf, int k);
        
        float precomputedSVratio(int x, int y, int k);
        float bilinearInterpolationV(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<float> > &map);
        ushort bilinearInterpolationV(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<ushort> > &map);
        
        rgb_16 map_float_to_rgb16(float f, std::vector<rgb_16> &colors);
        rgb_16 linear_interpolation(float x, int x1, int x2, rgb_16 c1, rgb_16 c2);
        
        std::vector< std::vector<ushort> > grayscale;
        std::vector< std::vector<float> > sharpness_map;
        std::vector< std::vector<rgb_16> > colored_sharpness_map;
        std::vector< std::vector<Eigen::VectorXf> > eigen_map;
        
        int N_computed;
        int nsv;
    
        bool updateGauge;
};

#endif
