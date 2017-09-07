#include "Klotski.hpp"
Klotski::Klotski()
{
    Map=NULL;
    His=NULL;
    
    cfg.width=0;
    cfg.height=0;
    cfg.map="";
    cfg.title="";
    cfg.author="";
    cfg.help="";
    cfg.target="";
    state=IDLE;
}

Klotski::~Klotski()
{
    if(Map!=NULL)
    {
        delete Map;
        Map=NULL;
    }
    if(His!=NULL)
    {
        delete His;
        His=NULL;
    }
}

int Klotski::gameTargetAnalysis()
{
    gameTargetType=0;
    {
        const char pRegexStr[] = "^[A-Za-z0-9.*&]=[A-Za-z0-9.*&]$";
        const char *pText = cfg.target.c_str();//"*=.";
        regex_t oRegex;
        int nErrCode = 0;
        if ((nErrCode = regcomp(&oRegex, pRegexStr, REG_EXTENDED)) == 0)
        {
            if ((nErrCode = regexec(&oRegex, pText, 0, NULL, 0)) == 0)
            {
                regfree(&oRegex);
                gameTargetType=1;
                gameTargetId1=pText[0];//'*';
                gameTargetId2=pText[2];//'.';
                gameTargetdx=0;
                gameTargetdy=0;
                return 1;
            }
        }
    }
    
    {
        const char pRegexStr[] = "^[A-Za-z0-9.*&]-[A-Za-z0-9.*&]=\\(-?[0-9]+,-?[0-9]+\\)$";
        const char *pText = cfg.target.c_str();//"a-*=(1,1)";
        regex_t oRegex;
        int nErrCode = 0;
        if ((nErrCode = regcomp(&oRegex, pRegexStr, REG_EXTENDED)) == 0)
        {
            if ((nErrCode = regexec(&oRegex, pText, 0, NULL, 0)) == 0)
            {
                regfree(&oRegex);
                gameTargetType=2;
                gameTargetId1=pText[0];
                gameTargetId2=pText[2];
                int a,b,c;
                a=cfg.target.find("(");
                b=cfg.target.find(",");
                c=cfg.target.find(")");
                string sub1=cfg.target.substr(a+1,b-a-1);
                string sub2=cfg.target.substr(b+1,c-b-1);
                printf("%s\n",sub1.c_str());
                printf("%s\n",sub2.c_str());
                std::stringstream ss1;
                ss1<<sub1;
                ss1>>gameTargetdx;
                std::stringstream ss2;
                ss2<<sub2;
                ss2>>gameTargetdy;
                printf("gameTargetdx %d gameTargetdy %d",gameTargetdx,gameTargetdy);
                return 1;
            }
        }
    }
    return 0;
}

void Klotski::setState(KlotskiState s)
{
    state = s;
}

KlotskiState Klotski::getState()
{
    return state;
}
    

int Klotski::checkTargetSuccess()
{
    if(gameTargetType==1)
    {
        if(Map->SearchBlockById(gameTargetId1)->Posx==
           Map->SearchBlockById(gameTargetId2)->Posx &&
           Map->SearchBlockById(gameTargetId1)->Posy==
           Map->SearchBlockById(gameTargetId2)->Posy )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    if(gameTargetType==2)
    {
        printf("%d %d",Map->SearchBlockById(gameTargetId1)->Posx
          -Map->SearchBlockById(gameTargetId2)->Posx,gameTargetdx);
        printf("%d %d",Map->SearchBlockById(gameTargetId1)->Posy
          -Map->SearchBlockById(gameTargetId2)->Posy,gameTargetdy); 
        if(Map->SearchBlockById(gameTargetId1)->Posx
          -Map->SearchBlockById(gameTargetId2)->Posx==gameTargetdx &&
           Map->SearchBlockById(gameTargetId1)->Posy
          -Map->SearchBlockById(gameTargetId2)->Posy==gameTargetdy )
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}
    
int Klotski::Init(char * luafile)
{
    if(ReadLuaConfig(&cfg,luafile))
    {
        if(gameTargetAnalysis()==1)
        {    
            printf("gameTargetAnalysis OK\n");
            width= cfg.width;
            height=cfg.height;
            Map=new KlotskiMap(cfg.width,cfg.height,(char *)(cfg.map.c_str()));
            His=new History();
            if(Map->SearchBlockById(gameTargetId1)==NULL)
            {
                delete Map;
                delete His;
                return 0;
            }
            if(Map->SearchBlockById(gameTargetId2)==NULL)
            {
                delete Map;
                delete His;
                return 0;
            }
            return 1;
        }
        else
        {
            printf("lua file: target err!\n");
            return 0;
        }
    }
    else
    {
        printf("lua file err!\n");
        return 0;
    }
}
    
void Klotski::Deinit()
{
    if(Map!=NULL)
    {
        delete Map;
        Map=NULL;
    }
    if(His!=NULL)
    {
        delete His;
        His=NULL;
    }
}
    
void Klotski::DoMove(int id, Direction direction)
{
    if(state!=NORMAL)
        return;
    switch (direction)
    {
        case UP:
        {
            if(Map->CheckMoveable(Map->SearchBlockById(id),UP))
            {
                Map->SearchBlockById(id)->DoMoveUp();
                Map->UpdateMap();
                Map->Print();
                His->Add(id,UP);
            }
            break;
        }
        case DOWN:
        {
            if(Map->CheckMoveable(Map->SearchBlockById(id),DOWN))
            {
                Map->SearchBlockById(id)->DoMoveDown();
                Map->UpdateMap();
                Map->Print();
                His->Add(id,DOWN);
            }
            break;
        }
        case LEFT:
        {
            if(Map->CheckMoveable(Map->SearchBlockById(id),LEFT))
            {
                Map->SearchBlockById(id)->DoMoveLeft();
                Map->UpdateMap();
                Map->Print();
                His->Add(id,LEFT);
            }
            break;
        }
        case RIGHT:
        {
            if(Map->CheckMoveable(Map->SearchBlockById(id),RIGHT))
            {
                Map->SearchBlockById(id)->DoMoveRight();
                Map->UpdateMap();
                Map->Print();
                His->Add(id,RIGHT);
            }
            break;
        }
        default:
            break;
    }
}

