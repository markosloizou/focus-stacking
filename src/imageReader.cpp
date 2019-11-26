#include "imageReader.h"

rawReader::rawReader()
{
}


void rawReader::openFile(char *file)
{
    if(img_rgb_16.size() != 0) img_rgb_16.clear();
    iProcessor.open_file(file);

    std::cout<< "Image size: " << iProcessor.imgdata.sizes.width << " x " << iProcessor.imgdata.sizes.height << std::endl;
    iProcessor.imgdata.params.use_camera_wb = 1;
    iProcessor.imgdata.params.user_flip = 1;
    
    iProcessor.imgdata.params.output_bps = 16;
    iProcessor.unpack(); // unpack imageReader

    iProcessor.dcraw_process();
}

void rawReader::printData()
{
    for(int i = 0; i < iProcessor.imgdata.sizes.width*iProcessor.imgdata.sizes.height; i++)
    {
        std::cout << "i = " << i;
        std::cout << " R = " << iProcessor.imgdata.image[i][0];
        std::cout << " G = " << iProcessor.imgdata.image[i][1];
        std::cout << " B = " << iProcessor.imgdata.image[i][2];
        std::cout << " G2 = " << iProcessor.imgdata.image[i][3] << std::endl;
    }
}

void rawReader::close()
{
    iProcessor.recycle();
}

void rawReader::printMetadata()
{
    std::cout<< "Image size: " << iProcessor.imgdata.sizes.width << " x " << iProcessor.imgdata.sizes.height << std::endl;
}

void rawReader::generateBMP()
{
    int err;
    libraw_processed_image_t* img = iProcessor.dcraw_make_mem_image(&err);
    std::cout << "Error: " << err <<std::endl;
    
    w = (*img).width;
    h = (*img).height;
    
    std::cout << "Bit depth: " << img->bits << std::endl;
    //img_rgb_16.erase();
    
//     std::cout << "Raw Image size: " << w << " x " << h << std::endl;
//     std::cout << "here" << std::endl;
//     std::cout << "Data size:" << (*img).data_size << std::endl;
//     std::cout << "here 2" << std::endl;
//     std::cout << "data: " << (ushort)img->data[0]<< std::endl;
//     std::cout << "data: " << (ushort)img->data[1]<< std::endl;
//     std::cout << "data: " << (ushort)img->data[2]<< std::endl;
//     std::cout << "here 3" << std::endl;
    
    rgb_16 c;
    rgb_16 max, min;

    max.G = 0;
    max.R = 0;
    max.B = 0;
    
    min.G = 255;
    min.R = 255;
    min.B = 255;
    
    for(int i = h-1; i >= 0; i--)
    {
        std::vector<rgb_16> row;
        for(int j = w-1; j >= 0; j--)
        {
//             c.R = (ushort)img->data[6*(i*w+j) + 0]*256 + img->data[6*(i*w+j) + 1];
//             c.G = (ushort)img->data[6*(i*w+j) + 2]*256 + img->data[6*(i*w+j) + 3];
//             c.B = (ushort)img->data[6*(i*w+j) + 4]*256 + img->data[6*(i*w+j) + 5];
            
            c.R = (ushort)img->data[6*(i*w+j) + 0] + img->data[6*(i*w+j) + 1]*256;
            c.G = (ushort)img->data[6*(i*w+j) + 2] + img->data[6*(i*w+j) + 3]*256;
            c.B = (ushort)img->data[6*(i*w+j) + 4] + img->data[6*(i*w+j) + 5]*256;
            
            row.push_back(c);
            
            if(c.R > max.R) max.R = c.R;
            else if(c.R < min.R) min.R = c.R;
            
            if(c.B > max.B) max.B = c.B;
            else if(c.B < min.B) min.B = c.B;
            
            if(c.G > max.G) max.G = c.G;
            else if(c.G < min.G) min.G = c.G;
        }
        img_rgb_16.push_back(row);
    }
    std::cout << "R max. = " << max.R << std::endl;
    std::cout << "G max. = " << max.G << std::endl;
    std::cout << "B max. = " << max.B << std::endl;
    
    std::cout << "R min. = " << min.R << std::endl;
    std::cout << "G min. = " << min.G << std::endl;
    std::cout << "B min. = " << min.B << std::endl;
    delete img;
}

void rawReader::fillBuffer(int width, int height, ushort*buffer)
{
    float tol = 1e-2;
    float x,y; 
    
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            x = (float)i/height * h;
            y = (float)j/width * w;
            
//             if( (x - round(x)) < tol && (y - round(y)) < tol)
//             {
//                 x = (int) round(x);
//                 y = (int) round(y);
//                 rgb_16 c  = img_rgb_16[x][y];
//                 
//                 buffer[3*(i*width + j) + 0] = c.R;
//                 buffer[3*(i*width + j) + 1] = c.G;
//                 buffer[3*(i*width + j) + 2] = c.B;
//             }
//             else
//             {
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
                
                current_pixel = bilinearInterpolation(c11,c12,c21,c22,x,y, img_rgb_16);
                
                buffer[3*(i*width + j) + 0] =  current_pixel.R;
                buffer[3*(i*width + j) + 1] =  current_pixel.G;
                buffer[3*(i*width + j) + 2] =  current_pixel.B;
//             }
        }
    }
}

rgb_16 rawReader::bilinearInterpolation(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector< std::vector<rgb_16> > &img)
{
    rgb_16 F_xy1, F_xy2, F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes
    
    //TODO have removed denomenators, check perforkamce
    if(!(c21.x == x) && (c11.x == x))
    {
        F_xy1 =  (c21.x - x) * img[c11.x][c11.y] +  (x - c11.x) * img[c21.x][c21.y];
        F_xy2 =  (c22.x - x) * img[c12.x][c12.y] +  (x - c12.x) * img[c22.x][c22.y];

    }
    else
    {
        F_xy1 = img[c11.x][c11.y];
        F_xy2 = img[c22.x][c22.y];
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


void rawReader::SaveImage()
{
    std::ofstream myfile;
    myfile.open ("image.ppm");
    myfile << "P3\n";
    myfile << w << " " << h << std::endl;
    myfile << 256 << std::endl;
   
    
   for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            myfile << img_rgb_16[i][j].R/256 << " "; 
            myfile << img_rgb_16[i][j].G/256 << " "; 
            myfile << img_rgb_16[i][j].B/256 << std::endl; 
        }
    }
    
     myfile.close();
}

void rawReader::getOriginalDimensions(int &width, int &height)
{
    width = w;
    height = h;
    return;
}








