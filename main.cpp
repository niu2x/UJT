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

int main(void)
{

    Vertex vertexs[5];
    vertexs[0].x = -1000;
    vertexs[0].y = -1000;
    vertexs[0].z = 0;
    vertexs[0].r = 1;
    vertexs[0].g = 0;
    vertexs[0].b = 0;
    vertexs[0].a = 1;
    vertexs[0].u = 0;
    vertexs[0].v = 0;

    vertexs[1].x = 1000;
    vertexs[1].y = -1000;
    vertexs[1].z = 0;
    vertexs[1].r = 0;
    vertexs[1].g = 1;
    vertexs[1].b = 0;
    vertexs[1].a = 1;
    vertexs[1].u = 1;
    vertexs[1].v = 0;

    vertexs[2].x = 1000;
    vertexs[2].y = 1000;
    vertexs[2].z = 0;
    vertexs[2].r = 0;
    vertexs[2].g = 0;
    vertexs[2].b = 1;
    vertexs[2].a = 1;
    vertexs[2].u = 1;
    vertexs[2].v = 1;

    vertexs[3].x = -1000;
    vertexs[3].y = 1000;
    vertexs[3].z = 0;
    vertexs[3].r = 0;
    vertexs[3].g = 0;
    vertexs[3].b = 1;
    vertexs[3].a = 1;
    vertexs[3].u = 0;
    vertexs[3].v = 1;

    vertexs[4].x = 0;
    vertexs[4].y = 0;
    vertexs[4].z = 0;
    vertexs[4].r = 0;
    vertexs[4].g = 0;
    vertexs[4].b = 1;
    vertexs[4].a = 1;
    vertexs[4].u = 0.5;
    vertexs[4].v = 0.5;

    GLushort indices[] = {4, 2, 3, 4, 0, 1, 4, 1, 2, 4, 3, 0};


    Director::createInstance();

    // auto scene = Scene::create();
    // scene->setCamera(Vec3(0, 0, 0), Vec3(0, 1, 0), Vec3(0, 0, 1));
    // Director::getInstance()->pushScene(scene);
    Director::getInstance()->run();

    Director::deleteInstance();

    return 0;
}
