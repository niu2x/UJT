#ifndef __Scene__
#define __Scene__


#include "../refCount/Ref.h"
#include "Node.h"

class Scene:public Node{
public:
	Scene *create();
	void 
private:
	Scene();
	virtual ~Scene();
};

#endif