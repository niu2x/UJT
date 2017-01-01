#include "Scene.h"
#include "Camera.h"
#include "Director.h"

Scene::Scene():camera(nullptr){

}

Scene::~Scene(){

}

Scene* Scene::create(){
	Scene * scene = new (std::nothrow) Scene();
	if(scene && scene->init()){
		scene->autorelease();
		return scene;
	}
	SAFE_DELETE(scene);
	return nullptr;
}

bool Scene::init(){
	if(!Node::init()){
		return false;
	}
	return true;
}


void Scene::visit(){
	if(camera){	
		auto director = Director::getInstance();
		director->pushMatrix(MatrixType::kView);
		director->loadMatrix(MatrixType::kView, camera->getViewMatrix());
		director->pushMatrix(MatrixType::kProject);
		director->loadMatrix(MatrixType::kProject, camera->getProjectMatrix());

		visit(Math::Mat4::IDENTITY);

		director->popMatrix(MatrixType::kProject);
		director->popMatrix(MatrixType::kView);
	}
}

void Scene::visit(const Math::Mat4 &parentModelMatrix){
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
			childNode->visit(Math::Mat4::IDENTITY); 
		}
	}

	director->popMatrix(MatrixType::kModel);
}


void Scene::setCamera(Camera *_camera){
	camera = _camera;
}
