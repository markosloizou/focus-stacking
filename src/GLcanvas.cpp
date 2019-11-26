#include "GLcanvas.h"

#include "gui.h"
#include "imageReader.h"
#include "bitmap.h"

#define PIXEL_MULTIPLE 16

BEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
	EVT_SIZE(MyGLCanvas::OnSize)
	EVT_PAINT(MyGLCanvas::OnPaint)
	EVT_MOUSE_EVENTS(MyGLCanvas::OnMouse)
    //EVT_ENTER_WINDOW(MyGLCanvas::OnMouseEnter)
    //EVT_LEAVE_WINDOW(MyGLCanvas::OnMouseLeave)
	//EVT_SIZE(MyGLCanvas::resized)
	//EVT_KEY_DOWN(MyGLCanvas::keyPressed)
	//EVT_KEY_UP(MyGLCanvas::keyReleased)
	//EVT_PAINT(MyGLCanvas::render)
END_EVENT_TABLE()

int wxglcanvas_attrib_list[5] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

MyGLCanvas::MyGLCanvas(wxWindow *parent ,  MyFrame *f, wxWindowID id, const wxPoint& pos,
		       const wxSize& size, long style, const wxString& name, const wxPalette& palette):
               wxGLCanvas(parent, id, wxglcanvas_attrib_list, pos, size, style, name, palette)
{
    context = new wxGLContext(this);
    init = false;
    

    
    bmp.openFile("/home/markos/c++/focus_stack/sample_images/random_images/tree.NEF");
    //rawR.printData();
    bmp.generateBMP();
    //rawR.SaveImage();
    //bmp.generateGrayscale();
    
    int ww, hh;
    bmp.getDimensions(ww, hh);
    simage.create_empty_image(ww,hh);
    simage.set_image_only(bmp.get_rgb_image());
    
    glutInitDisplayMode(GLUT_RGBA);
    
    frame = f;
    
    imageGL = nullptr;
    grayscaleGL = nullptr;
    overlayGL = nullptr;
    haveImage = false;
    haveGrayscale = false;
    haveOverlay = false;
   
    run = false;
    opacity = 50;
   
    getAspectRatioImage(width, height);
}

MyGLCanvas::~MyGLCanvas()
{
    bmp.close();
}

void MyGLCanvas::Run_SVD(int N, int sv)
{
    bmp.generateSharpnessMapSVD(frame, N, sv, mirror, true);
    run = true;
    simage.set_image(bmp.get_rgb_image(), bmp.get_sharpness_map());
    Render();
    
}

