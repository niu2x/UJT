#ifndef __Scene__
#define __Scene__


#include "../refCount/Ref.h"
#include "Node.h"

class Camera;

class Scene:public Node{
public:
	static Scene *create();

	virtual void visit();
	virtual void visit(const Math::Mat4 &parentModelMatrix);

	void setCamera(Camera *);
	Camera *getCamera(){
		return camera;
	}

	virtual ~Scene();

protected:
	Scene();
	virtual bool init();

	Camera *camera;
};

#endif