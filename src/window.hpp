#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <windows.h>
#include "gl\gl.h"
#include "gl\glu.h"

typedef LRESULT CALLBACK (*WndProcCallBack_Ptr)(HWND,UINT,WPARAM,LPARAM);

class Window
{
public:
    HINSTANCE   hInstance;
    HDC         hDC;
    HGLRC       hRC;
    HWND        hWnd;
    
    int BlockNumber_x;
    int BlockNumber_y;
    int BlockPixel;
    int TotalPixel_x;
    int TotalPixel_y;
    WndProcCallBack_Ptr WndProc;
    
    Window(int nx,int ny,WndProcCallBack_Ptr wndproc);
    ~Window(){};
    int Create();
    int Destroy();
    int SetTitle(char * title);

};

#endif
