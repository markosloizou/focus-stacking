#include "bitmap.h"

#include "gui.h"

bitmap::bitmap(std::string s)
{
    char *cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());
    openFile(cstr);
    delete[] cstr;
    generateBMP();
    N_computed = 0;
}

bitmap::bitmap()
{
    N_computed = 0;
}

void bitmap::openFileStr(std::string s)
{
    char *cstr = new char[s.length() + 1];
    strcpy(cstr, s.c_str());
    openFile(cstr);
    delete[] cstr;
    generateBMP();
    N_computed = 0;
}

void bitmap::generateSharpnessMapSVD(MyFrame *f, int N, int k, padding p, bool updateGaugeBar)
{
    updateGauge = updateGaugeBar;
    std::vector<rgb_16> colors;
    
    ushort full = pow(2,16)-1;
    colors.push_back(rgb_16(full,full,full));
    colors.push_back(rgb_16(full,full,0));
    colors.push_back(rgb_16(full,0,0));
    colors.push_back(rgb_16(full,0,full));
    colors.push_back(rgb_16(0,0,full));
    colors.push_back(rgb_16(0,full,full));
    colors.push_back(rgb_16(0,full,0));
    colors.push_back(rgb_16(0,0,0));
    
    
    if(N_computed != N)
    {
        generateGrayscale(N,k,p);
    }
    
    if(sharpness_map.size() != 0)
    {
        std::cout << "erasing sharpness map" << std::endl;
        sharpness_map.erase(sharpness_map.begin(), sharpness_map.end());
    }
    if(colored_sharpness_map.size() != 0)
    {
        colored_sharpness_map.erase(colored_sharpness_map.begin(), colored_sharpness_map.end());
    }
    
    if(N_computed != N)
    {
        if(eigen_map.size()!= 0)   eigen_map.erase(eigen_map.begin(), eigen_map.end());
    }
    
    for(int i = 0; i < h; i++)
    {
        if(updateGauge) f-> setGauge(100*i/h);
        std::vector<Eigen::VectorXf> sv_vec;
        std::vector<float> sharpness;
        std::vector<rgb_16> colored;
       
        
        for(int j = 0; j < w; j++)
        {
            float f;
            
            if(N_computed != N)
            {
                Eigen::VectorXf sv = SVs(i,j,N);
                f = svRatio(sv, k);
                sv_vec.push_back(sv);
            }
            else f = precomputedSVratio(i,j,k);
            
            sharpness.push_back(f);
            colored.push_back(map_float_to_rgb16(f, colors));
            
             //std::cout << "ratio: " << f << "\tcolor : " <<  map_float_to_rgb16(f,colors) << std::endl;
        }
        if(N_computed != N) eigen_map.push_back(sv_vec);
        colored_sharpness_map.push_back(colored);
        sharpness_map.push_back(sharpness);
    }
    
    float mean = 0;
    float stdd = 0;
    float max = -100;
    float min = 100;
    
    for(int i = 0; i < sharpness_map.size(); i++ )
    {
        for(int j =0; j< sharpness_map[0].size();j++)
        {
            mean += sharpness_map[i][j];
            
            if(sharpness_map[i][j] > max) max = sharpness_map[i][j];
            if(sharpness_map[i][j] < min) min = sharpness_map[i][j];
        }
        if(i%500 == 0)
        {
            std::cout << "(i,j):" << i <<  ", " << 0 << "\t f = " << sharpness_map[i][0] << "\t color" << colored_sharpness_map[i][0] << std::endl;
        }
    }
    mean = mean/(sharpness_map.size()*sharpness_map[0].size());
    
     for(int i = 0; i < sharpness_map.size(); i++ )
    {
        for(int j =0; j< sharpness_map[0].size();j++)
        {
            stdd += pow(sharpness_map[i][j] - mean, 2);
        }
    }
    
    stdd = stdd/(sharpness_map.size()*sharpness_map[0].size() - 1);
    
    std::cout << "Sharpness map mean: " << mean << "\nVariance: " << stdd << std::endl;
    std::cout << "Sharpness map max: " << max << "\t min: " << min << std::endl;
    
    N_computed = N;
}

