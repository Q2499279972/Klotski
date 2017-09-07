#ifndef _KLOTSKI_HPP_
#define _KLOTSKI_HPP_
#include "KlotskiMap.hpp"
#include "window.hpp"
#include "History.hpp"
#include "ReadLuaConfig.hpp"
#include "regex.h"
#include <sstream>

typedef enum _KlotskiState
{
    IDLE,
    NORMAL,
    FINISHED,
    AUTO,
    OTHER
} KlotskiState;

class Klotski
{
public:
    KlotskiMap *Map;
    History *His;
    KlotskiState state;
    int width;
    int height;
    
    unsigned int gameTargetType;
    int gameTargetId1;
    int gameTargetId2;
    int gameTargetdx,gameTargetdy;
    LuaConfig cfg;
    
    Klotski();
    ~Klotski();
    int checkTargetSuccess();
    int gameTargetAnalysis();
    void setState(KlotskiState s);  
    KlotskiState getState();
    int Init(char * luafile);
    void Deinit();
    void DoMove(int id, Direction direction);

};
#endif
