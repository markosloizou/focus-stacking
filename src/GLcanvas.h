#ifndef GLcanvas_h
#define GLcanvas_h

#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/cshelp.h>
#include <wx/tooltip.h>
#include <GL/glut.h>
#include <GL/gl.h>

#include <new>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <chrono>

#include <mutex>
#include <thread>
#include <condition_variable>

#include "bitmap.h"
#include "sharp_image.h"
#include "lbp.h"

class MyFrame;

class MyGLCanvas: public wxGLCanvas
{

	public:
    	//comstructor
    	MyGLCanvas(wxWindow *parent ,  MyFrame *f, wxWindowID id = wxID_ANY,
            const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0,
            const wxString& name = "MyGLCanvas", const wxPalette &palette=wxNullPalette);


        ~MyGLCanvas();
        void Render(); // function to draw canvas contents
        void Run_SVD(int N, int sv);
        void Run_LBP(float threshold, float radius, int region_radius);
        void SetOpacity(ushort opc);
        void SetCurrentImage(std::string file);
        void Process_LBP(float threshold, float radius, int region_radius, std::vector<std::string> files);
        void Process_SVD(int N, int sv,std::vector<std::string> files);
        void set_min_sharpness(float s);
        void save(std::string s);
    private:
        void OnMouse(wxMouseEvent &event);
        //void OnMouseEnter(wxMouseEvent &event);
        //void OnMouseLeave(wxMouseEvent &event);
        void OnSize(wxSizeEvent& event);
        void OnPaint(wxPaintEvent& event);
        void InitGL();
        
        void GetImage();
        void DrawImage();
        void SaveImage();
        
        void getAspectRatioImage(int &width, int &height);
        void centerPosition(int width, int height);
        void getOverlayImage(int &width, int &height,ushort alpha);
        void GetGrayscaleImage(int &width, int &height);
        
        void GetSharpness(wxPoint pos);

        MyFrame *frame;
        
        
        bool init;
        bool run;
        bool haveGrayscale;
        bool haveImage;
        bool haveOverlay;
        
        ushort opacity;
        ushort *image;
        
        int width, height;
        
        GLushort *grayscaleGL;
        GLushort *imageGL;
        GLushort *overlayGL;
        wxGLContext *context;              // OpenGL rendering context
        
        bitmap bmp;
        sharp_image simage;
        
        //Multithreading functions
        void process_single_LBP(float threshold, float radius, int region_radius, std::vector<std::string> *file);
        const int max_threads = 7;
        int running_threads;
        int finished_jobs;
        std::mutex queue_mutex;
        std::mutex running_threads_mutex;
        std::mutex render_mutex;
        std::mutex read_file_mutex;
        std::mutex update_image_mutex;
        std::condition_variable cv;

    DECLARE_EVENT_TABLE()
};
#endif