void MyGLCanvas::SetOpacity(ushort opc)
{
    opacity = opc;
    Render();
}
void MyGLCanvas::Render()
{
    int w, h;

    GetClientSize(&w, &h);

    SetCurrent(*context);
    if (!init)
    {
        InitGL();
        init = true;
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    GetImage();
    
    glRasterPos2i(0,0);
    
    
    std::cout << "(w,h)" << w << ", " << h << "\t(width, height):" << width << ", " << height << std::endl;
    
    
//     std::vector< std::vector<rgb_16> > v;
//     std::vector<rgb_16> v1,v2;
//     
//     rgb_16 c;
//     c.R = pow(2,16)-1;
//     c.G = pow(2,16)-1;
//     c.B = pow(2,16)-1;
//     
//     v2.push_back(c);
//     
//     c.R = 0;
//     c.G = 0;
//     c.B = 0;
//     
//     v2.push_back(c);
//     
//     c.R = pow(2,16)-1;
//     c.G = 0;
//     c.B = 0;
//     
//     v1.push_back(c);
//     
//     c.R = 0;
//     c.G = 0;
//     c.B = pow(2,16)-1;
//     v1.push_back(c);
//     
//     v.push_back(v2);
//     v.push_back(v1);
//     
//     std::vector< std::vector<float> > s;
//     std::vector<float> s1,s2;
//     s1.push_back(1);
//     s1.push_back(1);
//     s2.push_back(1);
//     s2.push_back(1);
//     s.push_back(s2);
//     s.push_back(s1);
    //if(haveGrayscale) glDrawPixels(width,height, GL_RGB, GL_UNSIGNED_SHORT, grayscaleGL);
    
//     simage.set_image(v,s);

    getAspectRatioImage(width, height);
    centerPosition(width,height);
    glDrawPixels(width,height, GL_RGBA, GL_UNSIGNED_SHORT, imageGL);

    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if(run == true)
    {
        getOverlayImage(width, height, opacity);
        centerPosition(width,height);
        glDrawPixels(width,height, GL_RGBA, GL_UNSIGNED_SHORT, overlayGL);
    }
    
    // We've been drawing to the back buffer, flush the graphics pipeline and swap the back buffer to the front
    glFlush();
    SwapBuffers();
}

void MyGLCanvas::InitGL()
{
    int w, h;

    GetClientSize(&w, &h);
    SetCurrent(*context);
    glDrawBuffer(GL_BACK);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glViewport(0, 0, (GLint) w, (GLint) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

 // Event handler for when the canvas is exposed
void MyGLCanvas::OnPaint(wxPaintEvent& event)
{
  int w, h;

  wxPaintDC dc(this); // required for correct refreshing under MS windows
  GetClientSize(&w, &h);
  
  Render();
}


void MyGLCanvas::OnSize(wxSizeEvent& event)
{
    if(run == true)
    {
        getAspectRatioImage(width,height);
        getOverlayImage(width,height, opacity);
        GetGrayscaleImage(width,height);
    }
}


void MyGLCanvas::GetImage()
{
    int w, h;

    GetClientSize(&w, &h);
    
    if(imageGL!=nullptr)
    {
        delete[] imageGL;
        imageGL = nullptr;
    }
    std::cout << "Canvas size: " << w << " x " << h << std::endl;
    image = new (std::nothrow) ushort[w*h*3];
    imageGL = new (std::nothrow) GLushort[w*h*3];
    
    w = w - (w % PIXEL_MULTIPLE);
    h = h - (h % PIXEL_MULTIPLE);
    std::cout << "GetImage 1" << std::endl;
    simage.fill_img_buffer(w,h, image);
    std::cout << "GetImage 2" << std::endl;
    for(int i = 0; i < h; i ++)
    {
        for(int j = 0; j < w; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                ushort a = image[3*(i*w + j) + k];
                imageGL[3*(i*w + j) + k] = (GLushort)  a;
                //std::cout << a << std::endl;
            }
        }
    }
    
    delete [] image;
    
    //SaveImage();
    haveImage = true;
}

void MyGLCanvas::GetGrayscaleImage(int &width, int &height)
{
     int w, h;

    GetClientSize(&w, &h);
    
    bmp.getOriginalDimensions(width, height);
    
    float aspectRatio = (float)width/height;
    int get_width, get_height;
    
    if((float)w/h > aspectRatio)
    {
        get_height = h;
        get_width = (int)round(aspectRatio * h);
    }
    else
    {
        get_width = w;
        get_height = (int) round(w / aspectRatio);
    }
    
    get_width = get_width - (get_width % PIXEL_MULTIPLE);
    get_height = get_height - (get_height % PIXEL_MULTIPLE);
    
    width = get_width;
    height = get_height;
    
    w = width;
    h = height;
    
    std::cout << "Canvas size: " << w << " x " << h << std::endl;
    if(imageGL!=nullptr)
    {
        delete[] imageGL;
        imageGL = nullptr;
    }
    if(grayscaleGL!= nullptr)
    {
        delete[] grayscaleGL;
        grayscaleGL = nullptr;
    }
    image = new (std::nothrow) ushort[w*h*3];
    grayscaleGL = new (std::nothrow) GLushort[w*h*3];

    std::cout << "GetImage 1" << std::endl;
    bmp.fillGrayscaleBuffer(w,h, image);
    std::cout << "GetImage 2" << std::endl;
    for(int i = 0; i < h; i ++)
    {
        for(int j = 0; j < w; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                ushort a = image[3*(i*w + j) + k];
                grayscaleGL[3*(i*w + j) + k] = (GLushort)  a;
                //std::cout << a << std::endl;
            }
        }
    }
    
    delete [] image;
    //SaveImage();
    haveGrayscale  = true;
}


void MyGLCanvas::getOverlayImage(int &width, int &height,ushort overlay)
{
     int w, h;

    GetClientSize(&w, &h);
    
    simage.getOriginalDimensions(width, height);
    
    float aspectRatio = (float)width/height;
    int get_width, get_height;
    
    if((float)w/h > aspectRatio)
    {
        get_height = h;
        get_width = (int)round(aspectRatio * h);
    }
    else
    {
        get_width = w;
        get_height = (int) round(w / aspectRatio);
    }
    
    get_width = get_width - (get_width % PIXEL_MULTIPLE);
    get_height = get_height - (get_height % PIXEL_MULTIPLE);
    
    width = get_width;
    height = get_height;
    w = width;
    h = height;
    
    if(overlayGL != nullptr)
    {
        delete[] overlayGL;
        overlayGL = nullptr;
    }
    image = new (std::nothrow) ushort[w*h*4];
    overlayGL = new (std::nothrow) GLushort[w*h*4];
    
    std::vector<rgba_16> gradient;
    
    std::cout << "opacity: " << opacity << std::endl;
    std::cout << "value: " << pow(2,16)-1 * opacity / 100 << std::endl;
    ushort max = (ushort)pow(2,16) - 1;

    gradient.push_back(rgba_16(max,max,max, (max * opacity) / 100));
    gradient.push_back(rgba_16(max,max,0, (max * opacity) / 100));
    gradient.push_back(rgba_16(max,0,0, (max * opacity) / 100));
    gradient.push_back(rgba_16(max,0,max, (max * opacity) / 100));
    gradient.push_back(rgba_16(0,0,max, (max * opacity) / 100));
    gradient.push_back(rgba_16(0,max,max, (max * opacity) / 100));
    gradient.push_back(rgba_16(0,max,0, (max * opacity) / 100));
    gradient.push_back(rgba_16(0,0,0, (max * opacity) / 100));
    
    
    gradient.begin(), gradient.end();
    

    simage.fill_map_buffer(w,h, image, gradient);
    
    
    for(int i = 0; i < h; i ++)
    {
        for(int j = 0; j < w; j++)
        {
            for(int k = 0; k < 4; k++)
            {
                ushort a = image[4*(i*w + j) + k];
                overlayGL[4*(i*w + j) + k] = (GLushort)  a;
            }
        }
    }

    
    delete [] image;
    //SaveImage();
    haveOverlay = true;
}

void MyGLCanvas::DrawImage()
{
    int w,h;
    GetClientSize(&w, &h);
    //upload to GPU texture
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB, w, h, 0,  GL_RGB, GL_UNSIGNED_SHORT, imageGL);
    glBindTexture(GL_TEXTURE_2D, 0);

    //match projection to window resolution (could be in reshape callback)
    glMatrixMode(GL_PROJECTION);
    glOrtho(0, w, 0, h, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    
    //glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //clear and draw quad with texture (could be in display callback)
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, tex);
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);
    glTexCoord2i(0, 0); glVertex2i(0, 0);
    glTexCoord2i(0, 1); glVertex2i(0, h);
    glTexCoord2i(1, 1); glVertex2i(w, h);
    glTexCoord2i(1, 0); glVertex2i(w, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    //glFlush(); //don't need this with GLUT_DOUBLE and glutSwapBuffers
}

