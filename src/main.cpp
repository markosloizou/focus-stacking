#include "main.h"


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame(NULL, "Phocus Staking", wxDefaultPosition,  wxSize(900, 800) );
    frame->Show(true);
    return(true); // enter the GUI event loop
}
