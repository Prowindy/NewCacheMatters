#ifndef _CACHE_ALGORITHMS_H
#define _CACHE_ALGORITHMS_H

class CacheAlgorithms{
public:
    virtual int update(unsigned long long item, unsigned long long visitTime) = 0;
    virtual void heartBeat() = 0;
    virtual void init() = 0;


};


#endif