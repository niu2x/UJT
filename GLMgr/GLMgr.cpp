#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Texture2D.h"

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
            sleep(uSecDuration - (nowTime - lastUpdateTime));
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
            // oColor = mColor;
            // oColor = vec4(mUV, 1, 1);
            oColor = texture(Texture0, mUV) * texture(Texture1, mUV);

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
        Math::Vec3(0, 1000, 0),
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

    useTestTexture();
}

// GLuint createShader(GLenum type, const GLchar* src)
// {
//     GLuint shader = glCreateShader(type);

//     glShaderSource(shader, 1, &src, nullptr);

//     glCompileShader(shader); 

//     return shader;
// }

// void GLMgr::useShader(const ShaderSource *shaderSource){
//     GLuint shaderProgram = glCreateProgram();

//     if(shaderSource->getVertexSource()){
//         glAttachShader(shaderProgram, createShader(
//             GL_VERTEX_SHADER, shaderSource->getVertexSource()
//         ));
//     }

//     if(shaderSource->getTessCtrlSource()){
//         glAttachShader(shaderProgram, createShader(
//             GL_TESS_CONTROL_SHADER, shaderSource->getTessCtrlSource()
//         ));
//     }

//     if(shaderSource->getTessEvalSource()){
//         glAttachShader(shaderProgram, createShader(
//             GL_TESS_EVALUATION_SHADER, shaderSource->getTessEvalSource()
//         ));
//     }

//     if(shaderSource->getGeometrySource()){
//         glAttachShader(shaderProgram, createShader(
//             GL_GEOMETRY_SHADER, shaderSource->getGeometrySource()
//         ));
//     }

//     if(shaderSource->getFragmentSource()){
//         glAttachShader(shaderProgram, createShader(
//             GL_FRAGMENT_SHADER, shaderSource->getFragmentSource()
//         ));
//     }

//     char log[1024];
//     glGetProgramInfoLog(shaderProgram, 1023, 0, log);
//     puts(log);

//     glLinkProgram(shaderProgram);

//     glUseProgram(shaderProgram);       

//     GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
//     // GLint posAttrib = 0;
//     glEnableVertexAttribArray(posAttrib);

//     glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

//     GLint uvAttrib = glGetAttribLocation(shaderProgram, "uv");
//     // GLint uvAttrib = 1;
//     glEnableVertexAttribArray(uvAttrib);

//     glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);

//     GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
//     // GLint colorAttrib = 2;
//     glEnableVertexAttribArray(colorAttrib);

//     glVertexAttribPointer(colorAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)20);

//     GLint tex0Location = glGetUniformLocation(shaderProgram, "Texture0");
//     GLint tex1Location = glGetUniformLocation(shaderProgram, "Texture1");
//     GLint tex2Location = glGetUniformLocation(shaderProgram, "Texture2");
//     GLint tex3Location = glGetUniformLocation(shaderProgram, "Texture3");

//     GLint viewLocation = glGetUniformLocation(shaderProgram, "view");
//     GLint projectLocation = glGetUniformLocation(shaderProgram, "project");

//     glUniform1i(tex0Location, 0);
//     glUniform1i(tex1Location, 1);
//     glUniform1i(tex2Location, 2);
//     glUniform1i(tex3Location, 3);

//     auto view = Math::Mat4();
//     Math::Mat4::createLookAt(
//         Math::Vec3(0, 0, 2000),
//         Math::Vec3(0, 1000, 0),
//         Math::Vec3(0, 1, 0),
//         &view
//     );
//     glUniformMatrix4fv(viewLocation, 1, GL_FALSE, view.m);

//     auto project = Math::Mat4();
//     Math::Mat4::createPerspective(
//         60,
//         1,
//         1,
//         8000,
//         &project
//     );
//     glUniformMatrix4fv(projectLocation, 1, GL_FALSE, project.m);
// }

void GLMgr::setFPS(float __targetFPS){
    targetFPS = __targetFPS;
    uSecDuration = 1000000 / __targetFPS;
}

void GLMgr::useTestTexture(){

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 8);

    const unsigned int data[] = {
        0xFFFF0000, 0xFFFFFFFF, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
        0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
    };

    int width = 4;
    int height = 4;
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

    glTexSubImage2D(
        GL_TEXTURE_2D, 
        0, 
        0, 0, 
        width, 
        height, 
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data
    );


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);




    GLuint textureId2;
    glGenTextures(1, &textureId2);

    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, textureId2);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glPixelStorei(GL_UNPACK_ALIGNMENT, 8);

    const unsigned int data2[] = {
        0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF,
        0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF,
        0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF,
        0x0000FFFF, 0x0000FFFF, 0x0000FFFF, 0x0000FFFF
    };

    int width2 = 4;
    int height2 = 4;
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width2, height2);

    glTexSubImage2D(
        GL_TEXTURE_2D, 
        0, 
        0, 0, 
        width, 
        height, 
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        data2
    );


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureId2);
}

void GLMgr::bindTexture2D(Texture2D *texture2D, GLint index){
    GLuint tex2dID = texture2D->getTexID();
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, tex2dID);
}