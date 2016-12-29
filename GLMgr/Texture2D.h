#ifndef __texture2d__
#define __texture2d__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../refCount/Ref.h"

class Texture2D:public Ref{
public:
	// RGBA
	static Texture2D *createWithRawData(unsigned char *data, int width, int height, GLenum imageFormat);
	static Texture2D *createWithFile(const char *filename);

	GLuint getTexID()const{
		return texID;
	} 
	void use(GLint index);

private:

	Texture2D();
	virtual ~Texture2D();

	bool initWithRawData(unsigned char *data, int width, int height, GLenum imageFormat);
	bool initWithFile(const char *);


	int width;
	int height;

	GLuint texID;

	bool valid;
};

#endif