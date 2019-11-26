#include "sharp_image.h"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui.hpp>

sharp_image::sharp_image()
{
    min_sharpness = 0;
}


//constructor to initialize empty image with given dimeninsions, used to update the image slowly
sharp_image::sharp_image(int width, int height)
{
    min_sharpness = 0;
    
    w = width;
    h = height;
    
    rgb_16 black(0,0,0);
    float sharpness = -1;
    
    for(int i = 0; i < h; i++)
    {
        std::vector<rgb_16> c;
        std::vector<float> f;
        for(int j = 0; j < w; j++)
        {
            c.push_back(black);
            f.push_back(sharpness);
        }
        sharpness_map.push_back(f);
        image.push_back(c);
    }
}

sharp_image::sharp_image(std::vector< std::vector<rgb_16> > img, std::vector< std::vector<float> > sharpness)
{
    min_sharpness = 0;
    
    sharpness_map = sharpness;
    image = img; 
    
    h = sharpness_map.size() - 1;
    w = sharpness_map[0].size() - 1;
}

void sharp_image::create_empty_image(int width, int height)
{
    w = width;
    h = height;
    
    rgb_16 black(0,0,0);
    float sharpness = -1;
    if(image.size()!= 0) image.clear();
    if(sharpness_map.size()!=0) sharpness_map.clear();
    for(int i = 0; i < h; i++)
    {
        std::vector<rgb_16> c;
        std::vector<float> f;
        for(int j = 0; j < w; j++)
        {
            c.push_back(black);
            f.push_back(sharpness);
        }
        sharpness_map.push_back(f);
        image.push_back(c);
    }
}

void sharp_image::set_image(std::vector< std::vector<rgb_16> > img, std::vector< std::vector<float> > sharpness)
{
    sharpness_map = sharpness;
    image = img; 
    
    h = sharpness_map.size() - 1;
    w = sharpness_map[0].size() - 1;
}

void sharp_image::set_image_only(std::vector< std::vector<rgb_16> > img)
{
    image = img; 
    
    h = sharpness_map.size() - 1;
    w = sharpness_map[0].size() - 1;
}

void sharp_image::update_image(std::vector< std::vector<rgb_16> > *img, std::vector< std::vector<float> > *map)
{
    mutex.lock();
    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            if((*map)[i][j] > sharpness_map[i][j] && (*map)[i][j] > min_sharpness)
            {
                image[i][j] = (*img)[i][j];
                sharpness_map[i][j] = (*map)[i][j];
            }
        }
    }
    mutex.unlock();
}



void sharp_image::fill_img_buffer(int width, int height, ushort alpha ,ushort *buffer)
{
    float x,y; 
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            x = (float)i/height * h;
            y = (float)j/width * w;
            
            
            rgb_16 current_pixel;
            
            coordinate c11,c12,c21,c22;
            
            c11.x = (int)floor(x);
            c11.y = (int)floor(y);
            
            c12.x = (int)floor(x);
            c12.y = (int)ceil(y);
            
            c21.x = (int)ceil(x);
            c21.y = (int)floor(y);
            
            c22.x = (int)ceil(x);
            c22.y = (int)ceil(y);
            
            
            current_pixel = bilinear_interpolation_rgb_16(c11,c12,c21,c22, x,y, &image);
            
            buffer[4*(i*width + j) + 0] = current_pixel.R;
            buffer[4*(i*width + j) + 1] = current_pixel.G;
            buffer[4*(i*width + j) + 2] = current_pixel.B;
            buffer[4*(i*width + j) + 3] = alpha;
        }
    }
}

void sharp_image::fill_img_buffer(int width, int height, ushort *buffer)
{
    float x,y; 
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            x = (float)i/height * h;
            y = (float)j/width * w;
            
            
            rgb_16 current_pixel;
            
            coordinate c11,c12,c21,c22;
            
            c11.x = (int)floor(x);
            c11.y = (int)floor(y);
            
            c12.x = (int)floor(x);
            c12.y = (int)ceil(y);
            
            c21.x = (int)ceil(x);
            c21.y = (int)floor(y);
            
            c22.x = (int)ceil(x);
            c22.y = (int)ceil(y);
            
            
            current_pixel = bilinear_interpolation_rgb_16(c11,c12,c21,c22, x,y, &image);
            

            
            buffer[3*(i*width + j) + 0] = current_pixel.R;
            buffer[3*(i*width + j) + 1] = current_pixel.G;
            buffer[3*(i*width + j) + 2] = current_pixel.B;
        }
    }
}

