#include "lbp.h"

lbp::lbp(std::string filename)
{
    inner_pointer = true;
    image = new bitmap(filename);    
}

lbp::lbp(bitmap *img, MyFrame *f)
{
    inner_pointer = false;
    frame = f;
    image =  img;
}

lbp::~lbp()
{
    if(inner_pointer) delete image;

}

void lbp::generate_sharpness_map(float threshold, float r, float region_r, bool updateGaugeBar)
{
    updateGauge = updateGaugeBar;//used to check if the application will update gauge bar or not
    radius = r;
    region_radius = region_r;
    image->generateGrayscale((int) (ceil(radius) + ceil(region_radius)), mirror);
    grayscale = image->get_grayscale();
    std::cout << "grayscale dimensions: " << grayscale.size() << " x " << grayscale[0].size() << std::endl;
    img_rgb_16 = image->get_rgb_image();
    image->getDimensions(w,h);
    
    if(sharpness_map.size() != 0)
    {
        sharpness_map.erase(sharpness_map.begin(), sharpness_map.end());
    }
    
    //TODO check if for loops are in the correct order for i and j
    if(code_map.size() == 0)
    {
        for(int i = 0; i < h + 2*(int) ceil(region_radius); i++)
        {
            std::vector<ushort> v;
            for(int j = 0; j < w + 2*(int) ceil(region_radius); j++)
            {
                v.push_back(0);
            }
            
            code_map.push_back(v);
        }
    }
    else
    {
        for(int i = 0; i < h + 2*(int) ceil(region_radius); i++)
        {
            for(int j = 0; j < w + 2*(int) ceil(region_radius); j++)
            {
                code_map[i][j] = 0; 
            }
        }
    }
    if(sharpness_map.size() == 0)
    {
        for(int i = 0; i < h; i++)
        {
            std::vector<float> v;
            for(int j = 0; j < w; j++)
            {
                v.push_back(0.0);
            }
            sharpness_map.push_back(v);
        }
    }
    else
    {
        for(int i = 0; i < h; i++)
        {
            for(int j = 0; j < w; j++)
            {
                sharpness_map[i][j] = 0.0; 
            }
        }
    }
    
    
    generate_lbp_per_pixel(radius, threshold);
    generate_sharpness_per_pixel(region_radius);
    
}

int lbp::measure(int x, int y, float radius, float threshold)
{
    int pattern[8];
    int current_pixel = (int)grayscale[x][y];
    
    for(int i = 0; i < 8; i++)
    {
        float xx,yy;
        coordinate c11, c12, c21, c22;
        int pixel;
        

        xx = x + radius*cos(2*M_PI / 8 *i);
        yy = y + radius*sin(2*M_PI / 8 *i);

        c11.x = floor(xx);
        c11.y = floor(yy);
        
        c12.x = ceil(xx);
        c12.y = floor(yy);
        
        c21.x = floor(xx);
        c21.y = ceil(yy);
        
        c22.x = ceil(xx);
        c22.y = ceil(yy);
        
        pixel = (int) round(bilinear_interpolation_float(c11,c12,c21,c22, xx,yy) ); 
        
        pattern[i] =  point_measure(pixel,current_pixel,threshold);
        
//         std::cout << "pixel: " << pixel << "\n" << "current_pixel: " << current_pixel << "\nvalue: " << abs((float)current_pixel-pixel)/pow(2,16)<<"\ntriggered: "<< pattern[i] <<std::endl;
    }
    
    int triggered = 0;
    int counter = 0;
    
    for(int i = 0; i < 8; i++)
    {
        if(pattern[i] == 1) triggered++;
//         std::cout << "pattern[" << i << "]: " << pattern[i] << std::endl;
    }
    
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(pattern[(j + i) % 8] == 1) counter++;
            else counter = 0;
            
            if(counter == triggered)
            {
//                 std::cout << "pattern: " << triggered << "\n\n" << std::endl; 
                return triggered;
            }
        }
    }
//     std::cout << "pattern: " << 9 << "\n\n" << std::endl; 
    
    return 9;
    
}

void lbp::generate_lbp_per_pixel(float radius, float threshold)
{
    
    for(int i = 0; i < h + (int) 2*ceil(region_radius); i++)
    {
        if(updateGauge) frame -> setGauge((int) round((float)i*100.0 / (float)(2*(h + (int) 2*ceil(region_radius)))));
        for(int j = 0; j < w+ (int) 2*ceil(region_radius); j++)
        {
            code_map[i][j] = measure(i + (int)ceil(radius), j + (int) ceil(radius), radius, threshold);
        }
    }
}

int lbp::point_measure(int pixel,int current_pixel, float threshold)
{
    int difference = pixel - current_pixel;
    difference = abs(difference);
    
    if((float)difference / (pow(2,16)-1) < threshold) return 0;
    else return 1;
}

void lbp::generate_sharpness_per_pixel(float region_radius)
{
    std::cout << "w: " << w << std::endl;
    std::cout << "h: " << h << std::endl;
    
    for(int i = 0; i < h; i++)
    {
        if(updateGauge) frame -> setGauge((int) round((float)i*100.0 / (float)(2*(h + (int) 2*ceil(region_radius))) + 50.0 ));
        for(int j = 0; j < w; j++)
        {
            sharpness_map[i][j] = sharpness(i+(int)ceil(region_radius),j+(int)ceil(region_radius));
//             std::cout << "sharpness_map[" << i << "][" << j <<"] = "<< sharpness_map[i][j] << "\n\n" << std::endl;
        }
    }
}

float lbp::sharpness(int x, int y)
{
    int area = 0;
    int  counter = 0;
    for(int i = 0; i < (int) ceil(region_radius); i++)
    {
        for(int j = 0; j < sqrt(region_radius*region_radius - i*i); j++)
        {
            area += 4;
            if(code_map[x + i][y + j]  >= 6)
            {
                counter++;
            }
            if(code_map[x + i][y - j]  >= 6)
            {
                counter++;
            }
            if(code_map[x - i][y + j]  >= 6)
            {
                counter++;
            }
            if(code_map[x - i][y - j]  >= 6)
            {
                counter++;
            }
        }
    }
//     std::cout << "sharpness_map[" << x << "][" << y <<"] = "<< (float)counter/(float)area << std::endl;
//     std::cout << "counter: " << counter << "\narea: " << area << std::endl;
    return (float)counter/(float)area;
}