void MyGLCanvas::SaveImage()
{
    int w,h;
    GetClientSize(&w, &h);
    
    std::ofstream myfile;
    myfile.open ("image.ppm");
    myfile << "P3\n";
    myfile << w << " " << h << std::endl;
    myfile << 256 << std::endl;
   
    
    for(int i = 0; i < h; i ++)
    {
        for(int j = 0; j < w; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                myfile << imageGL[3*(i*w + j) + k]/256 << " "; 
            }
        }
        myfile << std::endl;
    }
    
     myfile.close();
}

void MyGLCanvas::getAspectRatioImage(int &width, int &height)
{
    int w, h;

    GetClientSize(&w, &h);
    
    std::cout << "Canvas size: " << w << " x " << h << std::endl;
    
    simage.getOriginalDimensions(width, height);
    
    float aspectRatio = (float)width/height;
    int get_width, get_height;
    
    if((float)w/h > aspectRatio)
    {
        get_height = h;
        get_width = (int)round(aspectRatio * h);
    }
    else
    {
        get_width = w;
        get_height = (int) round(w / aspectRatio);
    }
    
    get_width = get_width - (get_width % PIXEL_MULTIPLE);
    get_height = get_height - (get_height % PIXEL_MULTIPLE);
    
    width = get_width;
    height = get_height;
    
    
    std::cout << "Get dimensions: " << get_width << " x " << get_height << std::endl;
    
    if(imageGL != nullptr)
    {
        delete[] imageGL;
        imageGL = nullptr;
    }
    image = new (std::nothrow) ushort[get_width*get_height*3];
    imageGL = new (std::nothrow) GLushort[get_width*get_height*4];
    
    simage.fill_img_buffer(get_width,get_height, image);
    
    for(int i = 0; i < get_height; i ++)
    {
        for(int j = 0; j < get_width; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                ushort a = image[3*(i*get_width + j) + k];
                imageGL[4*(i*get_width + j) + k] = (GLushort)  a;
                //std::cout << a << std::endl;
            }
            imageGL[4*(i*get_width + j) + 3] = (GLushort) pow(2,16)-1;
        }
    }
    
    delete [] image;
    image = nullptr;
    //SaveImage();
    haveImage = true;
}

