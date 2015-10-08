


#ifndef _MY_INPUT_H_
#define _MY_INPUT_H_

#include "type.h"

#include <windows.h>
#include "window.h"


class MY_INPUT
{

  private:

    bool   *tmp_pulse;
    bool   *tmp_keys;
    bool   LBmouse_tmp;
    bool   RBmouse_tmp;
    bool   MBmouse_tmp;
    WINDOW *window;
 
  public:

	MY_INPUT(WINDOW *win);
	~MY_INPUT();

    void  set_key_down(dword key);
    void  set_key_up(dword key);
    void  set_mouse_left_down();
    void  set_mouse_left_up();
    void  set_mouse_right_down();
    void  set_mouse_right_up();
    void  set_mouse_middle_down();
    void  set_mouse_middle_up();
    void  refresh();  // fixe les variables claviers, souris, joystick, donc pas de pb avec les événements.
    void  get_mouse_pos();  
    void  get_mouse_movement();
    void  set_mouse_pos(dword x, dword y);


    bool  *pulse;    // table indiquant l'état du "front montant" des touches (ne varie pas pendant la mainloop())
    bool  *keys;     // une table indiquant l'état des touches (ne varie pas pendant la mainloop())
    bool  LBmouse;   // état du bouton souris
    bool  RBmouse;   // état du bouton souris
    bool  MBmouse;   // état du bouton souris
    dword Xmouse;
    dword Ymouse;
    short Xrelmouse;  // déplacement relatif X de la souris depuis le dernier appel
    short Yrelmouse;  // déplacement relatif Y de la souris depuis le dernier appel

    // molette mouse 
    //bool  Bstick[6];

#ifdef WIN32

    #define KEY_CODE_0        0x30
    #define KEY_CODE_1        0x31
    #define KEY_CODE_2        0x32
    #define KEY_CODE_3        0x33
    #define KEY_CODE_4        0x34
    #define KEY_CODE_5        0x35
    #define KEY_CODE_6        0x36
    #define KEY_CODE_7        0x37
    #define KEY_CODE_8        0x38
    #define KEY_CODE_9        0x39
    #define KEY_CODE_SPACE    VK_SPACE
    #define KEY_CODE_PGUP     VK_PRIOR
    #define KEY_CODE_PGDN     VK_NEXT
    #define KEY_CODE_END      VK_END
    #define KEY_CODE_HOME     VK_HOME
    #define KEY_CODE_LEFT     VK_LEFT
    #define KEY_CODE_UP       VK_UP
    #define KEY_CODE_RIGHT    VK_RIGHT
    #define KEY_CODE_DOWN     VK_DOWN
    #define KEY_CODE_SELECT   VK_SELECT
    #define KEY_CODE_PRINT    VK_PRINT
    #define KEY_CODE_EXECUTE  VK_EXECUTE
    #define KEY_CODE_SNAPSHOT VK_SNAPSHOT
    #define KEY_CODE_INSERT   VK_INSERT
    #define KEY_CODE_DELETE   VK_DELETE
    #define KEY_CODE_HELP     VK_HELP
    #define KEY_CODE_A        0x41
    #define KEY_CODE_B        0x42
    #define KEY_CODE_C        0x43
    #define KEY_CODE_D        0x44
    #define KEY_CODE_E        0x45
    #define KEY_CODE_F        0x46
    #define KEY_CODE_G        0x47
    #define KEY_CODE_H        0x48
    #define KEY_CODE_I        0x49
    #define KEY_CODE_J        0x4A
    #define KEY_CODE_K        0x4B
    #define KEY_CODE_L        0x4C
    #define KEY_CODE_M        0x4D
    #define KEY_CODE_N        0x4E
    #define KEY_CODE_O        0x4F
    #define KEY_CODE_P        0x50
    #define KEY_CODE_Q        0x51
    #define KEY_CODE_R        0x52
    #define KEY_CODE_S        0x53
    #define KEY_CODE_T        0x54
    #define KEY_CODE_U        0x55
    #define KEY_CODE_V        0x56
    #define KEY_CODE_W        0x57
    #define KEY_CODE_X        0x58
    #define KEY_CODE_Y        0x59
    #define KEY_CODE_Z        0x5A
    #define KEY_CODE_LWIN     VK_LWIN
    #define KEY_CODE_RWIN     VK_RWIN
    #define KEY_CODE_APPS     VK_APPS
    #define KEY_CODE_NUMPAD0  VK_NUMPAD0
    #define KEY_CODE_NUMPAD1  VK_NUMPAD1
    #define KEY_CODE_NUMPAD2  VK_NUMPAD2
    #define KEY_CODE_NUMPAD3  VK_NUMPAD3
    #define KEY_CODE_NUMPAD4  VK_NUMPAD4
    #define KEY_CODE_NUMPAD5  VK_NUMPAD5
    #define KEY_CODE_NUMPAD6  VK_NUMPAD6
    #define KEY_CODE_NUMPAD7  VK_NUMPAD7
    #define KEY_CODE_NUMPAD8  VK_NUMPAD8
    #define KEY_CODE_NUMPAD9  VK_NUMPAD9
    #define KEY_CODE_MULTIPLY VK_MULTIPLY
    #define KEY_CODE_ADD      VK_ADD
    #define KEY_CODE_SLASH    VK_SEPARATOR
    #define KEY_CODE_MINUS    VK_SUBTRACT
    #define KEY_CODE_DECIMAL  VK_DECIMAL
    #define KEY_CODE_DIVIDE   VK_DIVIDE
    #define KEY_CODE_F1       VK_F1
    #define KEY_CODE_F2       VK_F2
    #define KEY_CODE_F3       VK_F3
    #define KEY_CODE_F4       VK_F4
    #define KEY_CODE_F5       VK_F5
    #define KEY_CODE_F6       VK_F6
    #define KEY_CODE_F7       VK_F7
    #define KEY_CODE_F8       VK_F8
    #define KEY_CODE_F9       VK_F9
    #define KEY_CODE_F10      VK_F10
    #define KEY_CODE_F11      VK_F11
    #define KEY_CODE_F12      VK_F12
    #define KEY_CODE_NUMLOCK  VK_NUMLOCK
    #define KEY_CODE_SCROLL   VK_SCROLL
    #define KEY_CODE_LSHIFT   VK_LSHIFT
    #define KEY_CODE_RSHIFT   VK_RSHIFT
    #define KEY_CODE_LCTRL    VK_LCONTROL
    #define KEY_CODE_RCTRL    VK_RCONTROL
    #define KEY_CODE_LMENU    VK_LMENU
    #define KEY_CODE_RMENU    VK_RMENU
    #define KEY_CODE_LALT
    #define KEY_CODE_RALT
    #define KEY_CODE_ESCAPE   VK_ESCAPE
    #define KEY_CODE_TAB      VK_TAB
    #define KEY_CODE_BKSPACE  VK_BACK
    #define KEY_CODE_ENTER    VK_RETURN
    #define KEY_CODE_RETURN   VK_RETURN
    #define KEY_CODE_PAUSE
    #define KEY_CODE_CAPSLOCK
    #define KEY_CODE_BEGIN
#endif


};


#endif

