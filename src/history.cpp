#include "history.hpp"

History::History()
{
    TotalStep=CurrentStep=0;
    StepList.clear();
}

History::~History()
{
    TotalStep=CurrentStep=0;
    StepList.clear();
}

void History::Add(char id, char move)
{
    if(TotalStep==CurrentStep)
    {
        if(CurrentStep%NODESIZE==0)
        {
            HistoryListNode *node = new HistoryListNode();
            node->Steps[0].id=id;
            node->Steps[0].move=move;
            StepList.push_back(*node);
            delete node;
            TotalStep++;
            CurrentStep++;
        }
        else
        {
            StepList.back().Steps[TotalStep%NODESIZE].id=id;
            StepList.back().Steps[TotalStep%NODESIZE].move=move;
            TotalStep++;
            CurrentStep++;
        }
    }
    else
    {
        int usefullnumber=(CurrentStep+NODESIZE-1)/NODESIZE;
        int totalnumber=(TotalStep+NODESIZE-1)/NODESIZE;
        int toremove=totalnumber-usefullnumber;
        while(toremove)
        {
            StepList.pop_back();
            toremove--;
        }
        TotalStep=CurrentStep;
        Add(id,move);
    }
}

int History::CanForward()
{
    return TotalStep!=CurrentStep;
}

int History::CanBackward()
{
    return CurrentStep!=0;
}

void History::DoForward()
{
    if(CurrentStep<TotalStep)
        CurrentStep++;
}

void History::DoBackward()
{
    if(CurrentStep>0)
        CurrentStep--;
}

StepInfo History::ReadHistory(int index)
{
    StepInfo ret={0,0};
    if(index<0 ||index>=TotalStep)
    {
        return ret;
    }
    int n1=index/NODESIZE;
    int n2=index%NODESIZE;
    list<HistoryListNode>::iterator i;
    i=StepList.begin();
    while(n1)
    {
        i++;
        n1--;
    }
    return i->Steps[n2];
}

StepInfo History::GetForwardStep()
{
    StepInfo ret={0,0};
    if(CanForward())
    {
        return ReadHistory(CurrentStep);
    }
    return ret;
}

StepInfo History::GetBackwardStep()
{
    StepInfo ret={0,0};
    if(CanBackward())
    {
        return ReadHistory(CurrentStep-1);
    }
    return ret;
}

Direction History::NegativeDirection(Direction direction)
{
    if(direction==UP) return DOWN;
    if(direction==DOWN) return UP;
    if(direction==LEFT) return RIGHT;
    if(direction==RIGHT) return LEFT;
    return ERR;
}
void History::PrintHistory()
{
    list<HistoryListNode>::iterator i;
    int k;
    int toprint=TotalStep;
    for(i=StepList.begin();i!=StepList.end();i++)
    {
        for(k=0;k<NODESIZE;k++)
        {
            printf("%c%c",i->Steps[k].id,i->Steps[k].move);
            toprint--;
            if(toprint==0) return;
        }
    }
}

void History::SaveHistory(FILE *file)
{
    list<HistoryListNode>::iterator i;
    int k;
    int toprint=TotalStep;
    for(i=StepList.begin();i!=StepList.end();i++)
    {
        for(k=0;k<NODESIZE;k++)
        {
            //printf("%c%c",i->Steps[k].id,i->Steps[k].move);
            char buf[2];
            buf[0]=i->Steps[k].id;
            buf[1]=i->Steps[k].move;
            fwrite( buf, 1, 2, file );
            toprint--;
            if(toprint==0) return;
        }
    }
}

int History::LoadHistory(FILE* file)
{
    TotalStep=CurrentStep=0;
    StepList.clear();
    char buf[2];
    while( 2==fread( buf, 1, 2, file ) )
    {
        if(buf[1]=='U' || buf[1]=='D' || buf[1]=='L' || buf[1]=='R')
            Add(buf[0],buf[1]);
        else
            return 0;
    }
    CurrentStep=0;
    printf("jhdsaaaaaaaaaaaaaaaaaa%d %d",CurrentStep,TotalStep);
    return 1;
}
