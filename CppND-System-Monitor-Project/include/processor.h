#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector> 

class Processor {
public:
    float Utilization(); 

    enum ParamType{
        pUser = 0,
        pNice,
        pSystem,
        pIdle,
        pIowait,
        pIrq,
        pSoftirq,
        pSteal 
    }; 

private:
    std::vector<float> prev_state = {0, 0, 0, 0, 0, 0, 0, 0};     
};

#endif