#include "RightButtonMenu.hpp"
using namespace std;

struct MenuResource
{
    UINT id;
    char name[128];
};
list<MenuResource> MenuList;

void MenuListInit()
{
    DIR *dir;
    struct dirent *ptr;
    int id=0;
    
    if ((dir=opendir("./levels")) == NULL) 
    { 
        perror("Open dir error..."); 
        exit(1);        
    }
    MenuList.clear();
    while ((ptr=readdir(dir)) != NULL) 
    { 
        if(strcmp(ptr->d_name,".")==0 ||
           strcmp(ptr->d_name,"..")==0)
           continue;
        if(strlen(ptr->d_name)>126)
           continue;
        
        //there need to check only valid lua file is here...  make it works first ....
        //there need to sort file is here...  make it works first ....
        string s(ptr->d_name);
        int pos;
        if(string::npos == (pos=s.find_last_of(".")))
        {
            continue;
        }
        else
        {
            if(s.substr(pos) != ".lua")
            {
                continue;
            }
            else
            {
                MenuResource *node = new MenuResource();
                node->id=id++;
                s.copy(node->name,pos,0);
                node->name[pos]=0;
                MenuList.push_back(*node);
                delete(node);
            }
        }
    }
    closedir(dir);
}


void InvokeRightMenu(HWND hWnd,int x,int y) 
{
    POINT p = {x, y};
    //ClientToScreen(hWnd, &p);
    HMENU hMainMenu = GetMenu(hWnd);
    HMENU m = CreatePopupMenu();
    {
        HMENU mm = CreatePopupMenu();
        AppendMenu(mm,MF_STRING,111," 菜单项11");
        AppendMenu(mm,MF_STRING,222," 菜单项21");
        InsertMenu(m,2,MF_BYPOSITION|MF_POPUP|MF_STRING,(UINT)mm,"子菜单"); 
    }
    //list<MenuResource>::iterator i;
    //for(i=MenuList.begin();i!=MenuList.end();i++)
    //{
    //    AppendMenu(m, MF_STRING, i->id, i->name);
    //}
    
    AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)m, "main");
    TrackPopupMenu(m, TPM_VERTICAL|TPM_TOPALIGN, p.x, p.y, 0, hWnd, NULL);
    DestroyMenu(m);
}
