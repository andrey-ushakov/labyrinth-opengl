

#include "debug.h"


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




/**************************************************************************\
*                                                                          *
* Affiche une messagebox avec la chaine passée en paramètre                *
*                                                                          *
\**************************************************************************/
void debug(char *fmt, ...)
{
  char     text[512];
  va_list  ap;

  va_start(ap, fmt);
  vsprintf(text, fmt, ap);
  va_end(ap);

#ifdef WIN32
  MessageBox(NULL,text,"< DEBUG >",MB_TASKMODAL);
#endif

#ifdef UNIX
  fprintf(stderr,"%s\n",text);
#endif

}



/**************************************************************************\
*                                                                          *
* Affiche une messagebox avec la valeur                                    *
*                                                                          *
\**************************************************************************/
void debug(int i)
{
  char text[100];

  sprintf(text,"%i",i);

#ifdef WIN32
  MessageBox(NULL,text,"< DEBUG >",MB_TASKMODAL);
#endif

#ifdef UNIX
  fprintf(stderr,"%s\n",text);
#endif

}



/**************************************************************************\
*                                                                          *
* Affiche une messagebox avec la valeur                                    *
*                                                                          *
\**************************************************************************/
void debug(float x)
{
  char text[100];

  sprintf(text,"%f",x);

#ifdef WIN32
  MessageBox(NULL,text,"< DEBUG >",MB_TASKMODAL);
#endif

#ifdef UNIX
  fprintf(stderr,"%s\n",text);
#endif

}



/**************************************************************************\
*                                                                          *
* Affiche une messagebox de debugage (sert de point d'arrêt)               *
*                                                                          *
\**************************************************************************/
void debug()
{

#ifdef WIN32
  MessageBox(NULL,"* breakpoint *","< DEBUG >",MB_TASKMODAL);
#endif

#ifdef UNIX
  fprintf(stderr,"* breakpoint *\n");
#endif

}