void MyGLCanvas::centerPosition(int width, int height)
{
    int w,h;
    GetClientSize(&w, &h);
    
    int w_r = w - width;
    int h_r = h - height;
    
    w_r = w_r/2;
    h_r = h_r/2;
    
    glRasterPos2i(w_r,h_r);
}



void MyGLCanvas::Run_LBP(float threshold, float radius, int region_radius)
{
    lbp img(&bmp, frame); 
    img.generate_sharpness_map(threshold, radius, (float)region_radius, true);
    simage.set_image(img.get_img_rgb_16(), img.get_sharpness_map());
    run = true;
    Render();
}




void MyGLCanvas::OnMouse(wxMouseEvent &event)
{
    if(event.Entering()) frame->setStatusBarText("Hover over sharpness map for sharpness value");
    else if(event.Leaving()) frame->setStatusBarText("Hover over sharpness map for sharpness value");
    else if(event.Moving()) GetSharpness(event.GetPosition());
}


void MyGLCanvas::GetSharpness(wxPoint pos)
{
    int w,h;
    GetClientSize(&w, &h);
    
    float sharpness;
    
    //left top coordinates
    int w_lt = w - width;
    int h_lt = h - height;
    
    w_lt = w_lt/2;
    h_lt = h_lt/2;
    
    //left bottom
    int w_lb = w_lt;
    int h_lb = h_lt  + height;
    
    //right top
    int w_rt = w_lt + width;
    int h_rt = h_lt;
    
    //right bottom
    int w_rb = w_rt;
    int h_rb = h_lb;
    
    if(pos.x >= w_lt && pos.x <= w_rt && pos.y >= h_lt && pos.y <= h_lb)
    {
        std::string s;
        if(run)
        {
            sharpness = simage.getSharpness(height - (pos.y - h_lt), pos.x - w_lt, width, height);
            s = "Sharpness Value   x = ";
            s += std::to_string(pos.x - w_lt);
            s += "  y = ";
            s += std::to_string(pos.y - h_lt);
            s += " : ";
            s = s + std::to_string(sharpness);
        }
        else
        {
            s = "Sharpness Map not yet generated";
        }

        frame->setStatusBarText(s);
    }
    else
    {
        frame->setStatusBarText("Hover over sharpness map for sharpness value");
    }
    

}




