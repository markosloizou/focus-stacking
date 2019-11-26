#ifndef main_h
#define main_h

#include <wx/wx.h>
#include "gui.h"
#include "GLcanvas.h"
#include "imageReader.h"

class MyApp: public wxApp
{
    public:
        bool OnInit(); // automatically called when the application starts
};
#endif
