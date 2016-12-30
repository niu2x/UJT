#include "Node.h"
#include <math.h>
#include "Director.h"
#include <algorithm>

Node::Node:
position(0, 0, 0),
rotation(0, 0, 0),
scale(1, 1, 1),
isVisible(true),
parent(nullptr),
scene(nullptr),
mesh(nullptr),
isRunning(false),
name("")
{

}

Node::~Node(){
	setMesh(nullptr);
	removeAllChildren();
}

Node *Node::create(){
	create("")
}

Node* Node::create(const std::string &name){
	Node *node = new (std::nothrow) Node();
	if(node && node->init(name)){
		node->autorelease();
		return node;
	}
	SAFE_DELETE(node);
	return nullptr;
}


bool Node::init(const std::string &_name){
	setName(_name);
	return true;
}

void Node::setPosition(const Math::Vec3 &pos){
	position = pos;	
}

void Node::setScale(const Math::Vec3 &s){
	scale = s;
}

void Node::setRotation(const Math::Vec3 &r){
	rotation = r;
	Math::Mat4 mat4X, mat4Y, mat4Z;
	Math::Mat4::createRotationX(rotation.x, &mat4X);
	Math::Mat4::createRotationY(rotation.y, &mat4Y);
	Math::Mat4::createRotationZ(rotation.z, &mat4Z);
	Math::Mat4 mat4 = mat4Z * mat4Y * mat4X;
	Math::Quaternion q;
	Math::Quaternion::createFromRotationMatrix(mat4, &q);
	quat = q;
}

void Node::setQuaternion(const Math::Vec3 &axis, float angle){
	Math::Quaternion q;
	Math::Quaternion::createFromAxisAngle(axis, angle, &q);
	setQuaternion(q);
}

void Node::setQuaternion(const Math::Quaternion &_quat){
	quat = _quat;
	float x = quat.x, y = quat.y, z = quat.z, w = quat.w;
	float _rotationX, _rotationY, _rotationZ;
    _rotationX = atan2f(2.f * (w * x + y * z), 1.f - 2.f * (x * x + y * y));
    float sy = 2.f * (w * y - z * x);
    sy = clampf(sy, -1, 1);
    _rotationY = asinf(sy);
    _rotationZ = atan2f(2.f * (w * z + x * y), 1.f - 2.f * (y * y + z * z));
    setRotation(Vec3(_rotationX, _rotationY, _rotationZ));
}

Math::Quaternion Node::getQuaternion()const{
	return quat;
}

void Node::setVisible(bool bVisible){
	visible = bVisible;
}

void Node::visit(){
	visit(Math::Mat4::IDENTITY);
}

void Node::visit(const Math::Mat4 &parentModelMatrix){
	auto modelMatrix = parentModelMatrix * getNodeToParentTransform();

	auto director = Director::getInstance();
	director->pushMatrix(MatrixType::kModel);
	director->loadMatrix(MatrixType::kModel, modelMatrix);

	if(visible){
		draw();
	}

	for(auto iter = children.begin(); iter != children.end(); iter ++){
		Node *childNode = *iter;
		if(childNode->visible){
			childNode->visit(modelMatrix); 
		}
	}

	director->popMatrix(MatrixType::kModel);
}

void Node::draw(){

}

void Node::addChild(Node *node){
	if(!hasChild(node)){
		__addChild(node);
	}
}

bool Node::hasChild(Node *node){
	auto iter = std::find(children.begin(), children.end(), node);
	return iter != children.end();
}

Node *Node::getChildByName(const std::string &name){
	typedef Node * NodePtr;
	auto iter = std::find_if(children.begin(), children.end(), [](const NodePtr &ptr){
		return ptr->name == name;
	});
	if(iter != children.end()){
		return *iter;
	}
	else{
		return nullptr;
	}
}

void Node::removeChild(Node *node){
	if(hasChild(node)){
		__removeChild(node);
	}
}

void Node::removeAllChildren(){
	std::vector<Node *> temp(children);

	std::for_each(temp.begin(), temp.end(), [](Node *childNode){
		removeChild(childNode);
	});
}

void Node::__addChild(Node *node){
	node->retain();

	children.push_back(node);

	if(running){
		node->onEnterScene();
	}
}

void Node::__removeChild(Node *node){
	if(running){
		node->onExitScene();
	}

	auto iter = std::find(children.begin(), children.end(), node);
	children.erase(iter);	

	node->release();
}

Node *Node::getParent(){
	return parent;
}

Scene *Node::getScene(){
	return scene;
}

void Node::setScene(Scene *_scene){
	scene = _scene;
}

Node *Node::getNode(const std::string &path){
	if(path.length() <= 0){
		return nullptr;
	}
	else{
		if(path[0] == '/'){
			if(scene){
				return scene->getNode(path.substr(1));
			}
			else{
				return nullptr;
			}
		}
		else{
			
		}
	}
}

void Node::setName(const std::string &_name){
	name = _name;
}

std::string Node::getName()const{
	return name;
}

void Node::setMesh(Mesh *_mesh){
	if(mesh){
		mesh->release();
		mesh = nullptr;
	}

	if(_mesh){
		mesh = _mesh;
		mesh->retain();
	}
}

Mesh *Node::getMesh(){
	return mesh;
}

void Node::onEnterScene();
void Node::onExitScene();

Math::Mat4 Node::getNodeToParentTransform()const{
	Math::Mat4 mat_translate;
	Math::Mat4 mat_scale;
	Math::Mat4 mat_rotation;

	Math::Mat4::createTranslation(position, &mat_translate);
	Math::Mat4::createScale(scale, &mat_scale);
	Math::Mat4::createRotation(quat, &mat_rotation);

	return mat_translate * mat_rotation * mat_scale;
}

Math::Mat4 Node::getParentToNodeTransform()const{
	return getNodeToParentTransform().getInversed();
}

Math::Mat4 Node::getNodeToWorldTransform()const{
	Math::Mat4 transform(getNodeToParentTransform());

	auto parent = getParent();
	while(parent){
		transform = parent->getNodeToParentTransform() * transform;
		parent = parent->getParent();
	}

	return transform;
}

Math::Mat4 Node::getWorldToNodeTransform()const{
	return getNodeToWorldTransform().getInversed();
}