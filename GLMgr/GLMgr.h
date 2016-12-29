#ifndef __class_gl__
#define __class_gl__

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <sys/time.h>
#include "Vertex.h"
#include "GLProgramSource.h"

class GLProgramSource;
class Texture2D;
class GLProgram;

class GLMgr{

public:
    typedef std::function<void(void)> RenderFunc;
    typedef std::function<bool(void)> UpdateFunc;

    ~GLMgr();
    bool init(int width, int height);
    void run();
    void setRenderFunc(RenderFunc);
    void setUpdateFunc(UpdateFunc);

    void setClearColor(float r, float g, float b, float a);
    void clearColor();
    void clearDepth();
    void clearStencil();

    void drawTriangles(
        const Vertex vertex[], 
        int num, 
        const GLushort indices [], 
        int numOfIndices
    );

    void bindTexture2D(Texture2D *, GLint index);
    void setFPS(float);
    static void createInstance();
    static void deleteInstance();
    static GLMgr* getInstance();
private:

    void useDefaultShader();

    void initVAOAndVBO();

    GLProgram *glProgram;
    GLFWwindow* window;
    int width, height;
    bool inited;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    RenderFunc renderFunc;
    UpdateFunc updateFunc;
    float targetFPS;
    unsigned int uSecDuration;
    unsigned long lastUpdateTime;
    GLMgr();
    GLMgr(const GLMgr&);
    GLMgr& operator=(const GLMgr&);
    static GLMgr *instance;
};


#endif
