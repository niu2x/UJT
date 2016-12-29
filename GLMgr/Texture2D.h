#ifndef __texture2d__
#define __texture2d__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../refCount/Ref.h"

class Texture2D:public Ref{
public:
	// RGBA
	Texture2D *createWithRawData(unsigned char *data, int width, int height, GLenum image_format = GL_RGBA);
	Texture2D *createWithPNG(const char *filename, GLenum image_format);

private:

	Texture2D();
	virtual ~Texture2D();

	bool initWithRawData(unsigned char *data, int width, int height, GLenum image_format = GL_RGBA);
	bool initWithPNG(const char *, GLenum image_format);

	void use(GLint index);

	int width;
	int height;

	GLuint texID;

	bool valid;
};

#endif