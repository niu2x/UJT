#include "../GLMgr/GLMgr.h"
#include "../refCount/AutoReleasePool.h"
#include "Director.h"

#include "Scene.h"
#include <math.h>

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
    if(!scenes.empty()){
        scenes.top()->onExitScene();
    }
    s->retain();
    s->onEnterScene();
    scenes.push(s);
}

void Director::popScene(){
    if(!scenes.empty()){
        auto s = scenes.top();
        scenes.pop();
        s->onExitScene();
        s->release();
        if(!scenes.empty()){
            scenes.top()->onEnterScene();
        }
    }
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

static float rx = 0;
static float ry = 0;
static float z = 0;

bool Director::update(){

    // scenes.top()->getNode("node")->setPosition(Math::Vec3(0, 0, -z));
    // z += 1;
    // if(z > 200){
    //     z = 0;
    // }
    scenes.top()->getNode("node")->setRotation(Math::Vec3(rx, sin(ry)*sin(sqrt(rx)), sin(rx)));
    scenes.top()->getNode("node")->setScale(Math::Vec3(1 + sin(ry)/2, 1+sin(rx)/2, 1+sin(rx+ry)/2));
    rx += 0.01;
    ry += 0.02;
    return false;
}

void Director::render(){
    auto glMgr = GLMgr::getInstance();
    glMgr->clearColor();
    glMgr->clearDepth();
    glMgr->clearStencil();
    scenes.top()->visit();
}

Director::Director(){
    PoolManager::getInstance()->createInstance();
    GLMgr::getInstance()->createInstance();
    GLMgr::getInstance()->init(640, 480);
    // GLMgr::getInstance()->setClearColor(0.1, 1, 0.1, 0);
    GLMgr::getInstance()->setClearColor(0.1, .1, .1, 0);

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

Scene* Director::getRunningScene(){
    if(!scenes.empty()){
        return scenes.top();
    }
    else{
        return nullptr;
    }
}
