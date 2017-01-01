#include "Node.h"
#include <math.h>
#include "Director.h"
#include <algorithm>
#include "Scene.h"
#include <string>
#include "../GLMgr/GLMgr.h"

#include "Mesh.h"
#include "Material.h"
static float clampf(float value, float min, float max){
    if(value < min){
        return min;
    }
    else if(value > max){
        return max;
    }
    else
        return value;
}

Node::Node():
position(0, 0, 0),
rotation(0, 0, 0),
scale(1, 1, 1),
visible(true),
parent(nullptr),
scene(nullptr),
mesh(nullptr),
running(false),
name("")
{

}

Node::~Node(){
	setMesh(nullptr);
	removeAllChildren();
}

Node* Node::create(){
	Node *node = new (std::nothrow) Node();
	if(node && node->init()){
		node->autorelease();
		return node;
	}
	SAFE_DELETE(node);
	return nullptr;
}


bool Node::init(){
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
	// Math::Mat4 mat4 = mat4X * mat4Y * mat4Z;
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
    setRotation(Math::Vec3(_rotationX, _rotationY, _rotationZ));
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
		if(childNode->isVisible()){
			childNode->visit(modelMatrix); 
		}
	}

	director->popMatrix(MatrixType::kModel);
}

void Node::draw(){
	if(!mesh)
		return;

	mesh->calculateBlendVertexs();

	Vertex *vertex = mesh->getVertexData();
	int numOfVertex = mesh->getVertexNum();

	GLushort *indices = mesh->getIndicesData();
	int numOfIndices = mesh->getIndicesNum();

	auto material = mesh->getMaterial();
	material->use();

	// printf("%x %d %x %d\n", vertex, numOfVertex, indices, numOfIndices);

	GLMgr::getInstance()->drawTriangles(vertex, numOfVertex, indices, numOfIndices);

	// printf("Vertex:\n");
	// for(int i = 0; i < numOfVertex; i ++){
	// 	printf("%02d: %f %f %f\n", i, vertex[i].x,vertex[i].y,vertex[i].z);
	// }
	// printf("Indices:\n");
	// for(int i = 0; i < numOfIndices; i ++){
	// 	printf("%02d: %d\n", i, indices[i]);
	// }

	// auto director = Director::getInstance();
 //    auto viewMatrix = director->getMatrix(MatrixType::kView);
 //    auto projectMatrix = director->getMatrix(MatrixType::kProject);

 //    printf("View:\n");

 //    for(int r = 0; r < 4; r ++){
 //    	for(int c = 0; c < 4; c ++){
 //    		printf("%f ", viewMatrix.m[c*4+r]);
 //    	}
 //    	printf("\n");
 //    }
 //    printf("\n");


 //    printf("kProject:\n");

 //    for(int r = 0; r < 4; r ++){
 //    	for(int c = 0; c < 4; c ++){
 //    		printf("%f ", projectMatrix.m[c*4+r]);
 //    	}
 //    	printf("\n");
 //    }
 //    printf("\n");
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
	auto iter = std::find_if(children.begin(), children.end(), [&name](const NodePtr &ptr){
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

	std::for_each(temp.begin(), temp.end(), [this](Node *childNode){
		removeChild(childNode);
	});
}

void Node::__addChild(Node *node){
	node->retain();

	children.push_back(node);
	node->parent = this;

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
	node->parent = nullptr;

	node->release();
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
			auto sepPos = path.find('/');
			std::string nextNodeName = path.substr(0, sepPos);
			Node *nextNode = nullptr;
			if(nextNodeName == std::string(".")){
				nextNode = this;
			}
			else if(nextNodeName == std::string("..")){
				nextNode = getParent();
			}
			else{
				nextNode = getChildByName(nextNodeName);
			}

            std::string nextPath;
            if(sepPos != std::string::npos){
            	nextPath = path.substr(sepPos+1, std::string::npos);
            }
			if(nextNode){
				if(nextPath.length() > 0 ){
					return nextNode->getNode(nextPath);
				}
				else{
					return nextNode;
				}
			}
			else{
				return nullptr;
			}
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
		mesh->attachTo(this);
	}
}

Mesh *Node::getMesh(){
	return mesh;
}

Math::Mat4 Node::getNodeToAncestorTransform(Node *ancestor)const{
	Math::Mat4 ret;
	const Node *parent = this;

	while(parent && parent != ancestor){
		ret = parent->getNodeToParentTransform() * ret;
		parent = parent -> getParent();
	}

	return ret;
}

Math::Mat4 Node::getAncestorToNodeTransform(Node *ancestor)const{
	return getNodeToAncestorTransform(ancestor).getInversed();
}

void Node::onEnterScene(){
	__onEnterScene();
	std::for_each(children.begin(), children.end(), [](Node *childNode){
		childNode->onEnterScene();
	});
}

void Node::onExitScene(){
	std::for_each(children.rbegin(), children.rend(), [](Node *childNode){
		childNode->onExitScene();
	});
	__onExitScene();
}

void Node::__onEnterScene(){
	running = true;

	Node *p = this;
	Scene *s = nullptr;
	do{
		p = p->getParent();
		if(p){
			s = dynamic_cast<Scene *>(p);
		}
	}while(s == nullptr && p != nullptr);
	setScene(s);
}

void Node::__onExitScene(){
	running = false;

	setScene(nullptr);
}

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
