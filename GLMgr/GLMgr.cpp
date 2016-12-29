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

        useDefaultShader();
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
        SAFE_RELEASE(glProgram);
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
	glClearColor(r*0xff, g*0xff, b*0xff, a*0xff);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices)*numOfIndices, indices, GL_DYNAMIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*num, vertex, GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES, numOfIndices, GL_UNSIGNED_SHORT, 0);
}

void GLMgr::useDefaultShader(){
    const GLchar* default_vertexSource = GLSL(

        uniform mat4 view;
        uniform mat4 project;

        in vec3 position;
        in vec2 uv;
        in vec4 color;
        out vec4 mColor;
        out vec2 mUV;
        void main() {
            mColor = color;
            mUV = uv;
            gl_Position = project * view * vec4(position, 1.0f);
        }
    );

    const GLchar* default_fragmentSource = GLSL(
        uniform sampler2D Texture0;
        uniform sampler2D Texture1;
        uniform sampler2D Texture2;
        uniform sampler2D Texture3;
        in vec4 mColor;
        in vec2 mUV;
        out vec4 oColor;
        void main() {
            oColor = texture(Texture0, mUV);
            oColor = oColor*texture(Texture1, mUV);
        }
    );

    auto shaderSource = GLProgramSource::createWithStrings(
        default_vertexSource, nullptr, nullptr, nullptr, default_fragmentSource
    );


    glProgram = GLProgram::createWithGLProgramSource(shaderSource);
    glProgram->retain();

    glProgram->use();


    auto view = Math::Mat4();
    Math::Mat4::createLookAt(
        Math::Vec3(0, 0, 2000),
        Math::Vec3(0, 0, 0),
        Math::Vec3(0, 1, 0),
        &view
    );
    glProgram->setUniformMatrix4fv("view", view.m);

    auto project = Math::Mat4();
    Math::Mat4::createPerspective(
        60,
        1,
        1,
        8000,
        &project
    );
    glProgram->setUniformMatrix4fv("project", project.m);
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
