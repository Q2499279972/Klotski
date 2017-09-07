#include "ReadLuaConfig.hpp"
#include <string>
#include <iostream>
using namespace std;

lua_State *L;

int ReadLuaConfig(LuaConfig *cfg,char *filename)
{
    printf("reading configure file %s\n",filename);
    L = lua_open();    
    luaL_openlibs(L);
    if(luaL_loadfile(L,filename) || lua_pcall(L,0,0,0)){    
        goto EXIT;
    }
    /////////////////////////////
    lua_getglobal(L,"width"); 
    if(!lua_isnumber(L,-1))
    {
        lua_pop(L, 1);
        goto EXIT;
    }
    else
    {    
        cfg->width = (int)lua_tonumber(L,-1);
        printf("width is %d\n",cfg->width);
        lua_pop(L, 1);
    }
    ////////////////////////////
    lua_getglobal(L,"height"); 
    if(!lua_isnumber(L,-1))
    {
        lua_pop(L, 1);
        goto EXIT;
    }
    else
    {    
        cfg->height = (int)lua_tonumber(L,-1);
        printf("height is %d\n",cfg->height);
        lua_pop(L, 1);
    }
    ////////////////////////////
    lua_getglobal(L,"title"); 
    if(!lua_isstring(L,-1))
    {
        lua_pop(L, 1);
        goto EXIT;
    }
    else
    {   
        size_t len;
        cfg->title = lua_tolstring (L,-1,&len);
        cout<<"title is"<<cfg->title<<endl;
        lua_pop(L, 1);
    }
    ////////////////////////////
    lua_getglobal(L,"map"); 
    if(!lua_isstring(L,-1))
    {
        lua_pop(L, 1);
        goto EXIT;
    }
    else
    {   
        size_t len;
        cfg->map = lua_tolstring (L,-1,&len);
        cout<<"map is"<<cfg->map<<endl;
        lua_pop(L, 1);
    }
    ////////////////////////////
    lua_getglobal(L,"author"); 
    if(!lua_isstring(L,-1))
    {
        lua_pop(L, 1);
        goto EXIT;
    }
    else
    {   
        size_t len;
        cfg->author = lua_tolstring (L,-1,&len);
        cout<<"author is"<<cfg->author<<endl;
        lua_pop(L, 1);
    }
    ////////////////////////////
    lua_getglobal(L,"help"); 
    if(!lua_isstring(L,-1))
    {
        lua_pop(L, 1);
        goto EXIT;
    }
    else
    {   
        size_t len;
        cfg->help = lua_tolstring (L,-1,&len);
        cout<<"help is"<<cfg->help<<endl;
        lua_pop(L, 1);
    }
    ////////////////////////////
    lua_getglobal(L,"target"); 
    if(!lua_isstring(L,-1))
    {
        lua_pop(L, 1);
        goto EXIT;
    }
    else
    {   
        size_t len;
        cfg->target = lua_tolstring (L,-1,&len);
        cout<<"target is"<<cfg->target<<endl;
        lua_pop(L, 1);
    }
    lua_close(L);
    return 1;
EXIT:
    lua_close(L);
    return 0;

}
