#ifndef _RIGHTBUTTONMENU_HPP_
#define _RIGHTBUTTONMENU_HPP_
#include <windows.h>
#include <dirent.h>
#include <list>
#include <string>
using namespace std;



void MenuListInit();
void InvokeRightMenu(HWND hWnd,int x,int y);

#endif