float bitmap::precomputedSVratio(int x, int y, int k)
{
    Eigen::VectorXf sv = eigen_map[x][y];
    //std::cout<< "prec:" << sv << "\n";
    float Tsum = 0;
    float ksum = 0;
    float ratio = 0;
    
    
    for(int i = 0; i < sv.size(); i++)
    {
        if(!isnan(sv(i)))
        {   
            Tsum += sv(i);
        }
    }

    for(int i = 0; (i < k) && (i < sv.size()) ; i++)
    {
        if(!isnan(sv(i)))
        {   
            ksum += sv(i);
        }
    }
    
    if(Tsum != 0)
    {
        ratio = ksum/Tsum;
    }
    else
    {
        std::cout << "Sum of all singular values  = 0" << std::endl;
        std::cout << sv << std::endl;
        return 0.0;
    }
    
    if(isnan(ksum))
    {
        std::cout << "KSum of all singular values  = 0" << std::endl;
        std::cout << "svs: \n" << sv << std::endl;
        return 0.0;
    }
    
    if(isnan(Tsum))
    {
        std::cout << "TSum of all singular values  = 0" << std::endl;
        std::cout << "svs: \n" << sv << std::endl;
        return 0.0;
    }
    return ratio;
}

Eigen::VectorXf bitmap::SVs(int x, int y, int N)
{
    Eigen::MatrixXf  M(N+1,N+1);
    
    x += N/2;
    y += N/2;

    
    for(int i = 0; i < N+1; i++)
    {
        for(int j = 0; j < N+1; j++)
        {
            int a = x + i - (int)ceil(N/2);
            int b = y + j - (int)ceil(N/2);
            M(i,j) = (float)grayscale[a][b]/pow(2,16);
        }
    }
    //if(x<2) std::cout << "(x,y)" << x << ", "<< x <<  std::endl;
    //if(x<2) std::cout << M << std::endl;
    
    

    Eigen::VectorXf sv(N);
    
    
    if(N < 16)
    {
        Eigen::JacobiSVD<Eigen::MatrixXf> svd(M);
        sv = svd.singularValues();
    }
    else
    {
        try{
        Eigen::BDCSVD<Eigen::MatrixXf> svd(M);
        sv = svd.singularValues();
        } catch(const std::exception &e) {
            std::cout <<  "BDC exception: " << e.what() << "\n" << M << std::endl;
        }
    }
    
    nsv = N;
    return sv;
}

float bitmap::svRatio(Eigen::VectorXf sv, int k)
{
    float Tsum = 0;
    float ksum = 0;
    float ratio = 0;
    
    for(int i = 0; i < nsv; i++)
    {
        if(!isnan(sv(i)))
        {   
            Tsum += sv(i);
        }
    }

    for(int i = 0; i < k; i++)
    {
        if(!isnan(sv(i)))
        {   
            ksum += sv(i);
        }
    }
    
    if(Tsum != 0)
    {
        ratio = ksum/Tsum;
    }
    else
    {
        std::cout << "Sum of all singular values  = 0" << std::endl;
        std::cout << sv << std::endl;
        return 0.0;
    }
    
    if(isnan(ksum))
    {
        std::cout << "KSum of all singular values  = 0" << std::endl;
        std::cout << "svs: \n" << sv << std::endl;
        return 0.0;
    }
    
    if(isnan(Tsum))
    {
        std::cout << "TSum of all singular values  = 0" << std::endl;
        std::cout << "svs: \n" << sv << std::endl;
        return 0.0;
    }
    return ratio;
}

