#include "Camera.h"
#include "../math/Vec3.h"
#include "../math/Mat4.h"
#include "../math/Quaternion.h"
#include "Scene.h"

Camera::Camera(){

}

Camera::~Camera(){

}

Camera *Camera::create(){
	Camera *camera = new (std::nothrow) Camera();
	if(camera && camera->init()){
		camera->autorelease();
		return camera;
	}
	SAFE_DELETE(camera);
	return nullptr;
}

bool Camera::init(){
	if(!Node::init()){
		return false;
	}
	return true;
}

void Camera::lookAt(
	const Math::Vec3 &pos, 
	const Math::Vec3 &look, 
	const Math::Vec3 &_up
){

	setPosition(pos);
    
    Math::Vec3 up(_up);

	up.normalize();

    Math::Vec3 zaxis;
    Math::Vec3::subtract(pos, look, &zaxis);
    zaxis.normalize();

    Math::Vec3 xaxis;
    Math::Vec3::cross(up, zaxis, &xaxis);
    xaxis.normalize();

    Math::Vec3 yaxis;
    Math::Vec3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();

    float zAngle = Math::Vec3::angle(Math::Vec3(0, 0, 1), zaxis);
    Math::Vec3 zAngleAxis;
    Math::Vec3::cross(Math::Vec3(0, 0, 1), zaxis, &zAngleAxis);
    if(zAngleAxis.isZero()){
	    Math::Vec3::cross(Math::Vec3(1, 1, 1), zaxis, &zAngleAxis);
    }
    zAngleAxis.normalize();

    Math::Quaternion q1;
    Math::Mat4 q1M;
    Math::Quaternion::createFromAxisAngle(zAngleAxis, zAngle, &q1);
    Math::Mat4::createRotation(q1, &q1M);

    Math::Vec3 xaxisAfterQ1(1, 0, 0);
    q1M.transformPoint(&xaxisAfterQ1);

    float xAngle = Math::Vec3::angle(xaxisAfterQ1, xaxis);
    Math::Vec3 xAngleAxis;
    Math::Vec3::cross(xaxisAfterQ1, xaxis, &xAngleAxis);
    xAngleAxis.normalize();

    Math::Quaternion q2;
    Math::Mat4 q2M;
    Math::Quaternion::createFromAxisAngle(xAngleAxis, xAngle, &q2);
    Math::Mat4::createRotation(q2, &q2M);

    Math::Mat4 m = q2M * q1M;

    Math::Quaternion q;
    m.getRotation(&q);

    setQuaternion(q);
}

Math::Mat4 Camera::getViewMatrix()const{
	Math::Vec3 pos(0, 0, 0);
	Math::Vec3 look(0, 0, -1);
	Math::Vec3 up(0, 1, 0);

	Math::Mat4 transform = getNodeToWorldTransform();
	transform.transformPoint(&pos);
	transform.transformPoint(&look);
	transform.transformPoint(&up);

	Math::Mat4 viewMatrix;
	Math::Mat4::createLookAt(pos, look, up, &viewMatrix);

	return viewMatrix;
}

void Camera::setPerspective(
	float fieldOfView, 
	float aspectRatio, 
	float zNearPlane, 
	float zFarPlane
){
	Math::Mat4::createPerspective(
		fieldOfView, 
		aspectRatio, 
		zNearPlane, 
		zFarPlane, 
		&projectMatrix
	);
}

void Camera::setOrthographic(
	float width, 
	float height, 
	float zNearPlane, 
	float zFarPlane
){
	Math::Mat4::createOrthographic(
		width, 
		height, 
		zNearPlane, 
		zFarPlane, 
		&projectMatrix
	);
}

void Camera::__onEnterScene(){
	Node::__onEnterScene();

	auto scene = getScene();
	if(scene){
		scene->setCamera(this);
	}
}

void Camera::__onExitScene(){

	auto scene = getScene();
	if(scene){
		scene->setCamera(nullptr);
	}
	Node::__onExitScene();
}

