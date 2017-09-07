#ifndef _KLOTSKIBLOCK_HPP_
#define _KLOTSKIBLOCK_HPP_

#include "KlotskiTypes.hpp"
#include <cstddef>
/*
coordinate:
      x
   0 1 2 3 4 5 .
y  1
   2
   3
   4
   .
*/
class KlotskiBlock
{
public:
    int Width,Height;
    char *Shape;
    unsigned char *ImageMappingMask;
    int Posx,Posy;
    int Id;
    
    KlotskiBlock();
    ~KlotskiBlock();

private:    
    inline int CheckAroundLeft(int index);
    inline int CheckAroundRight(int index);
    inline int CheckAroundUp(int index);
    inline int CheckAroundDown(int index);
    inline int CheckAroundLeftUp(int index);
    inline int CheckAroundRightUp(int index);
    inline int CheckAroundLeftDown(int index);
    inline int CheckAroundRightDown(int index);

public:    
    inline int GenImageMappingForDraw();

    void Init(char* map, int width, int height, char c);
    void Init(char* filename);
    
    //JUST DO WITHOUT CHECK HERE !!
    void DoMoveLeft();
    void DoMoveRight();
    void DoMoveUp();
    void DoMoveDown();

    void Print(int option = 0);
    
public:
    void Draw();
};

#endif