void bitmap::generateGrayscale(int N, int k, padding p)
{
    N += 1;
    if(grayscale.size() != 0)
    {
        grayscale.erase(grayscale.begin(), grayscale.end());
    }
    
    for(int i = 0; i < h; i++)
    {
        std::vector<ushort> gscale;
        //add padding  at the top of the image
        if(p == gray)
        {
            for(int k = 0; k < ceil(N/2); k++) 
            {
                gscale.push_back( (ushort) pow(2,16) / 2 );
            }
        }
        else if(p == mirror)
        {
            for(int k = ceil(N/2); k > 0; k--)
            {
                float X = 0.2989 * (float) img_rgb_16[i][h - k - 1].R; 
                X += 0.5870 * (float) img_rgb_16[i][h - k - 1].G;
                X += 0.1149 * (float) img_rgb_16[i][h - k - 1].B;
                gscale.push_back((ushort)X);

            }
        }
        
        //when we enter this loop we will start at the top of the image
        for(int j = 0; j < w; j++)
        {
            
            float X = 0.2981 * (float) img_rgb_16[i][j].R; 
            X += 0.5870 * (float) img_rgb_16[i][j].G;
            X += 0.1149 * (float) img_rgb_16[i][j].B;
            if(X > (pow(2,16)-1)) X = pow(2,16)-1;
            
            gscale.push_back((ushort) X );
            
        }
        
        //add padding at the bottom
        if(p == gray)
        {
            for(int k = 0; k < ceil(N/2); k++) 
            {
                gscale.push_back( (ushort) pow(2,16) / 2 );
            }
        }
        else if(p == mirror)
        {
            for(int k = 0; k < ceil(N/2); k++)
            {
                float X = 0.2989 * (float) img_rgb_16[i][k+1].R; 
                X += 0.5870 * (float) img_rgb_16[i][k+1].G;
                X += 0.1149 * (float) img_rgb_16[i][k+1].B;
                gscale.push_back((ushort) X);
            }
        }
        
        grayscale.push_back(gscale);
    }
    
    //add first N/2 rows 
    for(int k = 0; k < ceil(N/2); k++)
    {
        grayscale.insert(grayscale.begin(), grayscale[k+1]);
    }
    
    //add last N/2 rows
    for(int k = 0; k < ceil(N/2); k++)
    {
        grayscale.insert(grayscale.end(), grayscale[h - k - 1]);
    }
    
    std::cout << "grayscale dimensions: " << grayscale.size() << " x " << grayscale[0].size() << std::endl;
}


void bitmap::generateGrayscale(int padding_length,  padding p)
{
    if(grayscale.size() != 0)
    {
        std::cout << "erasing grayscale\n" << "size: " << grayscale.size() << std::endl;
        grayscale.clear();
    }
    
    for(int i = 0; i < h; i++)
    {
        std::vector<ushort> gscale;
        //add padding  at the top of the image
        if(p == gray)
        {
            for(int k = 0; k < padding_length; k++) 
            {
                gscale.push_back( (ushort) pow(2,16) / 2 );
            }
        }
        else if(p == mirror)
        {
            for(int k = padding_length; k > 0; k--)
            {
                float X = 0.2989 * (float) img_rgb_16[i][h - k - 1].R; 
                X += 0.5870 * (float) img_rgb_16[i][h - k - 1].G;
                X += 0.1149 * (float) img_rgb_16[i][h - k - 1].B;
                gscale.push_back((ushort)X);
                
            }
        }
        
        //when we enter this loop we will start at the top of the image
        for(int j = 0; j < w; j++)
        {
            
            float X = 0.2981 * (float) img_rgb_16[i][j].R; 
            X += 0.5870 * (float) img_rgb_16[i][j].G;
            X += 0.1149 * (float) img_rgb_16[i][j].B;
            if(X > (pow(2,16)-1)) X = pow(2,16)-1;
            
            gscale.push_back((ushort) X );
            
        }
        
        //add padding at the bottom
        if(p == gray)
        {
            for(int k = 0; k < padding_length; k++) 
            {
                gscale.push_back( (ushort) pow(2,16) / 2 );
            }
        }
        else if(p == mirror)
        {
            for(int k = 0; k < padding_length; k++)
            {
                float X = 0.2989 * (float) img_rgb_16[i][k+1].R; 
                X += 0.5870 * (float) img_rgb_16[i][k+1].G;
                X += 0.1149 * (float) img_rgb_16[i][k+1].B;
                gscale.push_back((ushort) X);
            }
        }
        
        grayscale.push_back(gscale);
    }
    
    //add first N/2 rows 
    for(int k = 0; k < padding_length; k++)
    {
        grayscale.insert(grayscale.begin(), grayscale[k+1]);
    }
    
    //add last N/2 rows
    for(int k = 0; k < padding_length; k++)
    {
        grayscale.insert(grayscale.end(), grayscale[h - k - 1]);
    }
    
    std::cout << "grayscale dimensions: " << grayscale.size() << " x " << grayscale[0].size() << std::endl;
}


void bitmap::fillOverlayBuffer(int width, int height, ushort alpha ,ushort*buffer)
{
    std::vector<rgb_16> colors;
    ushort full = pow(2,16)-1;
    colors.push_back(rgb_16(full,full,full));
    colors.push_back(rgb_16(full,full,0));
    colors.push_back(rgb_16(full,0,0));
    colors.push_back(rgb_16(full,0,full));
    colors.push_back(rgb_16(0,0,full));
    colors.push_back(rgb_16(0,full,full));
    colors.push_back(rgb_16(0,full,0));
    colors.push_back(rgb_16(0,0,0));
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
            

            current_pixel = bilinearInterpolation(c11,c12,c21,c22,x,y, colored_sharpness_map);
            
            buffer[4*(i*width + j) + 0] = current_pixel.R;
            buffer[4*(i*width + j) + 1] = current_pixel.G;
            buffer[4*(i*width + j) + 2] = current_pixel.B;
            buffer[4*(i*width + j) + 3] = alpha;
        }
    }
    
}

