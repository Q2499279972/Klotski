#include "KlotskiMap.hpp"
#include <string.h>
#include <stdio.h>

KlotskiMap::KlotskiMap(int width,int height,char *data)
{
    Width=width;
    Height=height;
    
    int N=Width*Height;
    int i;
    char list[256]={0};
    int listindex=0;
    for(i=0;i<N;i++)
    {
        if(data[i]==' ') continue;
        if(IsInString(data[i],list)==0)
        {
            list[listindex++]=data[i];
        }
    }
    
    BlockNumber=listindex;
    BlockNumber=listindex;
    MapData=new char[Width*Height];
    Blocks = new KlotskiBlock[listindex];
    
    for(i=0;i<listindex;i++)
    {
        Blocks[i].Init(data,width,height,list[i]);
        Blocks[i].Print();
    }
}

KlotskiMap::~KlotskiMap()
{
    delete MapData;
    delete Blocks;
}

void KlotskiMap::SetMap(int x,int y, char value)
{
    MapData[y*Width+x]=value;
}

void KlotskiMap::UpdateMap()
{
    int i,j;
    memset(MapData,' ',Width*Height);
	KlotskiBlock *pp=NULL;
    for(i=0;i<BlockNumber;i++)
    {
        KlotskiBlock *p;
        p=&Blocks[i];
		if(p->Id=='-')
		{
			pp=p;
			continue;
		}
        for(j=0;j<p->Width*p->Height;j++)
        {
            if(p->Shape[j]==' ') continue;
            int xoffset=p->Posx;
            int yoffset=p->Posy;
            int x=j%p->Width + xoffset;
            int y=j/p->Width + yoffset;
            SetMap(x,y,p->Id);
        }
    }
	if(pp)
	{
		for(j=0;j<pp->Width*pp->Height;j++)
        {
            if(pp->Shape[j]==' ') continue;
            int xoffset=pp->Posx;
            int yoffset=pp->Posy;
            int x=j%pp->Width + xoffset;
            int y=j/pp->Width + yoffset;
            if(MapData[x+y*Width]==' ')
			{
				SetMap(x,y,pp->Id);
			}
        }
	}
}

void KlotskiMap::Print()
{
    int i,j;
    printf("map\n");
    for(i=0;i<Height;i++)
    {
        for(j=0;j<Width;j++)
        {
            printf("%c",MapData[j+i*Width]);
        }
        printf("\n");
    }
}

int KlotskiMap::GetBlockId(int x, int y)
{
    return MapData[x+y*Width];
}

KlotskiBlock* KlotskiMap::SearchBlockById(int id)
{
    KlotskiBlock *p;
    p=Blocks;
    int i;
    for(i=0;i<BlockNumber;i++)
    {
        if(p->Id==id)
            return p;
        p++;
    }
    return NULL;
}

int KlotskiMap::CheckMoveable(KlotskiBlock* block, Direction direction)
{
    if(block->Id=='#' || block->Id=='-' || block->Id=='.')
        return 0;
    
    if(block->Id!='*')
    {
        switch (direction)
        {
            int j;
            case UP:
            {
                if(block->Posy==0)
                    return 0;
                for(j=0;j<(block->Width)*(block->Height);j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx;
                    int yoffset=block->Posy-1;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if( MapData[x+y*Width]!=block->Id && 
                        MapData[x+y*Width]!=' ')
                    {   
                        return 0;
                    }
                }
                return 1;
            }
            
            case DOWN:
            {
                if(block->Posy==Height-block->Height)
                    return 0;
                for(j=0;j<block->Width*block->Height;j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx;
                    int yoffset=block->Posy+1;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if( MapData[x+y*Width]!=block->Id
                     && MapData[x+y*Width]!=' ')
                    {
                        return 0;
                    }
                }
                return 1;
            }
            
            case LEFT:
            {
                if(block->Posx==0)
                    return 0;
                for(j=0;j<block->Width*block->Height;j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx-1;
                    int yoffset=block->Posy;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if( MapData[x+y*Width]!=block->Id
                     && MapData[x+y*Width]!=' ')
                    {
                        return 0;
                    }
                }
                return 1;
            }
            
            case RIGHT:
            {
                if(block->Posx==Width-block->Width)
                    return 0;
                for(j=0;j<block->Width*block->Height;j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx+1;
                    int yoffset=block->Posy;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if(MapData[x+y*Width]!=block->Id
                    && MapData[x+y*Width]!=' ')
                    {
                        return 0;
                    }
                }
                return 1;
            }
            default:
                return 0;
        }
    }
    
    if(block->Id=='*')
    {
        switch (direction)
        {
            int j;
            case UP:
            {
                if(block->Posy==0)
                    return 0;
                for(j=0;j<(block->Width)*(block->Height);j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx;
                    int yoffset=block->Posy-1;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if( MapData[x+y*Width]!=block->Id && 
                        MapData[x+y*Width]!=' ' &&
                        MapData[x+y*Width]!='.' &&
                        MapData[x+y*Width]!='-' )
                    {   
                        return 0;
                    }
                }
                return 1;
            }
            
            case DOWN:
            {
                if(block->Posy==Height-block->Height)
                    return 0;
                for(j=0;j<block->Width*block->Height;j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx;
                    int yoffset=block->Posy+1;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if( MapData[x+y*Width]!=block->Id && 
                        MapData[x+y*Width]!=' ' &&
                        MapData[x+y*Width]!='.' &&
                        MapData[x+y*Width]!='-' )
                    {
                        return 0;
                    }
                }
                return 1;
            }
            
            case LEFT:
            {
                if(block->Posx==0)
                    return 0;
                for(j=0;j<block->Width*block->Height;j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx-1;
                    int yoffset=block->Posy;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if( MapData[x+y*Width]!=block->Id && 
                        MapData[x+y*Width]!=' ' &&
                        MapData[x+y*Width]!='.' &&
                        MapData[x+y*Width]!='-' )
                    {
                        return 0;
                    }
                }
                return 1;
            }
            
            case RIGHT:
            {
                if(block->Posx==Width-block->Width)
                    return 0;
                for(j=0;j<block->Width*block->Height;j++)
                {
                    if(block->Shape[j]==' ') continue;
                    int xoffset=block->Posx+1;
                    int yoffset=block->Posy;
                    int x=j%block->Width + xoffset;
                    int y=j/block->Width + yoffset;
                    if( MapData[x+y*Width]!=block->Id && 
                        MapData[x+y*Width]!=' ' &&
                        MapData[x+y*Width]!='.' &&
                        MapData[x+y*Width]!='-' )
                    {
                        return 0;
                    }
                }
                return 1;
            }
            default:
                return 0;
        }
    }
}
