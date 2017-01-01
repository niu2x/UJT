#ifndef ___Camera___
#define ___Camera___

#include "../refCount/Ref.h"
#include "../math/Vec3.h"
#include "../math/Mat4.h"

#include "Node.h"

class Camera:public Node{
public:
	static Camera *create();

	void lookAt(const Math::Vec3 &pos, const Math::Vec3 &look, const Math::Vec3 &up);
	Math::Mat4 getViewMatrix()const;
	Math::Mat4 getProjectMatrix()const{
		return projectMatrix;
	}

	void setPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane);
    void setOrthographic(float width, float height, float zNearPlane, float zFarPlane);

	virtual ~Camera();

	virtual void __onEnterScene();
	virtual void __onExitScene();
protected:
	Camera();

	virtual bool init();

	Math::Mat4 projectMatrix;
};

#endif