float bitmap::bilinearInterpolationV(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<float>> &map)
{
    float F_xy1, F_xy2;
    float F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes
    
    //TODO have removed denomenators, check perforkamce
    if(!(c21.x == x) && (c11.x == x))
    {
        F_xy1 = (c21.x - x) * map[c11.x][c11.y];
        F_xy1 += (x - c11.x) * map[c21.x][c21.y];
        
        F_xy2 = (c22.x - x) * map[c12.x][c12.y];
        F_xy2 += (x - c12.x) * map[c22.x][c22.y];
    }
    else
    {
        F_xy1 = map[c11.x][c11.y];
        
        F_xy2 = map[c11.x][c22.y];
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


rgb_16 bitmap::map_float_to_rgb16(float f, std::vector<rgb_16> &colors)
{
    int n_colors = colors.size() - 1;
    
    float x = f*n_colors;
    
    if(isnan(x))
    {
        std::cout << "X is NaN. f = " << f << "   num of cols: " << n_colors << std::endl;
        x = n_colors/2;
    }
    
    int x_1 = (int)floor(x);
    int x_2 = (int)ceil(x);
    
    return linear_interpolation(x, x_1, x_2, colors[x_1], colors[x_2]);
    
}

rgb_16 bitmap::linear_interpolation(float x, int x1, int x2, rgb_16 c1, rgb_16 c2)
{
    rgb_16 color,color1, color2;
    
    if(x2 != x1)
    {
        color1 = (x - x1)/(x2 - x1)*c1; 
        color2 = (x2 -  x)/(x2 - x1)*c2;
        color = color1 + color2;

    }
    else
    {
        color = c1;
    }
        
    return color;
}



void bitmap::fillGrayscaleBuffer(int width, int height, ushort*buffer)
{
    float x,y; 
   
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            x = (float)i/height * h;
            y = (float)j/width * w;
            

            ushort current_pixel;

            coordinate c11,c12,c21,c22;
            
            c11.x = (int)floor(x);
            c11.y = (int)floor(y);
            
            c12.x = (int)floor(x);
            c12.y = (int)ceil(y);
            
            c21.x = (int)ceil(x);
            c21.y = (int)floor(y);
            
            c22.x = (int)ceil(x);
            c22.y = (int)ceil(y);
            

            current_pixel = bilinearInterpolationV(c11,c12,c21,c22,x,y, grayscale);
            
            buffer[3*(i*width + j) + 0] = (ushort)current_pixel;
            buffer[3*(i*width + j) + 1] = (ushort)current_pixel;
            buffer[3*(i*width + j) + 2] = (ushort)current_pixel;
            
        }
    }
    
}

ushort bitmap::bilinearInterpolationV(coordinate c11, coordinate c12, coordinate c21, coordinate c22, float x, float y, std::vector<std::vector<ushort>> &map)
{
    ushort F_xy1, F_xy2;
    ushort F_xy;
    
    //Linear interpolation in the x direction for y1 and y2
    //one for each channes

    if(!(c21.x == x) && (c11.x == x))
    {
        F_xy1 = (c21.x - x) * map[c11.x][c11.y];
        F_xy1 += (x - c11.x) * map[c21.x][c21.y];
        
        F_xy2 = (c22.x - x) * map[c12.x][c12.y];
        F_xy2 += (x - c12.x) * map[c22.x][c22.y];
    }
    else
    {
        F_xy1 = map[c11.x][c11.y];
        
        F_xy2 = map[c11.x][c22.y];
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


std::vector< std::vector<ushort> > bitmap::get_grayscale()
{
    return grayscale;
}

std::vector< std::vector<rgb_16> > bitmap::get_rgb_image()
{
    return img_rgb_16;
}

void bitmap::getDimensions(int &width, int &height)
{
    width = w;
    height = h;
}


std::vector< std::vector<float> > bitmap::get_sharpness_map()
{
    return sharpness_map;
}











