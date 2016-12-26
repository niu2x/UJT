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

#include "macro.h"

int main(void)
{
    puts("1");
    PoolManager::getInstance()->createInstance();

    GLMgr::getInstance()->createInstance();
    GLMgr::getInstance()->init(640, 480);
    GLMgr::getInstance()->setClearColor(0, 0, 0, 0.5);

    Vertex vertexs[4];
    vertexs[0].x = 0;
    vertexs[0].y = -1;
    vertexs[0].z = 0;
    vertexs[0].r = 1;
    vertexs[0].g = 0;
    vertexs[0].b = 0;
    vertexs[0].a = 1;
    vertexs[0].u = 0;
    vertexs[0].v = 0;

    vertexs[1].x = 1;
    vertexs[1].y = 0;
    vertexs[1].z = 0;
    vertexs[1].r = 0;
    vertexs[1].g = 1;
    vertexs[1].b = 0;
    vertexs[1].a = 1;
    vertexs[1].u = 0;
    vertexs[1].v = 1;

    vertexs[2].x = -1;
    vertexs[2].y = 0;
    vertexs[2].z = 0;
    vertexs[2].r = 0;
    vertexs[2].g = 0;
    vertexs[2].b = 1;
    vertexs[2].a = 1;
    vertexs[2].u = 1;
    vertexs[2].v = 0;

    vertexs[3].x = 0;
    vertexs[3].y = 1;
    vertexs[3].z = 0;
    vertexs[3].r = 0;
    vertexs[3].g = 0;
    vertexs[3].b = 1;
    vertexs[3].a = 1;
    vertexs[3].u = 1;
    vertexs[3].v = 0;

    GLushort indices[] = {1, 3, 2, 0, 1, 1}; 

    GLMgr::getInstance()->setRenderFunc([&vertexs, &indices](){
        GLMgr::getInstance()->clearColor();
        GLMgr::getInstance()->drawTriangles(vertexs, 4, indices, 6);
    });
    GLMgr::getInstance()->setUpdateFunc([&vertexs](){
        return false;
    });
    GLMgr::getInstance()->run();
    GLMgr::getInstance()->deleteInstance();

    PoolManager::getInstance()->deleteInstance();

    return 0;
}
