#include "texture.hpp"
void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, TexId);
}

Texture::Texture(int width,int height,unsigned int format,unsigned char *data, int opt)
{
	glGenTextures(0,&TexId);
	glBindTexture(GL_TEXTURE_2D,TexId);
	
	switch(opt)
	{
	case 0:	// No interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case 1:	// Linear interpolation
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case 2:	// Linear interpolation w/mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);
}

/// ugly image ...
#define WIDTH 32
#define HEIGHT 32
unsigned int buf[16*WIDTH][16*HEIGHT];
void picgen_step1(unsigned int color)
{
    int w=2;
    int i,j,k;
    for(i=0;i<256;i++)
    {
        int x=i%16;
        int y=i/16;
        
        int a0,a1,a2,a3,a4,a5,a6,a7;
        a0=(i>>0)&1;
        a1=(i>>1)&1;
        a2=(i>>2)&1;
        a3=(i>>3)&1;
        a4=(i>>4)&1;
        a5=(i>>5)&1;
        a6=(i>>6)&1;
        a7=(i>>7)&1;

        for(j=0;j<HEIGHT;j++)
        {
            for(k=0;k<WIDTH;k++)
            {
                if(j<w)
                {
                    if(k<w)//0
                    {
                        if(a0) goto C1;
                        goto C2;
                    }
                    else if(k>WIDTH-w-1)//2
                    {
                        if(a2) goto C1;
                        goto C2;
                    }
                    else//1
                    {
                        if(a1) goto C1;
                        goto C2;
                    }
                }
                else if(j>HEIGHT-w-1)
                {
                    if(k<w)//6
                    {
                        if(a6) goto C1;
                        goto C2;
                    }
                    else if(k>WIDTH-w-1)//4
                    {
                        if(a4) goto C1;
                        goto C2;
                    }
                    else//5
                    {
                        if(a5) goto C1;
                        goto C2;
                    }
                }
                else 
                {
                    if(k<w)//7
                    {
                        if(a7) goto C1;
                        goto C2;
                    }
                    else if(k>WIDTH-w-1)//3
                    {
                        if(a3) goto C1;
                        goto C2;
                    }
                    else
                    {
                        goto C2;
                    }
                }
            C1:
            buf[y*HEIGHT+j][x*WIDTH+k]=0xffffffff;
            continue;
            C2:
            buf[y*HEIGHT+j][x*WIDTH+k]=color;
            continue;
            }
        }	
    }
}

unsigned int * buf2;
void picgen()
{

    buf2=new unsigned int [16*WIDTH*16*HEIGHT*4];
    picgen_step1(0x0066ddff);
    memcpy(buf2+16*WIDTH*16*HEIGHT*0, &buf[0][0],16*WIDTH*16*HEIGHT*4);
    picgen_step1(0x000000ff);
    memcpy(buf2+16*WIDTH*16*HEIGHT*1, &buf[0][0],16*WIDTH*16*HEIGHT*4);
    picgen_step1(0x00888888);
    memcpy(buf2+16*WIDTH*16*HEIGHT*2, &buf[0][0],16*WIDTH*16*HEIGHT*4);
    picgen_step1(0x000000ff);
    memcpy(buf2+16*WIDTH*16*HEIGHT*3, &buf[0][0],16*WIDTH*16*HEIGHT*4);
}

void picrelease()
{
    delete (buf2);
}
