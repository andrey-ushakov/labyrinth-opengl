
#ifndef _DEBUG_H_
#define _DEBUG_H_


#include "type.h"

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>


void debug(char *fmt, ...);
void debug(int i);
void debug(float x);
void debug();


#endif

