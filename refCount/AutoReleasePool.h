#ifndef __AUTORELEASEPOOL_H__
#define __AUTORELEASEPOOL_H__

#include <vector>
#include <string>
#include "Ref.h"

class AutoreleasePool
{
public:
    AutoreleasePool();
    AutoreleasePool(const std::string &name);
    ~AutoreleasePool();
    void addObject(Ref *object);
    void clear();
    bool contains(Ref* object) const;
    void dump();
    
private:
    std::vector<Ref*> _managedObjectArray;
    std::string _name;
};


class PoolManager
{
public:

    static PoolManager* getInstance();
    
    static void deleteInstance();

    static void createInstance();
    
    AutoreleasePool *getCurrentPool() const;

    bool isObjectInPools(Ref* obj) const;

    friend class AutoreleasePool;
    
private:
    PoolManager();
    ~PoolManager();
    
    void push(AutoreleasePool *pool);
    void pop();
    
    static PoolManager* s_singleInstance;
    
    std::vector<AutoreleasePool*> _releasePoolStack;
};

#endif
