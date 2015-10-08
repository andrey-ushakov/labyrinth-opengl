
#ifndef _TYPE_H_
#define _TYPE_H_



/****************************************************************************\
*                                                                            *
*    LES TYPES DE BASE                                                       *
*                                                                            *
\****************************************************************************/

typedef unsigned char		byte;
typedef unsigned short		word;
typedef unsigned int		dword;
typedef unsigned __int64	qword;



/****************************************************************************\
*                                                                            *
*    LES #define et les #pragma                                              *
*                                                                            *
\****************************************************************************/



//// WIN32 ////
#ifdef WIN32

#define WIN32_LEAN_AND_MEAN 1          // pour réduire la taille des headers win32
#define HANDLE_PRAGMA_PACK_PUSH_POP 1  // pour utliser les #pragma pack façon WIN32 dans GCC 3.0

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "winmm.lib")

#endif



/****************************************************************************\
*                                                                            *
*  LES CONSTANTES                                                            *
*                                                                            *
\****************************************************************************/


// les "qualités" (textures, ...)
const dword LOW        = 0;
const dword MED        = 1;
const dword HI         = 2;


// les erreurs
const int   ERR_OK     = 0;   // pas d'erreur
const int   ERR_MEM    = 1;   // erreur d'allocation
const int   ERR_FILE   = 2;   // erreur e/s sur un fichier 
const int   ERR_SYS    = 3;   // une erreur quelconque
const int   ERR_UNSUP  = 4;   // non supporté
const int   ERR_INIT   = 5;   // erreur à l'initialisation



#include "debug.h"    // pour rendre globale ces fonctions là


#endif

