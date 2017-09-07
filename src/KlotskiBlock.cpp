#include "KlotskiBlock.hpp"
#include <stdio.h>

KlotskiBlock::KlotskiBlock()
{
    Width=Height=0;
    Shape=NULL;
    ImageMappingMask=NULL;
    Posx=Posy=0;
    Id=0;
}

KlotskiBlock::~KlotskiBlock()
{
    Width=Height=0;
    if(Shape!=NULL)
    {
        delete (Shape);
        Shape=NULL;
    }
    if(ImageMappingMask!=NULL)
    {
        delete (ImageMappingMask);
        ImageMappingMask=NULL;
    }
    Posx=Posy=0;
    Id=0;
}


inline int KlotskiBlock::CheckAroundLeft(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(x==0) return 0;
    if (Shape[index-1]==' ') return 0;
    return 1;
}

inline int KlotskiBlock::CheckAroundRight(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(x==Width-1) return 0;
    if (Shape[index+1]==' ') return 0;
    return 1;
}

inline int KlotskiBlock::CheckAroundUp(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(y==0) return 0;
    if (Shape[index-Width]==' ') return 0;
    return 1;
}

inline int KlotskiBlock::CheckAroundDown(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(y==Height-1) return 0;
    if (Shape[index+Width]==' ') return 0;
    return 1;
}

inline int KlotskiBlock::CheckAroundLeftUp(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(x==0 || y==0) return 0;
    if(Shape[index-Width-1]==' ')return 0;
    return 1;
}

inline int KlotskiBlock::CheckAroundRightUp(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(x==Width-1 || y==0) return 0;
    if(Shape[index-Width+1]==' ')return 0;
    return 1;
}

inline int KlotskiBlock::CheckAroundLeftDown(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(x==0 || y==Height-1) return 0;
    if(Shape[index+Width-1]==' ')return 0;
    return 1;
}

inline int KlotskiBlock::CheckAroundRightDown(int index)
{
    int x=index%Width;
    int y=index/Width;
    if(x==Width-1 || y==Height-1) return 0;
    if(Shape[index+Width+1]==' ')return 0;
    return 1;
}

int KlotskiBlock::GenImageMappingForDraw()
{
    int i;
    for(i=0;i<Width*Height;i++)
    {
        ImageMappingMask[i]=0xff;
        if(Shape[i]==' ') continue;
        int left=CheckAroundLeft(i);//7
        int right=CheckAroundRight(i);//3
        int up=CheckAroundUp(i);//1
        int down=CheckAroundDown(i);//5
        int leftup=CheckAroundLeftUp(i);//0
        int leftdown=CheckAroundLeftDown(i);//6
        int rightup=CheckAroundRightUp(i);//2
        int rightdown=CheckAroundRightDown(i);//4
        if(left)
            ImageMappingMask[i]&=~BIT7;
        if(right)
            ImageMappingMask[i]&=~BIT3;
        if(up)
            ImageMappingMask[i]&=~BIT1;
        if(down)
            ImageMappingMask[i]&=~BIT5;
        if(leftup && left && up)
            ImageMappingMask[i]&=~BIT0;
        if(rightup && right && up)
            ImageMappingMask[i]&=~BIT2;
        if(leftdown && left && down)
            ImageMappingMask[i]&=~BIT6;
        if(rightdown && right && down)
            ImageMappingMask[i]&=~BIT4;
    }
    return 0;	
}

void KlotskiBlock::Init(char* map, int width, int height, char c)
{
    int xmin=width-1;
    int xmax=0;
    int ymin=height-1;
    int ymax=0;
    
    int i;
    for(i=0;i<width*height;i++)
    {
        if(map[i]==c)
        {
            int x=i%width;
            int y=i/width;
            if(xmin>x) xmin=x;
            if(xmax<x) xmax=x;
            if(ymin>y) ymin=y;
            if(ymax<y) ymax=y; 
        }
    }
    Width=xmax-xmin+1;
    Height=ymax-ymin+1;
    Shape=new char[Width*Height];
    ImageMappingMask=new unsigned char[Width*Height];
    Posx=xmin;Posy=ymin;
    
    for(i=0;i<Width*Height;i++)
    {
        int x=i%Width;
        int y=i/Width;
        int xx=x+Posx;
        int yy=y+Posy;
        if(c==map[xx+yy*width])
        {
            Shape[i]='#';
        }
        else
        {
            Shape[i]=' ';
        }
    }
    
    Id=c;
    GenImageMappingForDraw();
}

void KlotskiBlock::DoMoveLeft()
{
    Posx--;
}

void KlotskiBlock::DoMoveRight()
{
    Posx++;
}

void KlotskiBlock::DoMoveUp()
{
    Posy--;
}

void KlotskiBlock::DoMoveDown()
{
    Posy++;
}

void KlotskiBlock::Print(int option)
{
    printf("BLOCK id=%c\n",Id);
    printf("w=%d h=%d\n",Width,Height);
    printf("position:(%d,%d)\n",Posx,Posy);
    printf("Shape:\n");
    int i,j;
    for(i=0;i<Height;i++)
    {
        for(j=0;j<Width;j++)
        {
            printf("%c",Shape[j+i*Width]);
        }
        printf("\n");
    }
}
