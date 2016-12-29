#ifndef __GL__program__
#define __GL__program__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>

#include "../refCount/Ref.h"

class GLProgramSource;

class GLProgram:public Ref{

public:

	static GLProgram *createWithGLProgramSource(GLProgramSource *);
	void use();

	void setUniformMatrix4fv(std::string name, void *);
private:
	GLProgram();
	virtual ~GLProgram();

	bool initWithGLProgramSource(GLProgramSource *);
	bool createProgram();
	bool createShader(GLenum shaderType, const char *src, GLuint *);

	virtual void initParams();

	std::map<std::string, GLint> localtions;

    GLProgramSource *source;
	GLuint programID;
};

#endif