void MyGLCanvas::SetCurrentImage(std::string file)
{
    run = false;
    bmp.openFileStr(file);
    bmp.generateBMP();
    
    int ww, hh;
    bmp.getDimensions(ww, hh);
    simage.create_empty_image(ww,hh);
    simage.set_image_only(bmp.get_rgb_image());
    Render();
}


void MyGLCanvas::process_single_LBP(float threshold, float radius, int region_radius, std::vector<std::string> *files)
{
    std::string file;
    while(true)
    {
        queue_mutex.lock();
        if(files->size() > 0)
        {
            file = files->back();
            files->pop_back();
            queue_mutex.unlock();
            std::cout << "Processing file: " << file << std::endl;
        }
        else
        {
            std::cout << "exiting" << std::endl;
            queue_mutex.unlock();
            break;
        }
        read_file_mutex.lock();
        bitmap bm(file);
        lbp img(&bm, frame);
        read_file_mutex.unlock();
        img.generate_sharpness_map(threshold, radius, (float)region_radius);
        
        update_image_mutex.lock();
        simage.update_image(img.get_img_rgb_16_ptr(), img.get_sharpness_map_ptr());
        update_image_mutex.unlock();
        run = true;
        render_mutex.lock();
        finished_jobs++;
        render_mutex.unlock();
        
    }
//     while(true)
//     {
//         running_threads_mutex.lock();
//         if(running_threads < max_threads)
//         {
//             running_threads++;
//             running_threads_mutex.unlock();
//             break;
//         }
//         running_threads_mutex.unlock();
//         std::unique_lock<std::mutex> lck(running_threads_mutex);
//         cv.wait(lck);
//     }
//     lbp *img;
//     read_file_mutex.lock();
//     bitmap bm(file);
//     img = new lbp(&bm, frame); 
//     read_file_mutex.unlock();
//     img->generate_sharpness_map(threshold, radius, (float)region_radius);
//     update_image_mutex.lock();
//     simage.update_image(img->get_img_rgb_16_ptr(), img->get_sharpness_map_ptr());
//     update_image_mutex.unlock();
//     run = true;
//     running_threads--;
//     cv.notify_all();
//     render_mutex.lock();
//     Render();
//     render_mutex.unlock();
//     delete img;
}



void MyGLCanvas::Process_LBP(float threshold, float radius, int region_radius, std::vector<std::string> files)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    running_threads = 0;
    finished_jobs = 0;
    
    std::vector<std::thread> threads;
    
    int jobs = files.size();
    
    for(int i = 0; i < max_threads; i++)
    {
        threads.push_back(std::thread(&MyGLCanvas::process_single_LBP,this, threshold, radius, region_radius, &files) );
    }
    
    int old_percentage = 0;
    
    while(true)
    {
        int percentage = (finished_jobs*100)/jobs;
        frame->setGauge(percentage);
        if(old_percentage != percentage)
        {
            Render();
            old_percentage = percentage;
        }
        if(jobs == finished_jobs) 
        {
            break;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
    }    
    
    for(int i = 0; i < max_threads; i++)
    {
        threads[i].join();
    }
    
    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    long seconds = std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    
    std::cout << jobs << " photos merged" << std::endl;
    std::cout << "Elapsed time: " << seconds / 3600 << ":" << (seconds%3600) / 60 << ":" << seconds % 60 << std::endl;
    
    
}



void MyGLCanvas::Process_SVD(int N, int sv,std::vector<std::string> files)
{

    
}

void MyGLCanvas::set_min_sharpness(float s)
{
    simage.set_min_sharpness(s);
    Render();
}


void MyGLCanvas::save(std::string s)
{
    simage.save_file(s);
}
