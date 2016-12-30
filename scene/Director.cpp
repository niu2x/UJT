#include "../GLMgr/GLMgr.h"
#include "../refCount/AutoReleasePool.h"
#include "Director.h"
Director *Director::instance = nullptr;

void Director::createInstance(){
    if(instance == nullptr){
        instance = new Director();
    }
}

void Director::deleteInstance(){
    if(instance != nullptr){
        delete instance;
        instance = 0;
    }
}

Director *Director::getInstance(){
    return instance;
}


void Director::pushScene(Scene *s){
    scenes.push(s);
}

void Director::popScene(){
    if(!scenes.empty())
        scenes.pop();
}

void Director::replaceScene(Scene *s){
    popScene();
    pushScene(s);
}

void Director::run(){
    GLMgr::getInstance()->run();
}

void Director::pushMatrix(MatrixType matType){
    getMatStack(matType)->push(Math::Mat4::IDENTITY);
}
void Director::popMatrix(MatrixType matType){
    getMatStack(matType)->pop();
}
void Director::loadMatrix(MatrixType matType, const Math::Mat4 &m){
    getMatStack(matType)->top() = m;
}

void Director::loadIdentityMatrix(MatrixType matType){
    loadMatrix(matType, Math::Mat4::IDENTITY);
}

Math::Mat4 Director::getMatrix(MatrixType matType){
    return getMatStack(matType)->top();
}

void Director::multiplyMatrix(MatrixType matType, const Math::Mat4 &m){
    getMatStack(matType)->top() = getMatStack(matType)->top() * m;
}


std::stack<Math::Mat4> *Director::getMatStack(MatrixType matType){
    switch(matType)
    {
        case MatrixType::kModel:
            return &modelMats;
        case MatrixType::kView:
            return &viewMats;
        case MatrixType::kProject:
            return &projectMats;
    }
}

bool Director::update(){
    return false;
}

void Director::render(){

}

Director::Director(){
    PoolManager::getInstance()->createInstance();
    GLMgr::getInstance()->createInstance();
    GLMgr::getInstance()->init(640, 480);
    GLMgr::getInstance()->setClearColor(0, 0, 0, 0.5);

    GLMgr::getInstance()->setRenderFunc([this](){
        this->render();
    });
    GLMgr::getInstance()->setUpdateFunc([this](){
        return this->update();
    });
}
Director::~Director(){
    while(!scenes.empty()){
        auto s = scenes.top();
        s->release();
        scenes.pop();
    }

    GLMgr::getInstance()->deleteInstance();
    PoolManager::getInstance()->deleteInstance();
}
