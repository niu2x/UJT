#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Texture2D.h"
#include "Texture2DManager.h"

#include "GLMgr.h"

#include "GLProgramSource.h"
#include "AutoReleasePool.h"

#include "../math/BaseMath.h"
#include "GLProgram.h"

#include "../macro.h"

GLMgr *GLMgr::instance = nullptr;

void GLMgr::createInstance(){
	if(instance == nullptr){
		instance = new GLMgr();
	}
}

void GLMgr::deleteInstance(){
	if(instance != nullptr){
		delete instance;
		instance = 0;
	}
}

GLMgr *GLMgr::getInstance(){
	return instance;
}

GLMgr::GLMgr():
inited(false), 
width(0), 
height(0), 
targetFPS(0),
lastUpdateTime(0){

    Texture2DManager::createInstance();
}

bool GLMgr::init(int _width, int _height){
	if(!inited){
		width = _width;
		height = _height;

		if (!glfwInit())
        	return inited;
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_RED_BITS, 5);
        glfwWindowHint(GLFW_GREEN_BITS, 6);
        glfwWindowHint(GLFW_BLUE_BITS, 5);
        glfwWindowHint(GLFW_ALPHA_BITS, 0);
        glfwWindowHint(GLFW_DEPTH_BITS, 16);
        glfwWindowHint(GLFW_STENCIL_BITS, 0);

        window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);

        if(!window){
	        glfwTerminate();
    	    return inited;
        }

        glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;

    	if(glewInit() != GLEW_OK){
    		glfwTerminate();
    		return inited;
    	}
        
        glViewport(0, 0, width, height);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glEnable(GL_BLEND);

    	initVAOAndVBO();

        setFPS(32);

        inited = true;
	}

	return inited;
}

void GLMgr::initVAOAndVBO(){
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

GLMgr::~GLMgr(){
    Texture2DManager::deleteInstance();
	if(inited){
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
		glfwTerminate();
	}
}

void GLMgr::setRenderFunc(RenderFunc _renderFunc){
	renderFunc = _renderFunc;
}


void GLMgr::setUpdateFunc(UpdateFunc _updateFunc){
    updateFunc = _updateFunc;
}

void GLMgr::run(){
	while (!glfwWindowShouldClose(window))
    {
        struct timeval now;
        gettimeofday(&now, nullptr);
        unsigned long nowTime = now.tv_sec*1000000 + now.tv_usec;
        if(nowTime - lastUpdateTime >= uSecDuration){
            updateFunc();
            renderFunc();
            glfwSwapBuffers(window);
            glFlush();
            lastUpdateTime = nowTime;
        }
        else{
            //sleep(uSecDuration - (nowTime - lastUpdateTime));
        }
        glfwPollEvents();
        PoolManager::getInstance()->getCurrentPool()->clear();
    }
}

void GLMgr::setClearColor(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
}

void GLMgr::clearColor(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLMgr::clearDepth(){
	glClear(GL_DEPTH_BUFFER_BIT);
}

void GLMgr::clearStencil(){
	glClear(GL_STENCIL_BUFFER_BIT);
}

void GLMgr::drawTriangles(
    const Vertex vertex[], 
    int num,
    const GLushort indices[],
    int numOfIndices
){
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*numOfIndices, indices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*num, vertex, GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_SHORT, 0);
}

void GLMgr::setFPS(float __targetFPS){
    targetFPS = __targetFPS;
    uSecDuration = 1000000 / __targetFPS;
}

void GLMgr::bindTexture2D(Texture2D *texture2D, GLint index){
    GLuint tex2dID = texture2D->getTexID();
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, tex2dID);
}