void sharp_image::fill_map_buffer(int width, int height,ushort *buffer, std::vector<rgba_16> mapping)
{
    float x,y; 
    int length = mapping.size();
    
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            x = (float)i/height * h;
            y = (float)j/width * w;
            
            
            rgba_16 current_pixel;
            
            coordinate c11,c12,c21,c22;
            
            c11.x = (int)floor(x);
            c11.y = (int)floor(y);
            
            c12.x = (int)floor(x);
            c12.y = (int)ceil(y);
            
            c21.x = (int)ceil(x);
            c21.y = (int)floor(y);
            
            c22.x = (int)ceil(x);
            c22.y = (int)ceil(y);
            
//             std::cout << "C11: " << c11 << std::endl;
//             std::cout << "C12: " << c12 << std::endl;
//             std::cout << "C21: " << c21 << std::endl;
//             std::cout << "C22: " << c22 << std::endl;
//             std::cout << "x  : " << x << std::endl;
//             std::cout << "y  : " << y << std::endl;
            
            
            float sharpness = bilinear_interpolation_f(c11,c12,c21,c22, x,y, &sharpness_map);
            if(sharpness < min_sharpness) current_pixel = rgba_16(0,0,0,0);
            else
            {
                float current_point = sharpness * length - 1;  // so that floor and ceil are the less and greater than element in the array of colors 
                current_pixel = linear_interpolation_rgba_16(floor(current_point), ceil(current_point), current_point, mapping[(int)floor(current_point)], mapping[(int)ceil(current_point)]);
            }
            
            
            buffer[4*(i*width + j) + 0] = current_pixel.R;
            buffer[4*(i*width + j) + 1] = current_pixel.G;
            buffer[4*(i*width + j) + 2] = current_pixel.B;
            buffer[4*(i*width + j) + 3] = current_pixel.A;
        }
    }
}

void sharp_image::get_dimensions(int &width, int &height)
{
    width = w;
    height = h;
}

void sharp_image::getOriginalDimensions(int &width, int &height)
{
    width = w;
    height = h;
}


float sharp_image::getSharpness(int x, int y, int width, int height)
{
    float xx,yy; 
    
    
    xx = (float)x/height * h;
    yy = (float)y/width * w;
    
    coordinate c11,c12,c21,c22;
    
    c11.x = (int)floor(xx);
    c11.y = (int)floor(yy);
    
    c12.x = (int)floor(xx);
    c12.y = (int)ceil(yy);
    
    c21.x = (int)ceil(xx);
    c21.y = (int)floor(yy);
    
    c22.x = (int)ceil(xx);
    c22.y = (int)ceil(yy);

    
    
    float sharpness = bilinear_interpolation_f(c11,c12,c21,c22, xx,yy, &sharpness_map);
    
   return sharpness;
}

void sharp_image::set_min_sharpness(float s)
{
    min_sharpness = s;
}



void sharp_image::save_file(std::string file)
{
    if(file.find(".tif") == std::string::npos)
    {
        file += ".tiff";
    }
    cv::Mat mat(h, w, CV_16UC3);
    
    //color order bgr
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            cv::Vec3s& bgr = mat.at<cv::Vec3s>(h-i-1, j);
//             std::cout << "(i,j) = (" << i << "," << j << ")" << std::endl;
            bgr[0] = image[i][j].B;
            bgr[1] = image[i][j].G;
            bgr[2] = image[i][j].R;
        }
    }
    
    
    try {
        imwrite(file, mat);
        std::cout << "opencv file save finished" << std::endl;
    } 
    catch (std::runtime_error& ex) {
        std::cout << ex.what() << std::endl;
    }
    


}
