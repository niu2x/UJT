#include "Ref.h"
#include "AutoreleasePool.h"

Ref::Ref()
: _referenceCount(1) 
{
}

Ref::~Ref()
{
}

void Ref::retain()
{
    ++_referenceCount;
}

void Ref::release()
{
    --_referenceCount;

    if (_referenceCount == 0)
    {
        delete this;
    }
}

Ref* Ref::autorelease()
{
    PoolManager::getInstance()->getCurrentPool()->addObject(this);
    return this;
}

unsigned int Ref::getReferenceCount() const
{
    return _referenceCount;
}
