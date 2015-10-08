
#include <math.h>
#include <string.h>
#include "type.h"
#include "window.h"
#include "ogl.h"




/****************************************************************************\
*                                                                            *
*                            Variables EXTERNES                              *
*                                                                            *
\****************************************************************************/


/****************************************************************************\
*                                                                            *
*                            Variables GLOBALES                              *
*                                                                            *
\****************************************************************************/



/****************************************************************************\
*                                                                            *
*                             Variables LOCALES                              *
*                                                                            *
\****************************************************************************/
static GLuint FontDiplayList;




/**************************************************************************\
*                                                                          *
*  Initialise OpenGL                                                       *
*                                                                          *
\**************************************************************************/
bool create_context(WINDOW &w)
{
 
  int PixelFormat;              // Holds The Results After Searching For A Match

  PIXELFORMATDESCRIPTOR pfd =   // pfd Tells Windows How We Want Things To Be
    {
          sizeof(PIXELFORMATDESCRIPTOR),
          1,                    // Version Number
          PFD_DRAW_TO_WINDOW |  // Format Must Support Window
          PFD_SUPPORT_OPENGL |  // Format Must Support OpenGL
          PFD_DOUBLEBUFFER,     // Must Support Double Buffering
          PFD_TYPE_RGBA,        // Request An RGBA Format
          w.nbbits,             // Select Our Color Depth
          0, 0, 0, 0, 0, 0,     // Color Bits Ignored
          0,                    // No Alpha Buffer
          0,                    // Shift Bit Ignored
          0,                    // No Accumulation Buffer
          0, 0, 0, 0,           // Accumulation Bits Ignored
          24,					// Z-Buffer (Depth Buffer)
          0,                    // Stencil Buffer
          0,                    // No Auxiliary Buffer
          PFD_MAIN_PLANE,       // Main Drawing Layer
          0,                    // Reserved
          0, 0, 0               // Layer Masks Ignored
    };

  PixelFormat = ChoosePixelFormat(w.context,&pfd);
  if (!PixelFormat)                                  // Did Windows Find A Matching Pixel Format ?
    {
      return false;
    }

  if (!SetPixelFormat(w.context,PixelFormat,&pfd))   // Are We Able To Set The Pixel Format ?
    {
      return false;
    }

  w.context_3D = wglCreateContext(w.context);
  if (!w.context_3D)                                   // Are We Able To Get A Rendering Context ?
    {
      return false;
    }

 
  if (!wglMakeCurrent(w.context,w.context_3D))         // Try To Activate The Rendering Context
    {
      return false;
    }



	glViewport(0, 0, w.Xres, w.Yres);	// cadrage (la zone active est la fenêtre en entier)


  return true;
}




/**************************************************************************\
*                                                                          *
*  termine OpenGL                                                          *
*                                                                          *
\**************************************************************************/
void kill_context(WINDOW &w)
{

  if (w.context_3D)
    {
      wglMakeCurrent(NULL,NULL);
      wglDeleteContext(w.context_3D);
      w.context_3D = NULL;
    }


}



/**************************************************************************\
*                                                                          *
*  Affiche la scène : le buffer couleur OpenGL est copié à l'écran         *
*                                                                          *
\**************************************************************************/
void swap_buffer(WINDOW *w)
{
	SwapBuffers(w->context);
}


/**************************************************************************\
*                                                                          *
*  Construit la liste d'affichage des caractères.                          *
*                                                                          *
\**************************************************************************/
void init_font(WINDOW &w, char *police)
{

  FontDiplayList = glGenLists(224);


  HFONT font;
  HFONT font_bis;

  font = CreateFont( w.Yres/48,
                     0,
                     0,
                     0,
                     FW_NORMAL,
                     FALSE,
                     FALSE,
                     FALSE,
                     ANSI_CHARSET,
                     OUT_TT_PRECIS,
                     CLIP_DEFAULT_PRECIS,
                     DEFAULT_QUALITY, //ANTIALIASED_QUALITY
                     FF_DONTCARE | DEFAULT_PITCH,
                     police);

  if (!font)
  {
      return;
  }

  font_bis = (HFONT)SelectObject(w.context, font);       // renvoie l'objet remplacé
  wglUseFontBitmaps(w.context, 32, 224, FontDiplayList);
  SelectObject(w.context, font_bis);                     // on remet l'objet remplacé à sa place
  DeleteObject(font);

}




/**************************************************************************\
*                                                                          *
*  détruit la liste d'affichage de la police                               *
*                                                                          *
\**************************************************************************/
void kill_font()
{
	glDeleteLists(FontDiplayList, 224);
}




/**************************************************************************\
*                                                                          *
* positionne le pointeur d'affichage du texte, on doit de préférence être  *
* en mode ortho avec une caméra simple. Z est implicitement mis à 0.       *
*                                                                          *
\**************************************************************************/
void gotoxy(int x, int y)
{
	glRasterPos2i(x,y);
}




/**************************************************************************\
*                                                                          *
*  écrit le texte à l'écran. Même remarque que gotoxy().                   *
*                                                                          *
\**************************************************************************/
void write_2_screen(char *text)
{
	glDisable(GL_TEXTURE_2D);
	glPushAttrib(GL_LIST_BIT);
	glListBase(FontDiplayList - 32);
	glRasterPos2i(0,0);
	glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}








