#ifndef svd_H
#define svd_H


#include "sharpness_mapping.h"
#include "data_types.h"
#include "rgb_types.h"

class svd : public sharpness_mapping
{
    private:
        Eigen::VectorXf SVs(int x, int y, int N);
        float svRatio(Eigen::VectorXf, int k);
        
        float precomputedSVratio(int x, int y, int k);
};


#endif
