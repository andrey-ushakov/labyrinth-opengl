

#ifndef _OGL_H_
#define _OGL_H_



#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "type.h"
#include "window.h"



bool   create_context     (WINDOW &w);
void   kill_context       (WINDOW &w);
void   swap_buffer        (WINDOW *w);


void   init_font          (WINDOW &w, char *police_name);
void   kill_font          ();
void   gotoxy             (int x, int y);
void   write_2_screen     (char *text);


#endif

