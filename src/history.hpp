#ifndef _HISTORY_HPP_
#define _HISTORY_HPP_

#include <list> 
using namespace std;

#include "KlotskiTypes.hpp"

class History
{
private:
    static const int NODESIZE =1024;
    struct HistoryListNode
    {
        StepInfo Steps[NODESIZE];
    };
public:    
    int TotalStep;
    int CurrentStep;
    list<HistoryListNode> StepList;
    StepInfo ReadHistory(int index);
public:
    History();
    ~History();
    void Add(char id, char move);
    int CanForward();
    int CanBackward();
    void DoForward();
    void DoBackward();
    StepInfo GetForwardStep();
    StepInfo GetBackwardStep();
    Direction NegativeDirection(Direction direction);
    void PrintHistory();
    void SaveHistory(FILE *file);
    int LoadHistory(FILE *file);
};

#endif
