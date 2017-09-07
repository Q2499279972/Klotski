#ifndef _KLOTSKIMAP_HPP_
#define _KLOTSKIMAP_HPP_

#include "KlotskiBlock.hpp"
#include "KlotskiTypes.hpp"
#include "misc.hpp"

class KlotskiMap
{
public:
    int Width,Height;
    int BlockNumber;
    char *MapData;
    KlotskiBlock *Blocks;
    
    KlotskiMap(int width,int height,char *data);
    ~KlotskiMap();
    
    void SetMap(int x,int y, char value);
    void UpdateMap();
    void Print();
    int GetBlockId(int x, int y);
    KlotskiBlock* SearchBlockById(int id);
    int CheckMoveable(KlotskiBlock* block, Direction direction);

};


#endif
