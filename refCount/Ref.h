#ifndef __BASE_CCREF_H__
#define __BASE_CCREF_H__

class Ref
{

public:
   
    void retain();

    void release();

    Ref* autorelease();

    unsigned int getReferenceCount() const;

protected:

    Ref();

public:

    virtual ~Ref();

protected:
    unsigned int _referenceCount;

    friend class AutoreleasePool;
};



#endif
