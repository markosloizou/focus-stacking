#ifndef imageReader_h
#define imageReader_h

#include <iostream>
#include <vector>
#include <math.h>

#include "libraw/libraw.h"

#include "rgb_types.h"

#include "data_types.h"

class rawReader
{
    public:
        rawReader(); //constructor
        void openFile(char *file);
        void printData();
        void close();
        void printMetadata();
        void generateBMP();
        void fillBuffer(int w, int h, ushort* buffer);
        void SaveImage();
        void getOriginalDimensions(int &width, int &height);
        
    protected:
        rgb_16 bilinearInterpolation(coordinate c1, coordinate c2, coordinate c3, coordinate c4, float x, float y, std::vector< std::vector <rgb_16> >  &img);
        
        LibRaw iProcessor;
        std::vector< std::vector<rgb_16> > img_rgb_16;
        int w,h;
        
        
};

#endif
