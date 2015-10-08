
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "type.h"

#include <windows.h>


class WINDOW
{
  private:

    bool   fen_ok;


  public:

    int    nbbits;          // 16,24,32 bits
    dword  freq;            // 60-160 Hz
    bool   fullscreen;      // true = plein écran
    dword  Xres;            // viewport X
    dword  Yres;            // viewport Y
    dword  Xpos;            // position X
    dword  Ypos;            // position Y

    bool   visible;         // minimized = false / maximized = true
    bool   focus;           // true si la fenêtre à le focus

    HWND   handle;
    HDC    context;
    HGLRC  context_3D;


    WINDOW();
    ~WINDOW();
    
    bool create(int width, int height, int nbbits, int freq, bool fullscreen);
    bool close();
	void set_title(char *s);

};


#endif

