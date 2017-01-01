//
//  main.cpp
//  c
//
//  Created by Xiaorong Niu on 16/12/20.
//  Copyright © 2016年 Xiaorong Niu. All rights reserved.
//
#include "GLMgr/GLMgr.h"
#include "refCount/AutoReleasePool.h"

#include <stdio.h>
#include <string.h>
#include "scene/Director.h"

#include "macro.h"
#include "math/Vec3.h"

#include "scene/Scene.h"
#include "scene/Mesh.h"
#include "scene/Material.h"
#include "scene/Camera.h"

int main(void)
{

    Director::createInstance();

    auto scene = Scene::create();
    // scene->setCamera(Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
    Director::getInstance()->pushScene(scene);

    auto node = Node::create();
    node->setName("node");
    scene->addChild(node);

    auto mesh = Mesh::createCube(100);
    node->setMesh(mesh);

    auto material = Material::create();
    material->useDefaultShader();

    mesh->setMaterial(material);

    auto camera = Camera::create();

    scene->addChild(camera);
    camera->lookAt(Math::Vec3(300, 300, 300), Math::Vec3(0, 0, 0), Math::Vec3(0, 0, 1));
    camera->setPerspective(60, 640.0/480.0, 1, 10000);
    // camera->setOrthographic(640, 480, 1, 10000);

    Director::getInstance()->run();

    Director::deleteInstance();

    return 0;
}
