#ifndef _CACHE_ALGORITHMS_H
#define _CACHE_ALGORITHMS_H

#include "S2LazyQueue.h"

typedef unsigned long long ULLD;
class CacheAlgorithms{
public:
    virtual int update(ULLD item, ULLD visitTime) = 0;
    virtual void heartBeat() = 0;
    virtual void init() = 0;


};


#endif
