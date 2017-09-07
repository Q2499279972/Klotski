#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "windows.h"
#include "gl\gl.h"
#include "gl\glu.h"

class Texture
{
public:
    GLuint TexId;

    void Bind();
    Texture(int width,int height,unsigned int format,unsigned char *data, int opt);

    static void Enable(bool yes)
    {
        if(yes)
            glEnable(GL_TEXTURE_2D);
        else
            glDisable(GL_TEXTURE_2D);
    }
    static void SphereMapping( const bool enable ) 
    {
        if ( enable ) 
        {
            glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
            glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP );
            glEnable( GL_TEXTURE_GEN_S );
            glEnable( GL_TEXTURE_GEN_T );
        }
        else 
        {
            glDisable( GL_TEXTURE_GEN_S );
            glDisable( GL_TEXTURE_GEN_T );		
        }
    }
};


//



void picgen_step1(unsigned int color);
extern unsigned int * buf2;
void picgen();
void picrelease();

#endif 
