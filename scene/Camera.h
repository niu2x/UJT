#ifndef ___Camera___
#define ___Camera___

#include "../refCount/Ref.h"
#include "../math/Vec3.h"
#include "../math/Mat4.h"

#include "Node.h"

class Camera:public Node{
public:
	Camera *create();

	void lookAt(const Math::Vec3 &pos, const Math::Vec3 &look, const Math::Vec3 &up);
	Math::Mat4 getViewMatrix()const;

private:
	Camera();
	virtual ~Camera();
};

#endif